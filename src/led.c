/*****************************************************************************
 * Filename:    rocket_led.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     January 13, 2009
 *
 * Description: Rocket LED Interface Routines
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "api.h"
#include "delay.h"
#include "led.h"
#include "rocket_led_hw.h"
#include "timer_mux.h"
#if defined( PROCESSOR_PMU430 )
#include "rocket_registers.h"
#include "rocket_pmic.h"
#else
#if defined( PROCESSOR_MQ31 )
#include "lagavulin_registers.h"
#include "lagavulin_pmic.h"
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

#define LED_MAX_PATTERN_SIZE        ( sizeof(LedPattern) * 32 )
#define SLOPE_SHIFTER               6
#define MIN_TRANSITION_TIME_MS      10      // 10ms, smallest step is 5ms

BYTE i2cLedArray[1];
BYTE i2cLedPattern[ LED_MAX_PATTERN_SIZE ];

// step colour slope
SWORD RedSlope;
SWORD GreenSlope;
SWORD BlueSlope;

DWORD TransitionTimeTicks;  // time between colour change event ticks in ticks
WORD NumTransitions;
WORD TransitionCounter;

WORD PatternSize;           // contains the last address that was read from/written to
WORD LedRxIndex;

LedState statusLed = {
    (LedPattern*)i2cLedPattern,
    0,
    0,
    0,
    0,
    0,
    0
};

void LedTransitionEvent( BOOL transitioning );
void UpdateTransitionInfo( void );

// callback wrapper for timer expiry
void LedTransitionEventCb( WORD data )
{
    LedTransitionEvent( TRUE );
}

void LedTransitionEvent( BOOL transitioning )
{
    DWORD lastColour;
    DWORD nextColour;
    SWORD r,g,b;

    if( statusLed.ledState == FALSE ) {
        return;
    }

    if( statusLed.patternIndex < statusLed.patternSize ) {

        lastColour = statusLed.pattern[ statusLed.lastPatternIndex ].colour;
        nextColour = statusLed.pattern[ statusLed.patternIndex ].colour;

        if( GetLedColourHw() == nextColour || !transitioning ||
            TransitionCounter == NumTransitions ) {

            TransitionCounter = 0;

            SetLedColourHw( nextColour );

            // find/setup next colour
            statusLed.lastPatternIndex = statusLed.patternIndex;
            statusLed.patternIndex += 1;

            // if we are repeating, go back to the start of the pattern sequence
            if( statusLed.patternIndex >= statusLed.patternSize  && statusLed.repeatPattern ) {
                statusLed.patternIndex = 0;
            }

            // if we have more patterns to step through, move to next
            if( statusLed.patternIndex < statusLed.patternSize ) {
                DWORD onTicks = MSEC_TO_RTC_TICKS( statusLed.pattern[ statusLed.lastPatternIndex].duration );

                // only update transition info if we have a transition to execute
                if( statusLed.pattern[ statusLed.lastPatternIndex].transitionTime != 0 ) {
                    UpdateTransitionInfo();
                } else {
                    NumTransitions = 0;
                }

                SetTimer( STATUS_LED_TIMER, onTicks );
            }

        } else {

            r = RED_CHANNEL(lastColour) << SLOPE_SHIFTER;
            g = GREEN_CHANNEL(lastColour) << SLOPE_SHIFTER;
            b = BLUE_CHANNEL(lastColour) << SLOPE_SHIFTER;

            /* Move closer to the next colour */
            r = (SWORD)(r) + RedSlope*TransitionCounter;
            g = (SWORD)(g) + GreenSlope*TransitionCounter;
            b = (SWORD)(b) + BlueSlope*TransitionCounter;

            r = (r) >> SLOPE_SHIFTER;
            g = (g) >> SLOPE_SHIFTER;
            b = (b) >> SLOPE_SHIFTER;

             /*  We ned to do some tweaking to make things look nicer. */
            if( RedSlope > 0) {
                /* Increasing slope */
                if( (r > RED_CHANNEL(nextColour)) ||
                     (r < RED_CHANNEL( GetLedColourHw() )) ) {
                    /*  We either missed the mark or rolled over.  Just
                     *  move to the next colour then */
                    r = RED_CHANNEL(nextColour);
                }
            } else if( RedSlope < 0 ) {
                /* Decreasing slope */
                if( (r < RED_CHANNEL(nextColour)) ||
                     (r > RED_CHANNEL( GetLedColourHw() )) ) {
                    /* Red was decrementing, and we mised the mark or
                     * rolled over */
                    r = RED_CHANNEL(nextColour);
                }
            }

            if( GreenSlope > 0 ) {
                if( (g > GREEN_CHANNEL(nextColour)) ||
                     (g < GREEN_CHANNEL( GetLedColourHw() )) ) {
                    g = GREEN_CHANNEL(nextColour);
                }
            } else if( GreenSlope < 0 ) {
                if ( (g < GREEN_CHANNEL(nextColour)) ||
                     (g > GREEN_CHANNEL( GetLedColourHw() ))) {
                    g = GREEN_CHANNEL(nextColour);
                }
            }

            if( BlueSlope > 0) {
                if( (b > BLUE_CHANNEL(nextColour)) ||
                     (b < BLUE_CHANNEL( GetLedColourHw() )) ) {
                    b = BLUE_CHANNEL(nextColour);
                }
            } else if( BlueSlope < 0 ) {
                if( (b < BLUE_CHANNEL(nextColour)) ||
                     (b > BLUE_CHANNEL( GetLedColourHw() )) ) {
                    b = BLUE_CHANNEL(nextColour);
                }
            }

            SetLedColourHw( RGB_RED( r ) | RGB_GREEN( g ) | RGB_BLUE( b ) );
            TransitionCounter++;
            SetTimer( STATUS_LED_TIMER, TransitionTimeTicks );

        }


    } else {
        // pattern is done, turn off LED
        SetLedColourHw( 0 );
    }
}

void UpdateTransitionInfo( void )
{
    WORD diffRed;
    WORD diffGreen;
    WORD diffBlue;
    WORD biggestStep = 0;
    BOOL rSign, gSign, bSign;

    DWORD lastColour;
    DWORD nextColour;

    lastColour = statusLed.pattern[ statusLed.lastPatternIndex ].colour;
    nextColour = statusLed.pattern[ statusLed.patternIndex ].colour;

    NumTransitions = statusLed.pattern[ statusLed.lastPatternIndex ].transitionTime / MIN_TRANSITION_TIME_MS;
    if( NumTransitions == 0 ) {
        // if transition time is smaller then we can do, only do 1
        NumTransitions = 1;
    }

    // calculate the slope for each colour channel
    if( RED_CHANNEL(nextColour) >= RED_CHANNEL(lastColour) ) {
        diffRed = RED_CHANNEL(nextColour) - RED_CHANNEL(lastColour);
        rSign = FALSE;
    } else {
        diffRed = RED_CHANNEL(lastColour) - RED_CHANNEL(nextColour);
        rSign = TRUE;
    }
    biggestStep = diffRed;

    if( GREEN_CHANNEL(nextColour) >= GREEN_CHANNEL(lastColour) ) {
        diffGreen = GREEN_CHANNEL(nextColour) - GREEN_CHANNEL(lastColour);
        gSign = FALSE;
    } else {
        diffGreen = GREEN_CHANNEL(lastColour) - GREEN_CHANNEL(nextColour);
        gSign = TRUE;
    }
    if( diffGreen > biggestStep ) {
        biggestStep = diffGreen;
    }

    if( BLUE_CHANNEL(nextColour) >= BLUE_CHANNEL(lastColour) ) {
        diffBlue = BLUE_CHANNEL(nextColour) - BLUE_CHANNEL(lastColour);
        bSign = FALSE;
    } else {
        diffBlue = BLUE_CHANNEL(lastColour) - BLUE_CHANNEL(nextColour);
        bSign = TRUE;
    }
    if( diffBlue > biggestStep ) {
        biggestStep = diffBlue;
    }

    if( biggestStep < NumTransitions && biggestStep != 0 ) {
        // we don't need all the transitions
        NumTransitions = biggestStep;
        TransitionTimeTicks = MSEC_TO_RTC_TICKS( statusLed.pattern[ statusLed.lastPatternIndex ].transitionTime / NumTransitions );
    } else {
        TransitionTimeTicks = MSEC_TO_RTC_TICKS( MIN_TRANSITION_TIME_MS );
    }

    RedSlope = ((diffRed<<SLOPE_SHIFTER) + (NumTransitions>>1)) / NumTransitions;
    GreenSlope = ((diffGreen<<SLOPE_SHIFTER) + (NumTransitions>>1)) / NumTransitions;
    BlueSlope = ((diffBlue<<SLOPE_SHIFTER) + (NumTransitions>>1)) / NumTransitions;

    if (rSign) (RedSlope)  *= -1;
    if (gSign) (GreenSlope)*= -1;
    if (bSign) (BlueSlope) *= -1;
}

void LedInit( void )
{
    statusLed.pattern = (LedPattern*)i2cLedPattern;

    // register i2c-host interface handlers
    ApiRegisterHandlers( DEVID_LED, LedRegRead, LedRegWrite, LedStartReceived, LedStopReceived );

    RegisterTimerExpiry( STATUS_LED_TIMER, LedTransitionEventCb, 0 );

    /****** Large Pattern  (8) *****/
/*    statusLed.pattern[0].colour = 0x0000FF;
    statusLed.pattern[0].duration = 700;
    statusLed.pattern[0].transitionTime = 700;

    statusLed.pattern[1].colour = 0x000000;
    statusLed.pattern[1].duration = 50;
    statusLed.pattern[1].transitionTime = 700;


    statusLed.pattern[2].colour = 0x00FF00;
    statusLed.pattern[2].duration = 700;
    statusLed.pattern[2].transitionTime = 700;

    statusLed.pattern[3].colour = 0x000000;
    statusLed.pattern[3].duration = 50;
    statusLed.pattern[3].transitionTime = 700;

    statusLed.pattern[4].colour = 0xFF0000;
    statusLed.pattern[4].duration = 700;
    statusLed.pattern[4].transitionTime = 700;

    statusLed.pattern[5].colour = 0x000000;
    statusLed.pattern[5].duration = 50;
    statusLed.pattern[5].transitionTime = 700;

    statusLed.pattern[6].colour = 0x00FF00;
    statusLed.pattern[6].duration = 700;
    statusLed.pattern[6].transitionTime = 700;

    statusLed.pattern[7].colour = 0x000000;
    statusLed.pattern[7].duration = 50;
    statusLed.pattern[7].transitionTime = 700;*/

    /****** Smaller Fade Pattern  (3) *****/

    statusLed.pattern[0].colour = 0x000000;
    statusLed.pattern[0].duration = 1000;
    statusLed.pattern[0].transitionTime = 3000;

    statusLed.pattern[1].colour = 0x00003F;
    statusLed.pattern[1].duration = 1000;
    statusLed.pattern[1].transitionTime = 3000;

    statusLed.pattern[2].colour = 0x003F00;
    statusLed.pattern[2].duration = 1000;
    statusLed.pattern[2].transitionTime = 3000;

    statusLed.pattern[3].colour = 0x3F0000;
    statusLed.pattern[3].duration = 1000;
    statusLed.pattern[3].transitionTime = 3000;

    statusLed.pattern[4].colour = 0x3F3F3F;
    statusLed.pattern[4].duration = 1000;
    statusLed.pattern[4].transitionTime = 3000;


    /****** Blink  Pattern  (2) *****/

 /*   statusLed.pattern[0].colour = 0x00003F;
    statusLed.pattern[0].duration = 1000;
    statusLed.pattern[0].transitionTime = 1000;

    statusLed.pattern[1].colour = 0x00000;
    statusLed.pattern[1].duration = 1000;
    statusLed.pattern[1].transitionTime = 0;*/

    /****** Solid On  Pattern  (1) *****/

   /* statusLed.pattern[0].colour = 0x00003F;
    statusLed.pattern[0].duration = 1000;
    statusLed.pattern[0].transitionTime = 0;*/


    //SetLedColourPattern( statusLed.pattern, 5, TRUE );

}


void SetLedColourPattern( LedPattern *pattern, BYTE size, BOOL repeat )
{

    if( pattern == NULL ) {
        return;
    }

    if( size > LED_MAX_PATTERN_SIZE ) {
        return;
    }

    // special case, if size of pattern is 1, just set the LED colour to the value
    // specified in the pattern; ignore duration and transition times
    if( size == 1 ) {
        SetLedColourHw( pattern[0].colour );
        return;
    }

    statusLed.ledState = TRUE;
    statusLed.repeatPattern = repeat;
    statusLed.patternSize = size;

    statusLed.patternIndex = 0;
    statusLed.lastPatternIndex = 0;

    LedTransitionEvent( FALSE );
}

void StopLedColourPattern( void )
{
    StopTimer( STATUS_LED_TIMER );
    statusLed.ledState = FALSE;
}

void LedStartReceived( void )
{
    LedRxIndex = 0;
}

void LedStopReceived( void )
{
    // we received a pattern, keep track of its size for later
    if( LedRxIndex != 0 ) {
        PatternSize = LedRxIndex / sizeof( LedPattern );
    }
}

void LedRegWrite( BYTE regAddr, BYTE value )
{
    switch( regAddr ) {
        case LED_REG_CONFIG0:
            // if a pattern was active, and we cleared the bit, lets stop it
            if( statusLed.ledState && ((value & LED_CONFIG0_ENABLE) == 0) ) {
                StopLedColourPattern();
            // elseif a pattern wasn't active, and we set the bit, lets start it
            } else if( !statusLed.ledState && (value & LED_CONFIG0_ENABLE) ) {
                SetLedColourPattern( statusLed.pattern, PatternSize, TRUE );
            }
            break;
        case LED_REG_STATUS_PATTERN:
            // only modify pattern array if we arn't in the
            // middle of a pattern
            if( statusLed.ledState == FALSE ) {
                // fill the pattern array
                i2cLedPattern[ LedRxIndex++ ] = value;
            }
            break;
        default:
            break;
    }
}

BYTE LedRegRead( BYTE regAddr  )
{
    BYTE result = 0;
    switch( regAddr ) {
        case LED_REG_CONFIG0:
            result = (BYTE)statusLed.ledState;
            break;
        case LED_REG_STATUS_PATTERN:

            break;
        default:
            break;
    }
    return result;
}

