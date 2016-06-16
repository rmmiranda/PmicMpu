/*****************************************************************************
 * Filename:    gpio.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 07, 2010
 *
 * Description: GPIO module definitions
 ****************************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_

#if defined( PROCESSOR_PMU430 )

#include "rocket_gpio_def.h"

// Only PORT1 and PORT2 support interrupts
#define PAD_SUPPORTS_INTERRUPTS(x)  ( (x) < 0x0200 ? 1 : 0 )

// Gets specific port number given the line
#define GET_PORT_NUMBER(x)          ( ((x)>>3) )

#define GET_PORT_BASE(x)            ( 0x200 + ( ( (x) & 0xF0 ) << 1 ) )
#define GET_PORT_OFFSET(x)          ( (line & 0x8) ? 0x1: 0x0 )

#define PORT1_BASE                  ( 0x200 )
#define PORT2_BASE                  ( 0x201 )
#define PORT3_BASE                  ( 0x220 )
#define PORT4_BASE                  ( 0x221 )
#define PORT5_BASE                  ( 0x240 )
#define PORTJ_BASE                  ( 0x320 )

#define PIN_REG8(x)                 ( (x) + 0x00 )
#define POUT_REG8(x)                ( (x) + 0x02 )
#define PDIR_REG8(x)                ( (x) + 0x04 )
#define PREN_REG8(x)                ( (x) + 0x06 )
#define PDS_REG8(x)                 ( (x) + 0x08 )
#define PSEL_REG8(x)                ( (x) + 0x0A )
#define PIES_REG8(x)                ( (x) + 0x18 )
#define PIE_REG8(x)                 ( (x) + 0x1A )
#define PIFG_REG8(x)                ( (x) + 0x0C )
//#define PIV_REG8(x)               see below

#define PIV1_REG8                   ( GET_PORT_BASE(PAD_PORT_1)+0x0E )
#define PIV2_REG8                   ( GET_PORT_BASE(PAD_PORT_2)+0x1E )

// Port Mapping Registers & Defines
#define PMAPPWD_REG16               0x1C0
#define PMAPPWD_PASSWD              0x2D52

#define PMAPCTL_REG16               0x1C2
#define PMAPCTL_PMAPLOCKED          ( 1<<0 )
#define PMAPCTL_PMAPRECFG           ( 1<<1 )

// decodes PAD_PORT_X_Y to proper port map register addr
#define PMAP_REG8(x)                ( ((x)>>3)*8 + 0x1C8 + ((x) & 0x07) )

#elif defined( PROCESSOR_MQ31 )

#include "lagavulin_gpio_def.h"
#include "maxq31_sfr.h"

// Gets specific port and index number given the pad/line
#define GET_PORT_NUMBER(line)		( (line) >> 3 )
#define GET_INDEX_NUMBER(line)      ( (line) &  7 )

#else

#error GPIO module not defined for this processor

#endif


// TODO: see note in config_gpio.h
#include "config_gpio.h"

typedef enum {
    PAD_OUTPUT_HIGH,
    PAD_OUTPUT_LOW,
    PAD_INPUT_ENABLE,
    PAD_INPUT_PULLUP,
    PAD_INPUT_PULLDOWN,
    PAD_INTERRUPT_RISING,
    PAD_INTERRUPT_FALLING,
    PAD_INTERRUPT_DISABLE,
    PAD_INTERRUPT_CLEAR,
    PAD_ALT_MODE,
    PAD_STRENGTH_REDUCED,
    PAD_STRENGTH_FULL
} PadFunction;

typedef BYTE Line;

void GpioInit( void );
void PadConfig( Line line, PadFunction pf );
BOOL PadRead( Line line );
void PadRegisterIntHandler( Line line, FUNCTION function );
BYTE PadGetState( Line line );
WORD getPadBase( Line line );
void PadSupercapMode( void );

#endif /*_GPIO_H_*/
