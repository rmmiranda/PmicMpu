/*****************************************************************************
 * Filename:    lagavulin_utc.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 28, 2011
 *
 * Description: Using the MQ31 RTC module, keep the device's time.
 ****************************************************************************/
#include "basetype.h"
#include "rtc.h"
#include "registers.h"
#include "timer_mux.h"
#include "string.h"
#include "watchdog.h"
#include "lagavulin_registers.h"

#define UTC_REFRESH_TICKS 0xFFFF

//#pragma DATA_SECTION( UtcEnabled, ".preserved" )
__no_init BOOL UtcEnabled;

//#pragma DATA_SECTION( time, ".preserved" )
__no_init BYTE time[8];

//#pragma DATA_SECTION( rtcHigh, ".preserved" )
__no_init DWORD rtcHigh;

//#pragma DATA_SECTION( lastRtcValue, ".preserved" )
__no_init DWORD lastRtcValue;

static DWORD RtcGetCounter()
{
    DWORD val;

    do {
        val = (DWORD)ReadRegWord(RTSH_REG16) << 16;
        val +=  ReadRegWord(RTSL_REG16);
    } while( (val & 0xFFFF) != ReadRegWord(RTSL_REG16) );

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
        WriteRegWord( RCNT_REG16, RCNT_WE );
		RtcGetCounter();
	}
}

void UtcDisable()
{
    if( UtcEnabled ) {
        UtcEnabled = FALSE;
        // Disable RTC
        WriteRegWord( RCNT_REG16, RCNT_WE );
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

    // Reset second counter to 0
    WriteRegWord( RTSH_REG16, 0x0000 );
    WriteRegWord( RTSL_REG16, 0x0000 );

    // Enable RTC
    WriteRegWord( RCNT_REG16, RCNT_WE );
    WriteRegWord( RCNT_REG16, RCNT_RTCE | RCNT_WE );

    SetTimer( UTC_TIMER, UTC_REFRESH_TICKS );
}

void UtcInit()
{
    BOOL countPreserved;

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
