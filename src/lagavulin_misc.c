/*****************************************************************************
 * Filename:    lagavulin_misc.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 14, 2010
 *
 * Description: Lagavulin miscellaneous routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "watchdog.h"
#include "gpio.h"
#include "intvector.h"
#include "eventlog.h"
#include "timer_mux.h"
#include "hostctrl.h"
#include "slowclock.h"

#if defined( DEVID_UPDATER_SUPPORT )
extern BOOL AppCallsAllowed;
#endif

void UtcBackup( void );

//#pragma DATA_SECTION( ResetCookie, ".preserved" )
__no_init WORD ResetCookie;

#define WDT_DISABLED_FOR_DEBUGGING 1

// 1 Second interval between watchdog kicks.
#define WDT_KICK_INTERVAL		32768 // ticks

BYTE ResetStat = 0xFF;
BYTE ResetExtra = 0xFF;

#if !defined( WDT_DISABLED_FOR_DEBUGGING )
void WdtIsr( WORD data )
{
//  PadConfig( NO_PWR_N, PadRead( NO_PWR_N ) ? PAD_OUTPUT_LOW : PAD_OUTPUT_HIGH );
    WDT_HIT();
    SetTimer( WATCHDOG_TIMER, WDT_KICK_INTERVAL );
    EventLogAdd( EVENTLOG_WATCHDOG_HIT_ISR );
//    PadConfig( HOST_I2C_SDA, PAD_OUTPUT_LOW );
}
#endif

/**
 * Handler for power fault interrupts
 */
void PowerFaultIsr( void )
{
    // Read power fault interrupt flags
    WORD flags = ReadRegWord( PWCN_REG16 );

    // Clear all pending interrupts
    WriteRegWordAnd( PWCN_REG16, ~( PWCN_AVDDI | PWCN_COMMIN0I | PWCN_COMMIN1I ) );
    WriteRegWordAnd( PWCN_REG16, ~( PWCN_VGPIO3I | PWCN_RTCI | PWCN_RTCOUTTHI ) );

    EventLogAdd( EVENTLOG_PMU430_USRNMI_INT );

    if ( flags & (PWCN_AVDDI | PWCN_COMMIN0I | PWCN_COMMIN1I | PWCN_VGPIO3I) ) {

#if !defined( WDT_DISABLED_FOR_DEBUGGING )
    	// disable the watchdog if its on since the timer used to kick it is going
        // to be shut off
        // TODO: maybe reconfigure the timer just to kick this instead?
        WDT_DISABLE();
#endif

        // Do a quick shutdown of non-PMIC die registers/things
        // since we just lost power
#if !defined( PLATFORM_RIMEVALBOARD )
        HostCtrlPowerLost();
#endif

        EventLogAdd( EVENTLOG_ENTER_SUPERCAP_LPM );

        PreEnterDeepSleep();
    }

}

BYTE GetResetReason( void )
{
    return( ResetStat );
}

void UpdateResetStat( WORD cookie )
{
    switch( cookie ) {
        case RESET_COOKIE_INITIATED:
            ResetStat = ROCKET_RESET_INITIATED;
            break;
        case RESET_COOKIE_INITIATED_LPM:
            ResetStat = ROCKET_RESET_SUPERCAP;
            break;
        case RESET_COOKIE_INITIATED_HOST:
            ResetStat = ROCKET_RESET_HOST;
            break;
        case RESET_COOKIE_INITIATED_ERROR:
            ResetStat = ROCKET_RESET_430ERROR;
            break;
        case RESET_COOKIE_INITIATED_KEYVOL:
            ResetStat = ROCKET_RESET_KEYVOL;
            break;
        case RESET_COOKIE_INITIATED_UPDATER:
            ResetStat = ROCKET_RESET_UPDATER;
            break;
        case RESET_COOKIE_INITIATED_PMICFAULT:
            ResetStat = ROCKET_RESET_430DOG;        // add new reason for this?
            break;
        case RESET_COOKIE_INITIATED_DEEPSLEEP:
            ResetStat = ROCKET_RESET_DEEPSLEEP;
            break;
        case RESET_COOKIE_INITIATED_WARMRST:
            // fallthrough
        case RESET_COOKIE_INITIATED_FASTRST:
            ResetStat = ROCKET_RESET_WARMRST;
            break;
        default:
            ResetStat = ROCKET_RESET_430DOG;
            break;
    }
}

void WdtInit( void )
{
    WORD wason = CoreIntDisable();

    // get last reason for a reset
    ResetExtra = ReadRegByte( WDCN_REG8 );

    // initialize watchdog timer register
    WriteRegByte( WDCN_REG8, 0x00 );

    if( ResetExtra & WDCN_POR ) {
        WORD pwcnReg, pwcnMask;

        // either power-on reset OR power-fail reset
        WriteRegWordAnd( WDCN_REG8, ~WDCN_POR );

        pwcnReg = ReadRegWord( PWCN_REG16 );
        pwcnMask = 0;

    	if( pwcnReg & PWCN_AVDDI ) {
			// AVDD power-fail reset
    		pwcnMask |= PWCN_AVDDI;
    	}
    	if ( pwcnReg & PWCN_COMMIN0I ) {
			// COMMIN0I power-fail reset
    		pwcnMask |= PWCN_COMMIN0I;
    	}
    	if ( pwcnReg & PWCN_COMMIN1I ) {
			// COMMIN0I power-fail reset
    		pwcnMask |= PWCN_COMMIN1I;
    	}
    	if ( pwcnReg & PWCN_VGPIO3I ) {
			// VGPIO3I power-fail reset
    		pwcnMask |= PWCN_VGPIO3I;
    	}
    	if ( pwcnReg & PWCN_RTCI ) {
			// RTCI power-fail reset
    		pwcnMask |= PWCN_RTCI;
    	}

        if( pwcnMask != 0 ) {
            pwcnReg &= ~pwcnMask;
            WriteRegWord( PWCN_REG16, pwcnReg );

            /* Do not overwrite the cookie if it was reset for
             * other known reason
             */
            if( RESET_COOKIE_UNEXPECTED == ResetCookie ) {
                ResetCookie = RESET_COOKIE_INITIATED_PMICFAULT;
            }
        }
                
    } else if( ResetExtra & WDCN_WTRF ) {
    	// watchdog timer reset
        WriteRegWordAnd( WDCN_REG8, ~WDCN_WTRF );
        if( ResetCookie != RESET_COOKIE_INITIATED_HOST ) {
            EventLogAdd( EVENTLOG_WATCHDOG_RESET );
        }

    } else {
    	if( ReadRegWord(SHDNCN_REG16) & SHDNCN_SHDNI ) {
        	// shutdown timer reset
    		WriteRegWordAnd( SHDNCN_REG16, ~SHDNCN_SHDNI );
            if( RESET_COOKIE_UNEXPECTED == ResetCookie ) {
        		ResetCookie = RESET_COOKIE_INITIATED_HOST;
            }
    	} else {
            EventLogAdd( EVENTLOG_RESET_UNEXPECETED );
    	}
    }

    // update ResetStat
    UpdateResetStat( ResetCookie );

    EventLogAddN( EVENTLOG_RESET_REASON, ResetStat );
    EventLogAddN( EVENTLOG_RESET_EXTRA, ResetExtra );
    EventLogAddN( EVENTLOG_RESET_COOKIE, ResetCookie );

    ResetCookie = RESET_COOKIE_UNEXPECTED;
    
#if defined( WDT_DISABLED_FOR_DEBUGGING )
    WDT_DISABLE();
#else

   // enable the watchdog
    WDT_ENABLE();
    WDT_HIT();
    EventLogAdd( EVENTLOG_WATCHDOG_HIT_MANUAL );
#endif

    CoreIntEnable( wason );
}

void WdtStartTimer( void )
{
#if !defined( WDT_DISABLED_FOR_DEBUGGING )
    // setup periodic timer to kick the watchdog
    RegisterTimerExpiry( WATCHDOG_TIMER, WdtIsr, 0 );
    SetTimer( WATCHDOG_TIMER, WDT_KICK_INTERVAL );
#endif
}

void WdtHit( void )
{
#if !defined( WDT_DISABLED_FOR_DEBUGGING )
    WORD wason = CoreIntDisable();
    WDT_HIT();
    EventLogAdd( EVENTLOG_WATCHDOG_HIT_MANUAL );
    CoreIntEnable( wason );
#endif
}

/*void WdtPause( void )
{
#if !defined( WDT_DISABLED_FOR_DEBUGGING )
    WORD wason = CoreIntDisable();
    StopTimer( WATCHDOG_TIMER );
    WDT_DISABLE();
    CoreIntEnable( wason );
#endif
}

void WdtResume( void )
{
#if !defined( WDT_DISABLED_FOR_DEBUGGING )
    WORD wason = CoreIntDisable();
    SetTimer( WATCHDOG_TIMER, WDT_KICK_INTERVAL );
    WDT_ENABLE();
    WDT_HIT();
    CoreIntEnable( wason );
#endif
}*/

void RimInitiateReset( WORD reason )
{
#if defined( DEVID_UPDATER_SUPPORT )
    // store UTC info across reset
    if( AppCallsAllowed ) {
        UtcBackup();
    }
#else
    UtcBackup();
#endif

    // Generate BOR
    EventLogAdd( EVENTLOG_INITIATE_RESET );

    ResetCookie = reason;

    // this should trigger a soft reset
    CoreIntDisable();
    // Set watchdog interval to the shortest possible
    WriteRegByteAnd( WDCN_REG8, ~WDCN_WD_MASK );
    while(1);
}

void ConfigureRstNmi( void )
{

    RegisterIntHandler( PWCN_VECTOR, PowerFaultIsr );

    // initialize (clear pending interrupts, disable interrupts, etc)
    WriteRegWord( PWCN_REG16, 0x0000 );

    // enable all DVDD dependent power fail interrupts
    WriteRegWordOr( PWCN_REG16, PWCN_AVDDI | PWCN_COMMIN0I | PWCN_COMMIN1I | PWCN_VGPIO3I );
}

