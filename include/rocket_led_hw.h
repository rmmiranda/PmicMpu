/*****************************************************************************
 * Filename:    rocket_led_hw.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     January 20, 2010
 *
 * Description: Definitions of LED HW module interface
 ****************************************************************************/
#ifndef __ROCKET_LED_HW_H__
#define __ROCKET_LED_HW_H__

#define RED_CHANNEL(x)              (((x)>>16) & 0xFF)
#define GREEN_CHANNEL(x)            (((x)>>8)  & 0xFF)
#define BLUE_CHANNEL(x)             ((x)     & 0xFF)

#define RGB_RED(x)                  ((DWORD)((x) & 0xFF)<<16)
#define RGB_GREEN(x)                ((DWORD)((x) & 0xFF)<<8)
#define RGB_BLUE(x)                 ((DWORD)((x) & 0xFF))

#define RGB_CHANNEL_MASK            ( 0xFFFFFF )

BOOL  GetLedStateHw( void );
DWORD GetLedColourHw( void );
void  SetLedColourHw( DWORD colour );
void  LedInitHw( void );


#endif /* __ROCKET_LED_HW_H__ */

