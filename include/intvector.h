/*****************************************************************************
 * Filename:    intvector.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 10, 2010
 *
 * Description: Definitions interrupt vector numbers used to assign
 *              ISRs to the correct vector
 ****************************************************************************/
#ifndef __INTVECTOR_H__
#define __INTVECTOR_H__

#if defined( PROCESSOR_MQ31 )
#include "maxq31_sfr.h"
#endif

/*
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 *
 *     If the number of vectors changes, <chip>_inout.c must
 *  be updated so there is an IntVectn function for each vector
 *
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 */
typedef enum {
#if defined( PROCESSOR_PMU430 )
    RTC_VECTOR,
    PORT1_VECTOR,
    DMA_VECTOR,
    TIMERA1_1_VECTOR,
    TIMERA1_0_VECTOR,
    ADC_VECTOR,
    USCIB2_RXTX_VECTOR,
    USCIA2_RXTX_VECTOR,
    USCIA1_RXTX_VECTOR,
    USCIA0_RXTX_VECTOR,
    USCIB1_RXTX_VECTOR,
    USCIB0_RXTX_VECTOR,
    TIMERA0_1_VECTOR,
    TIMERA0_0_VECTOR,
    PORT2_VECTOR,
    WDTI_VECTOR,
    USR_NMI_VECTOR,
    SYS_NMI_VECTOR,
    RESET_VECTOR,       // dummy vector, this still needs to be in flash
    NUM_INT_VECTORS
#elif defined( PROCESSOR_MQ31 )
	PWCN_VECTOR,
    WDT_MEMF_VECTOR,
    SPI2_VECTOR,
    PR66FIQ_VECTOR,
    TIMER32K_VECTOR,
    MQ31EXT_VECTOR,
    SPMI_I2C2_VECTOR,
    SPI0_I2C0_VECTOR,
    SPI1_I2C1_VECTOR,
    ADC_VECTOR,
    RTC_VECTOR,
    TIMERB0_VECTOR,
    TIMERB1_VECTOR,
    PR66IRQ_VECTOR,
    RESERVED_VECTOR,
    UNHANDLED_VECTOR,
    NUM_INT_VECTORS
#else
#error Interrupt vector module not defined for this processor
#endif
} IntId;

void InterruptControllerInit( void );
void RegisterIntHandler( IntId intId, FUNCTION handler );
void RegisterIntHandlerReal( IntId intId, FUNCTION handler );

#endif //__INTVECTOR_H__
