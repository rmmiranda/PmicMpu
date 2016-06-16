/*****************************************************************************
 * Filename:    msp430_rtc.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     Dec 2, 2008
 *
 * Description: Register defitions for the MSP430's RTC module
 ****************************************************************************/

#ifndef __MSP430_RTC_H__
#define __MSP430_RTC_H__

#if defined( PROCESSOR_PMU430 )
#define RTC_BASE_ADDR   0x04A0
#else
 #error RTC not available on this processor
#endif

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

void UtcDisable( void );

BOOL UtcIsDisabled( void );

void UtcGetTime( BYTE * );

void UtcSetTime( BYTE * );

void UtcInit( void );

#endif
