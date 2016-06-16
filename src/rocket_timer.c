/*****************************************************************************
 * Filename:    rocket_timer.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     November 27, 2008
 *
 * Description: Rocket Timer_A & Timer_B Module Routines
 ****************************************************************************/

#include "critical.h"
#include "registers.h"
#include "timer.h"
#include "gpio.h"
#include "intvector.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

// interrupt handlers
void TimerA0Handler( void );
void TimerA1Handler( void );

static FUNCTION muxTimerHandler = NULL;

// high 16 bits of current count
static volatile WORD HighWord = 0;


WORD GetHWTimerCountWord( void )
{
    WORD count;

    // must do a majority vote read while counter is active
    // to be sure we get the correct value (according to data sheet)
    do {
        count = ReadRegWord( TAR_REG16 );
    } while( count != ReadRegWord( TAR_REG16 ) );

    return( count );
}

DWORD GetHWTimerCountDword( void )
{
    WORD low;
    WORD high;
    WORD intFlag;
    WORD wason = CoreIntDisable();

    // make sure low and high word reads are in sync
    do {
        high = HighWord;
        low = GetHWTimerCountWord();

        // NOTE: Doing this here assumes we are not using the other
        // 2 match registers on this timer and that the only flag
        // TAIV will EVER indicate is a rollover!!!!
        intFlag = ReadRegWord( TAIV_REG16 );

        // Timer A overflow occured
        if( intFlag == 0xE ) {
            HighWord++;
        }
    } while( high != HighWord );


    CoreIntEnable( wason );

    return( ((DWORD)high<<16) | (DWORD)low );
}

void SetHWTimer( TimerType timer, WORD ticksFromNow )
{
    WORD match;
    WORD wason = CoreIntDisable();

    if( ticksFromNow < 3 ) {
        ticksFromNow = 3;
    }
    switch( timer ) {
        case MUX_TIMER:
            // Disable the counter interrupt and clear any pending flag
            WriteRegWordAnd( TACCTL_REG16( 0 ), ~(TACCTL_CCIE | TACCTL_CCIFG) );
            // setup the new match value
            match = GetHWTimerCountWord() + ticksFromNow;
            WriteRegWord( TACCR_REG16( 0 ), match );
            V3( EventLogAddN( EVENTLOG_TIMER_HWMATCH, match ) );
            // re-enable the match interrupt
            WriteRegWordOr( TACCTL_REG16( 0 ), TACCTL_CCIE );
            break;
        default:
            // TODO
            break;
    }


    CoreIntEnable( wason );
}

void StopHWTimer( TimerType timer )
{
    WORD wason = CoreIntDisable();

    switch( timer ) {
        case MUX_TIMER:
            // clear and disable the timer
            WriteRegWordAnd( TACCTL_REG16( 0 ), ~(TACCTL_CCIE | TACCTL_CCIFG) );
            break;
        default:
            // TODO
            break;
    }

    CoreIntEnable( wason );
}

void TimerHWInit( void )
{

    RegisterIntHandler( TIMERA0_0_VECTOR, TimerA0Handler );
    RegisterIntHandler( TIMERA0_1_VECTOR, TimerA1Handler );

    // Clear TAR
    WriteRegWord( TACTL_REG16, TACTL_TACLR );

    // Configure counter for continuous mode, divide clk source by 1,
    // clock source set to ACLK(32khz), and enable the timers interrupt
    WriteRegWord( TACTL_REG16, TACTL_TASSEL_ACLK | TACTL_MC_CONTINUOUS | TACTL_TAIE );

}

// disables the entire module (called when trying to enter deep sleep while running on the SuperCap
void TimerHWHalt( void )
{
    StopHWTimer( MUX_TIMER );
    // disable module interrupt (stops us from waking up every 2 seconds for the 16bit rollover event
    WriteRegWordAnd( TACTL_REG16, ~TACTL_TAIE );
}

void RegisterHWTimerExpiry( TimerType timer, FUNCTION func )
{
    switch( timer ) {
        case MUX_TIMER:
            muxTimerHandler = func;
            break;
        default:
            // TODO
            break;
    }
}

// TimerA0Handler handles interrupts triggered by a match event on match register 0
void TimerA0Handler( void )
{
    EventLogAdd( EVENTLOG_PMU430_TIMERA0_INT );
    if( muxTimerHandler != NULL ) {
        muxTimerHandler();
    }

    // CCIFG for TACCR0 is automatically cleared when we enter the ISR
}

// TimerA1Handler handles interrupts triggered by a count overflow
void TimerA1Handler( void )
{
    // Get reason for interrupt event .. currently we just care about the
    // counter overflow event. Reading this reg clears the highest priority
    // pending event
    WORD intFlag = ReadRegWord( TAIV_REG16 );

    EventLogAdd( EVENTLOG_PMU430_TIMERA1_INT );

    // Timer A overflow occured
    if( intFlag == 0xE ) {
        HighWord++;
    }

    // See note in GetHWTimerCountDword() If this is changed at all.
}

