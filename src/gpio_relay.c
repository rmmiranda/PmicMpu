/*****************************************************************************
 * Filename:    rocket_gpio_relay.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     July 15, 2010
 *
 * Description: Generic mechanism to relay gpio control from host processor
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#if defined( PROCESSOR_PMU430 )
#include "rocket_registers.h"
#else
#include "lagavulin_registers.h"
#endif
#include "api.h"
#include "msgque.h"
#include "gpio.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

static Line GpioLine;
static BYTE GpioControl;

extern WORD SysFlags0;

#if defined( PROCESSOR_PMU430 )
/******************************************************************************
 * PadGetState()
 *
 * Utility function to populate control register when its read with current
 * IO level/direction
 *****************************************************************************/
BYTE PadGetState( Line line )
{
    BYTE reg;
    BYTE padMask;
    WORD padBase;

    padMask = 1<<(line & 0x7);
    padBase = getPadBase( line );

    reg = 0;

    // GPIO configured as Output
    if( ReadRegByte( PDIR_REG8(padBase) ) & padMask ) {
        reg |= GPIO_CONTROL_DIRECTION;

        // GPIO level configured as Output: HIGH
        if( ReadRegByte( POUT_REG8(padBase) ) & padMask ) {
            reg |= GPIO_CONTROL_LEVEL;
        }
    // GPIO configured as Input
    } else {
        if( ReadRegByte( PIN_REG8(padBase) ) & padMask ) {
            reg |= GPIO_CONTROL_LEVEL;
        }
    }

    return( reg );
}
#endif

/******************************************************************************
 * gpioRegWrite()
 *
 * Handle received byte
 *****************************************************************************/
static void gpioRegWrite( BYTE reg, BYTE data )
{
    switch( reg ) {
        case GPIO_REG_LINE:
            GpioLine = (Line)data;
            break;
        case GPIO_REG_CONTROL:
            // if output
            if( data & GPIO_CONTROL_DIRECTION ) {
#if defined( RIM_FAST_RESET )
    #if defined( LCD_RST_N )
                if( (GpioLine == LCD_RST_N) && (SysFlags0 & ROCKET_SYSFLAGS0_LCD_RET_EN ) ) {
                    // don't allow host to toggle LCD_RST_N if we are in LCD Retention mode
                    break;
                }
    #endif
#endif // RIM_FAST_RESET
                if( data & GPIO_CONTROL_LEVEL ) {
                    PadConfig( GpioLine, PAD_OUTPUT_HIGH );
                } else {
                    PadConfig( GpioLine, PAD_OUTPUT_LOW );
                }
            } else {
                PadConfig( GpioLine, PAD_INPUT_ENABLE );
            }
            break;
        default:
            // do nothing
            break;
    }
}

/******************************************************************************
 * gpioRegRead()
 *
 * Provide requested byte
 *****************************************************************************/
static BYTE gpioRegRead( BYTE reg )
{
    BYTE ret;
    switch( reg ) {
        case GPIO_REG_LINE:
            ret = (BYTE)GpioLine;
            break;
        case GPIO_REG_CONTROL:
            ret = PadGetState( GpioLine );
            break;
        default:
            ret = 0;
            break;
    }
    return( ret );
}

/******************************************************************************
 * GpioRelayInit()
 *
 *****************************************************************************/
void GpioRelayInit( void )
{
    WORD wason = CoreIntDisable();

    ApiRegisterHandlers( DEVID_GPIO, gpioRegRead, gpioRegWrite, NULL, NULL );

    CoreIntEnable( wason );
}

void GpioRelayUninit( void )
{
    ApiUnregisterHandlers( DEVID_GPIO );
}

