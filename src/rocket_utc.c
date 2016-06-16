/*****************************************************************************
 * Filename:    rocket_utc.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     December 2, 2008
 *
 * Description: Using the MSP430 RTC module, keep the device's time.
 ****************************************************************************/
#include "basetype.h"
#include "msp430_rtc.h"
#include "registers.h"
#include "intvector.h"
#include "timer_mux.h"
#include "string.h"
#include "watchdog.h"
#include "rocket_registers.h"
#include "gpio.h"
#include "eventlog.h"

#define UTC_REFRESH_TICKS 0xFFFF

#pragma DATA_SECTION( UtcEnabled, ".preserved" )
BOOL UtcEnabled;

#pragma DATA_SECTION( time, ".preserved" )
BYTE time[8];

#pragma DATA_SECTION( rtcHigh, ".preserved" )
DWORD rtcHigh;

#pragma DATA_SECTION( lastRtcValue, ".preserved" )
DWORD lastRtcValue;

// interrupt handler
void RtcHandler( void );

static DWORD RtcGetCounter()
{
    DWORD val;

    do {
        val = ReadRegDword( RTCNT1 );
    } while( val != ReadRegDword( RTCNT1 ) );

    if( val < lastRtcValue ) {
        rtcHigh++;
    }
    lastRtcValue = val;

    return val;
}

static void UtcTimerExpiry( WORD data )
{
    // we need to check the counter at least once between 32-bit
    // overflows to keep the high value counter updated
    if( UtcEnabled ) {
        RtcGetCounter();
    }
    SetTimer( UTC_TIMER, UTC_REFRESH_TICKS );
}

// grabs the current UTC time and updates our preserved variables.
// Should be called as close to the reset point as possible
void UtcBackup( void )
{
	if( UtcEnabled ) {
		// forces lastRtcValue and rtcHigh to be updated so they are valid
		// after reset as a "current" preserved time.

		// turn off the RTC so we can get a quick return from RecGetCounter()
		WriteRegByte( RTCCTL1, 0x40 );
		RtcGetCounter();
	}
}

void UtcDisable()
{
    if( UtcEnabled ) {
        UtcEnabled = FALSE;
        // Disable RTC
        WriteRegByte( RTCCTL1, 0x40 );
        StopTimer( UTC_TIMER );
    }
}

BOOL UtcIsDisabled()
{
    return !UtcEnabled;
}

static void UtcGetTimeInternal( BYTE *result, BOOL restore )
{
	 if( UtcEnabled ) {
        int i;
        // temp is 80 bits to handle overflow
        BYTE temp[10];
        DWORD remain = 0;
        // copy 64 bits of RTC time into temp
        if( restore ) {
        	*((DWORD*)&temp[0]) = lastRtcValue;
        } else {
        	*((DWORD*)&temp[0]) = RtcGetCounter();
        }
        *((DWORD*)&temp[4]) = rtcHigh;
        temp[8] = 0;
        temp[9] = 0;
        // multiple temp by 1000
        for( i=0; i<10 ;i++ ) {
            remain += (DWORD)temp[i] * 1000;
            temp[i] = remain;
            remain >>= 8;
        }
        // shift temp by 15 (ie div by 32768)
        for( i=0; i<8; i++ ) {
            temp[i] = ( temp[i+1] >> 7 ) | ( temp[i+2] << 1 );
        }
        // result is temp + time
        remain = 0;
        for( i=0; i<8; i++ ) {
            remain += (WORD)temp[i] + (WORD)time[i];
            result[i] = remain;
            remain >>= 8;
        }

    } else {
        memset( result, 0xFF, 8 );
    }
}

static void UtcRestore( void )
{
	BYTE temp[8];
	UtcGetTimeInternal( temp, TRUE );
	UtcSetTime( temp );
}

void UtcGetTime( BYTE *result )
{
    UtcGetTimeInternal( result, FALSE );
}

void UtcSetTime( BYTE *data )
{
    UtcDisable();
    memcpy( time, data, 8 );

    // Enable UTC
    rtcHigh = 0;
    lastRtcValue = 0;
    UtcEnabled = TRUE;

    // Reset counter to 0
    WriteRegDword( RTCNT1, 0 );

    // Setup RTC:
    // - counter mode
    // - counter enabled
    // - use ACLK
    WriteRegByte( RTCCTL1, 0x00 );
    SetTimer( UTC_TIMER, UTC_REFRESH_TICKS );
}

void UtcInit()
{
    BOOL countPreserved;

    RegisterIntHandler( RTC_VECTOR, RtcHandler );
    RegisterTimerExpiry( UTC_TIMER, UtcTimerExpiry, 0 );

    switch( GetResetReason() ) {
        case ROCKET_RESET_NORTC:
        case ROCKET_RESET_430DOG:
        case ROCKET_RESET_430ERROR:
        case ROCKET_RESET_UPDATER:
            countPreserved = FALSE;
            break;
        default:
            countPreserved = TRUE;
    }

    // if we did a controlled reset (IE RimInitiateReset) we can trust
    // the preserved UTC variables
    if( countPreserved && UtcEnabled ) {
    	// if the UTC was enabled pre-reset lets set it up again and continue counting
  		UtcRestore();
		SetTimer( UTC_TIMER, UTC_REFRESH_TICKS );
    } else {
    	memset( time, 0, 8 );
    	UtcEnabled = FALSE;
    }
}

void RtcHandler( void )
{
    EventLogAdd( EVENTLOG_PMU430_RTC_INT );
    // This interrupt is not used
}

