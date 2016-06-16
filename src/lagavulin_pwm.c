/*****************************************************************************
 * Filename:    lagavulin_pwm.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 7, 2011
 *
 * Description: PWM Control
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "timer.h"
#include "gpio.h"

/******************************************************************************
 * PwmInit()
 *
 *****************************************************************************/
void PwmInit( void )
{
    return;
}

/******************************************************************************
 * PwmUninit()
 *
 *****************************************************************************/
void PwmUninit( void )
{
    return;
}

/******************************************************************************
 * PwmConfig( WORD pwm )
 *
 *****************************************************************************/
void PwmConfig( WORD dc )
{
    // convert dc from 32KHZ ticks time base
	// to 12MHz ticks time base
	WriteRegWord( TBC0_REG16, ((dc * 12000000) / 32768) );

    return;
}

/******************************************************************************
 * PwmEnable( BOOL enable )
 *
 *****************************************************************************/
void PwmEnable( WORD dc, WORD period )
{
    if( dc ) {

    	// set output mode: reset on TBC match, set on 0x000
    	WriteRegWord( TBCN0_REG16, TBCN_TBCR );

    	// convert period from 32KHz ticks time base
    	// to 12MHz ticks time base
    	WriteRegWord( TBR0_REG16, ((period * 12000000) / 32768) );

        // configure pad output
        //PadConfig( PAD_PORT_X_X, PAD_OUTPUT_LOW );
        //PadConfig( PAD_PORT_X_X, PAD_ALT_MODE );

        // start the timer
        WriteRegWordOr( TBCN0_REG16, TBCN_TRB );

    } else {

    	// stop the timer
        WriteRegWordAnd( TBCN0_REG16, ~TBCN_TRB );

        // set pad output low
        //PadConfig( PAD_PORT_X_X, PAD_OUTPUT_LOW );

        // disable pwm mode
        WriteRegWordAnd( TBCN0_REG16, ~(TBCN_TBCR | TBCN_TBCS) );
    }

    return;
}

/******************************************************************************
 * PwmIsActive( void )
 *
 *****************************************************************************/
BOOL PwmIsActive( void )
{
    return( (ReadRegWord( TBCN0_REG16 ) & (TBCN_TBCR | TBCN_TBCS)) != 0 );
}





