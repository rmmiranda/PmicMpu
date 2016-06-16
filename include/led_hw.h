/*****************************************************************************
 * Filename:    led_hw.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 24, 2011
 *
 * Description: Definitions of LED HW module interface (renamed from
 * 				rocket_led_hw.h which was authored by Bryon Hummel and
 * 				created on January 20, 2010).
 ****************************************************************************/
#ifndef __LED_HW_H__
#define __LED_HW_H__

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


#endif /* __LED_HW_H__ */

