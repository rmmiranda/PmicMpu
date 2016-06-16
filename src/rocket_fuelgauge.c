/*****************************************************************************
 * Filename:    rocket_fuelgauge.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     May 19, 2010
 *
 * Description: Rocket fuelgauge routines
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "rocket_registers.h"
#include "gpio.h"
#include "api.h"
#include "rocket_pmic.h"
#include "i2c.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

/******************************************************************************
 * FGIntHandler()
 *
 * Handles incoming SOC_INT signal from FG and relays it via the Rocket API
 * to the host
 *****************************************************************************/
static void FGIntHandler( void )
{
    PadConfig( FG_SOC_INT, PAD_INTERRUPT_CLEAR );

    if( PadRead( FG_SOC_INT ) ) {
        PadConfig( FG_SOC_INT, PAD_INTERRUPT_FALLING );
        V1( EventLogAdd( EVENTLOG_FG_INT ) );
        ApiInterruptHost( ROCKET_INT_FG );

    } else {
        PadConfig( FG_SOC_INT, PAD_INTERRUPT_RISING );
        ApiClearInterruptHost( ROCKET_INT_FG );
    }
}


/******************************************************************************
 * FGInit()
 *
 * Configure FG_SOC_INT interrupt handler to pass the SOC_INT through to the
 * host
 *****************************************************************************/
void FGInit( void )
{

    WORD wason = CoreIntDisable();

    // enable the FG LDO
//    PmicSet( PMIC_VRTC, 1<<7 );

//    PadRegisterIntHandler( FG_SOC_INT, FGIntHandler );
    PadConfig( FG_SOC_INT, PAD_INPUT_ENABLE );
//    PadConfig( FG_SOC_INT, PAD_INTERRUPT_RISING );

    // if we are already being interrupted, handle it
//    if( PadRead( FG_SOC_INT ) ) {
//        FGIntHandler();
//    }

    V1( EventLogAdd( EVENTLOG_FG_INIT ) );

    CoreIntEnable( wason );
}

void FGUninit( void )
{
//    PadConfig( FG_SOC_INT, PAD_INTERRUPT_DISABLE );
    // disable the FG LDO
//    PmicClear( PMIC_VRTC, 1<<7 );
}

