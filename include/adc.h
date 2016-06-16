/*****************************************************************************
 * Filename:    adc.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 24, 2011
 *
 * Description: Definitions of ADC module registers (renamed from rocket_adc.h
 * 				file which had been authored by Bryon Hummel and created on
 * 				January 8, 2009)
 ****************************************************************************/
#ifndef __ADC_H__
#define __ADC_H__

#if defined( PROCESSOR_PMU430 )
#include "rocket_registers.h"
#else
#include "lagavulin_registers.h"
#endif

void AdcInit( void );
void AdcUninit( void );

BYTE AdcRegRead( BYTE reg );
void AdcRegWrite( BYTE reg, BYTE value );
void AdcStopReceived( void );
//void AdcStartReceived( void );

typedef enum {
    ADC_INHIBIT_LED,
    ADC_INHIBIT_TESTGPIO,
    ADC_NUM_INHIBITS
} AdcInhibitId;

// ADC_NUM_ENGINES are the available engines to the host, an additional engine is added on rocket
// for PA Slump detection that is not available to the host to use.
#define ADC_NUM_ENGINES_REAL    (ADC_NUM_ENGINES+1)

// dedicated ADC PA Slump engine number
#define PA_SLUMP_ENGN   (ADC_NUM_ENGINES_REAL-1)

// dedicated handler for PA slump detection ADC engine
void PaSlumpConversionComplete( WORD result );
// PaSlump detection utility functions to setup ADC conversions
void PaSlumpConfigAdcEngine( void );
void PaSlumpStartConversion( void );
void PaSlumpStopConversion( void );

void AdcInhibitChange( BYTE inhibitId, BOOL active );

#endif /* __ADC_H__ */

