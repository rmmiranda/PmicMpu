/*****************************************************************************
 * Filename:    led.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 25, 2011
 *
 * Description: Definitions of LED module interface (renamed from rocket_led.h
 * 				which was authored by Bryon Hummel and created on January 14,
 * 				2009).
 ****************************************************************************/
#ifndef __LED_H__
#define __LED_H__

/*#define RED_CHANNEL(x)              (((x)>>16) & 0xFF)
#define GREEN_CHANNEL(x)            (((x)>>8)  & 0xFF)
#define BLUE_CHANNEL(x)             ((x)     & 0xFF)

#define RGB_RED(x)                  ((DWORD)((x) & 0xFF)<<16)
#define RGB_GREEN(x)                ((DWORD)((x) & 0xFF)<<8)
#define RGB_BLUE(x)                 ((DWORD)((x) & 0xFF))

#define RGB_CHANNEL_MASK            ( 0xFFFFFF )
*/
#define LED_STATUS          0
//#define LED_TRACKBALL       1

/**
 * LED pattern structure.
 */
typedef struct {
  DWORD   colour;          ///< Colour.
  DWORD   duration;        ///< Duration.
  DWORD   transitionTime;  ///< Transition time.
} LedPattern;

/**
 * LED state structure.
 */

typedef struct {
    LedPattern *pattern;
    DWORD colour;                   // current colour of HW
    BYTE patternIndex;
    BYTE lastPatternIndex;
    BYTE patternSize;
    BOOL repeatPattern;             // 1=REPEAT Mode
    BOOL ledState;                  // 0=OFF 1=ON
    BOOL patternState;              // 0='duration state' 1='transition state'
} LedState;

void SetLedColour( DWORD colour );
void SetLedColourPattern( LedPattern *pattern, BYTE size, BOOL repeat );

void LedInit( void );
void LedStartReceived( void );
void LedStopReceived( void );
void LedRegWrite( BYTE regAddr, BYTE value );
BYTE LedRegRead( BYTE regAddr );

#endif /* __LED_H__ */

