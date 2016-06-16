/*****************************************************************************
 * Filename:    lagavulin_led_hw.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 24, 2011
 *
 * Description: Lagavulin LED HW Interface Routines
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "led_hw.h"
#include "adc.h"
#include "lagavulin_pmic.h"

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
        PmicWrite( PMIC_RLEDSET, RED_CHANNEL( LedColour ) );
        PmicWrite( PMIC_GLEDSET, GREEN_CHANNEL( LedColour ) );
        PmicWrite( PMIC_BLEDSET, BLUE_CHANNEL( LedColour ) );

        // if the LEDs were off, we need to enable them
        if( LedState == FALSE ) {
            AdcInhibitChange( ADC_INHIBIT_LED, TRUE );
            PmicWrite( PMIC_FLASHRGBON, 0xFF );
            LedState = TRUE;
        }

    } else {
        // if the LEDs were on, we need to turn them off
        if( LedState == TRUE ) {
            PmicWrite( PMIC_FLASHRGBON, 0 );
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

