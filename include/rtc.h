/*****************************************************************************
 * Filename:    rtc.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 28, 2011
 *
 * Description: RTC module
 ****************************************************************************/

#ifndef __RTC_H__
#define __RTC_H__

#if defined( PROCESSOR_PMU430 )

#define RTC_BASE_ADDR   0x04A0

#define RTCCTL0         (RTC_BASE_ADDR + 0x00 )
#define RTCCTL1         (RTC_BASE_ADDR + 0x01 )
#define RTCCTL2         (RTC_BASE_ADDR + 0x02 )
#define RTCPS0CTL       (RTC_BASE_ADDR + 0x08 )
#define RTCPS1CTL       (RTC_BASE_ADDR + 0x0A )
#define RTCPS0          (RTC_BASE_ADDR + 0x0C )
#define RTCPS1          (RTC_BASE_ADDR + 0x0D )
#define RTCIV           (RTC_BASE_ADDR + 0x0E )
// counter mode
#define RTCNT1          (RTC_BASE_ADDR + 0x10 )
#define RTCNT2          (RTC_BASE_ADDR + 0x11 )
#define RTCNT3          (RTC_BASE_ADDR + 0x12 )
#define RTCNT4          (RTC_BASE_ADDR + 0x13 )
// calendar mode
#define RTCSEC          (RTC_BASE_ADDR + 0x10 )
#define RTCMIN          (RTC_BASE_ADDR + 0x11 )
#define RTCHOUR         (RTC_BASE_ADDR + 0x12 )
#define RTCDOW          (RTC_BASE_ADDR + 0x13 )
#define RTCDAY          (RTC_BASE_ADDR + 0x14 )
#define RTCMON          (RTC_BASE_ADDR + 0x15 )
#define RTCYEARL        (RTC_BASE_ADDR + 0x16 )
#define RTCYEARH        (RTC_BASE_ADDR + 0x17 )
#define RTCAMIN         (RTC_BASE_ADDR + 0x18 )
#define RTCAHOUR        (RTC_BASE_ADDR + 0x19 )
#define RTCADOW         (RTC_BASE_ADDR + 0x1A )
#define RTCADOY         (RTC_BASE_ADDR + 0x1B )

#elif defined( PROCESSOR_MQ31 )

#include "maxq31_sfr.h"

#else

#error RTC module not defined for this processor

#endif


void UtcDisable( void );

BOOL UtcIsDisabled( void );

void UtcGetTime( BYTE * );

void UtcSetTime( BYTE * );

void UtcInit( void );

#endif // __RTC_H__
