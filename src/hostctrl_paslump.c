/*****************************************************************************
 * Filename:    hostctrl_paslump.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     April 2011
 *
 * Description: Rocket pa slump detection routines
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "timer_mux.h"
#include "eventlog.h"
#if defined( PROCESSOR_PMU430 )
#include "rocket_registers.h"
#else
#include "lagavulin_registers.h"
#endif
#include "api.h"
#include "adc.h"

// 3300mv threshold             ( ( 3300 * 0xFFF ) / (2500*3) )
#define PA_SLUMP_THRESHOLD      ( 1801 )

static BYTE PaSlumpConfig;
static WORD PaSlumpVbat = 0;
static WORD PaSlumpTresh;

/******************************************************************************
 * PaSlumpConversionComplete()
 *
 * ADC engine calles this function as an 'interrupt' when conversion is complete
 * after its been triggered
 *****************************************************************************/
void PaSlumpConversionComplete( WORD result )
{

    if( result < PaSlumpTresh ) {
        PaSlumpVbat = result;
        // slump detected, disable enging and interrupt host
        PaSlumpConfig = PASLUMP_DETECTED;
        ApiInterruptHost( ROCKET_INT_PASLUMP );
    } else {
        // no slump detected, setup next trigger
#if defined( TX_EN )
        PadConfig( TX_EN, PAD_INTERRUPT_RISING );
#endif
    }
}

/******************************************************************************
 * PaSlumpTriggerReading()
 *
 * a reading was triggered, start conversion
 *****************************************************************************/
static void PaSlumpTriggerReading( void )
{
#if defined( TX_EN )
    PadConfig( TX_EN, PAD_INTERRUPT_DISABLE );
#endif
    PaSlumpStartConversion();
}

/******************************************************************************
 * PaSlumpEnable()
 *
 * Enable/disable PA slump detection
 *****************************************************************************/
static void PaSlumpEnable( BOOL enable )
{
    if( enable && !(PaSlumpConfig & PASLUMP_ENABLE) ) {
        PaSlumpConfig |= PASLUMP_ENABLE;
#if defined( TX_EN )
        PadConfig( TX_EN, PAD_INTERRUPT_RISING );
#endif
    } else if( !enable && (PaSlumpConfig & PASLUMP_ENABLE) ) {
        PaSlumpConfig &= ~PASLUMP_ENABLE;
#if defined( TX_EN )
        PadConfig( TX_EN, PAD_INTERRUPT_DISABLE );
#endif
        PaSlumpStopConversion();
    }
}

/******************************************************************************
 * FtRegRead()
 *
 * I2C Interface to PA slump feature (read)
 *****************************************************************************/
static BYTE PaSlumpRegRead( BYTE reg )
{
    BYTE ret;

    switch( reg ) {
        case PASLUMP_REG_CONFIG:
            ret = PaSlumpConfig;
            // clear signal to host
            if( PaSlumpConfig & PASLUMP_DETECTED ) {
                ApiClearInterruptHost( ROCKET_INT_PASLUMP );
                // auto clear PASLUMP_DETECTED status bit
                PaSlumpConfig &= ~PASLUMP_DETECTED;
            }
            return( ret );
        case PASLUMP_REG_VBAT_MSB:
            return( PaSlumpVbat>>8 );
        case PASLUMP_REG_VBAT_LSB:
            return( PaSlumpVbat & 0xFF );
        case PASLUMP_REG_TRESH_MSB:
            return( PaSlumpTresh>>8 );
        case PASLUMP_REG_TRESH_LSB:
            return( PaSlumpTresh & 0xFF );

        default:
            return 0;
    }
}

/******************************************************************************
 * PaSlumpRegWrite()
 *
 * I2C Interface to PA slump feature (write)
 *****************************************************************************/
static void PaSlumpRegWrite( BYTE reg, BYTE value )
{
    switch( reg ) {
        case PASLUMP_REG_CONFIG:
            PaSlumpEnable( value & PASLUMP_ENABLE );
            break;
        case PASLUMP_REG_TRESH_MSB:
            PaSlumpTresh = (value<<8) | (PaSlumpTresh & 0xFF);
            break;
        case PASLUMP_REG_TRESH_LSB:
            PaSlumpTresh = value | (PaSlumpTresh & 0xFF00);
            break;
        default:
            break;
    }
}



/******************************************************************************
 * HostCtrlPaSlumpInit( )
 *
 * Init PA slump detection functionality
 *****************************************************************************/
void HostCtrlPaSlumpInit( void )
{
    ApiRegisterHandlers( DEVID_PASLUMP, PaSlumpRegRead, PaSlumpRegWrite, NULL, NULL );
    PaSlumpConfig = 0;
    PaSlumpConfigAdcEngine();
    PaSlumpTresh = PA_SLUMP_THRESHOLD;
#if defined( TX_EN )
    PadRegisterIntHandler( TX_EN, PaSlumpTriggerReading );
#endif
}

/******************************************************************************
 * HostCtrlPaSlumpUninit( )
 *
 * Uninit PA slump detection functionality
 *****************************************************************************/
void HostCtrlPaSlumpUninit( void )
{
    ApiUnregisterHandlers( DEVID_PASLUMP );
    PaSlumpEnable( FALSE );
}


