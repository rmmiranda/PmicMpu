/*****************************************************************************
 * Filename:    rocket_pwm.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Aug 10, 2010
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
    // set a new duty cycle match value
    WriteRegWord( TA1CCR1_REG16, dc );

    return;
}

/******************************************************************************
 * PwmEnable( BOOL enable )
 *
 *****************************************************************************/
void PwmEnable( WORD dc, WORD period )
{
    if( dc ) {

        // set output mode to Toggle/Set
        WriteRegWord( TA1CCTL_REG16(1), (7<<5) );

        // configure timer module for ACLK(32k), UP mode
        WriteRegWord( TA1CTL_REG16, TACTL_TASSEL_SMCLK | TACTL_MC_UP );
        // set the period
        WriteRegWord( TA1CCR0_REG16, period );

        // configure the duty cycle
        // if DC == Period, there is a 130ns Glitch where the PWM output goes low
        // if DC == 0, the output stays low all the time as expected
        PwmConfig( dc );

        // set direction to output
        PadConfig( PAD_PORT_X_X, PAD_OUTPUT_LOW );
        // set alt mode
        PadConfig( PAD_PORT_X_X, PAD_ALT_MODE );

    } else {
        // disable able mode, and set output low
        PadConfig( PAD_PORT_X_X, PAD_OUTPUT_LOW );
        // set mode to STOP
        WriteRegWordAnd( TA1CTL_REG16, ~TACLT_MC_MASK );
    }

    return;
}

/******************************************************************************
 * PwmIsActive( void )
 *
 *****************************************************************************/
BOOL PwmIsActive( void )
{
    return( (ReadRegWord( TA1CTL_REG16 ) & TACTL_MC_MASK) != 0 );
}





