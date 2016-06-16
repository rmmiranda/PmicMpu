/*****************************************************************************
 * Filename:    lagavulin_adc_hw.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     Feb 18, 2011
 *
 * Description: Definitions of ADC module registers
 ****************************************************************************/
#ifndef __LAGAVULIN_ADC_HW_H__
#define __LAGAVULIN_ADC_HW_H__

#include "maxq31_sfr.h"

// Enum corresponds to ADCSELx bit of the ADCSEL0 and ADCSEL1 registers
typedef enum {
    CH_BYP,
    CH_TDIE,
    CH_VBBATT,
    CH_VSYS,
    CH_VCHGINA,
    CH_VCHGINB,
    CH_IMON,
    CH_FLED1,
    CH_FLED2,
    CH_I_CHG,
    CH_ADC1,
    CH_ADC2,
    CH_ADC3,
    CH_ADC4,
    CH_ADC5,
    CH_ADC6,
    CH_ADC7,
    CH_ADC8,
    NUM_ADC_CHANNELS
} AdcChannel;

typedef enum {
    CHREF_AVCC_AVSS,
    CHREF_VREFP_AVSS,
    CHREF_VEREFP_AVSS,
    CHREF_VEREFP_AVSS_2,
    CHREF_AVCC_VEREFN,
    CHREF_VREFP_VEREFN,
    CHREF_VEREFP_VEREFN,
    CHREF_VEREFP_VEREFN_2
} AdcChannelRef;


void AdcInitHw( void );

void AdcConvertHw( AdcChannel ch, AdcChannelRef cr, FUNCTION1 ccCallback );

WORD AdcReadSampleHw( void );

void AdcHalt( void );

#endif /* __LAGAVULIN_ADC_HW_H__ */

