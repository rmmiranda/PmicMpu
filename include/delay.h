/****************************************************************************
 * Filename:    delay.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Dec 22, 2008
 *
 * Description: HwDelay definitions
 ****************************************************************************/
#ifndef __DELAY_H__
#define __DELAY_H__

#include <basetype.h>

#define HUNDRETH_OF_MILLISECOND      10000UL
#define TENTH_OF_MILLISECOND        100000UL
#define ONE_HALF_MILLISECOND        500000UL
#define ONE_MILLI_SEC              1000000UL
#define TEN_MILLISECOND           10000000UL
#define ONE_TWENTIETH_OF_SEC      50000000UL
#define TENTH_OF_SEC             100000000UL
#define TWO_FIFTHS_OF_SEC        400000000UL
#define ONE_HALF_OF_SEC          500000000UL
#define ONE_SEC                 1000000000UL
#define TWO_SEC                 2000000000UL

#if defined( PROCESSOR_PMU430 )
#define HwDelay( x )        __delay_cycles( (x) / 62.5 )
#elif defined( PROCESSOR_MQ31 )
void HwDelay( DWORD ticksFromNow );
#else
#error HwDelay not defined for this processor
#endif

#define HW_DELAY_TICKS(x)		HwDelay( (x) )
#define HW_DELAY_SEC(x)			HwDelay( RTC_TICKS_PER_SECOND*(x) )
#define HW_DELAY_MSEC(x)		HwDelay( MSEC_TO_RTC_TICKS((x)) )
#define HW_DELAY_USEC(x)		HwDelay( USEC_TO_RTC_TICKS((x)) )

#endif // __DELAY_H__

