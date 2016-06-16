/*****************************************************************************
 * Filename:    rocket_led_hw.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     January 20, 2010
 *
 * Description: Rocket LED HW Interface Routines
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "rocket_led_hw.h"
#include "rocket_registers.h"
#include "rocket_pmic.h"
#include "adc.h"

static DWORD LedColour;
static BOOL LedState;

BOOL GetLedStateHw( void )
{
    return( LedState );
}

DWORD GetLedColourHw( void )
{
    return( LedColour );
}

void SetLedColourHw( DWORD colour )
{

    LedColour = colour & RGB_CHANNEL_MASK;

    if( LedColour ) {
        PmicWrite( PMIC_PWM_RED, RED_CHANNEL( LedColour ) );
        PmicWrite( PMIC_PWM_GRN, GREEN_CHANNEL( LedColour ) );
        PmicWrite( PMIC_PWM_BLU, BLUE_CHANNEL( LedColour ) );

        // if the LEDs were off, we need to enable them
        if( LedState == FALSE ) {
            AdcInhibitChange( ADC_INHIBIT_LED, TRUE );
            PmicWrite( PMIC_RGB_CTRL, 0xFF );
            LedState = TRUE;
        }

    } else {
        // if the LEDs were on, we need to turn them off
        if( LedState == TRUE ) {
            PmicWrite( PMIC_RGB_CTRL, 0 );
            LedState = FALSE;
            AdcInhibitChange( ADC_INHIBIT_LED, FALSE );
        }
    }
}

void LedInitHw( void )
{
    LedState = 0;
    LedColour = 0;
}

