/*****************************************************************************
 * Filename:    rocket_gpio.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     November 25, 2008
 *
 * Description: Rocket gpio configuration routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "watchdog.h"
#include "gpio.h"
#include "intvector.h"
#include "rocket_sfr.h"
#include "rocket_ucs.h"
#include "eventlog.h"
#include "api.h"
#include "timer_mux.h"
#include "slowclock.h"
#include "hostctrl.h"
#include "rocket_adc_hw.h"
#include "delay.h"

// interrupt handlers
void UserNMIFaultIsr( void );
void SysNMIFaultIsr( void );

#if defined( HOSTPROCESSOR_TAVOR_MG )
extern BYTE ExtSwLastWrittenValue;
extern FUNCTION1 LastMsgQCall;
#endif

#if defined( DEVID_UPDATER_SUPPORT )
extern BOOL AppCallsAllowed;
#endif

void UtcBackup( void );

#pragma DATA_SECTION( ResetCookie, ".preserved" )
WORD ResetCookie;

//#define WDT_DISABLED_FOR_DEBUGGING 1

// 2 Second interval between watchdog kicks.
#define WDT_KICK_INTERVAL  (0x10000)

BYTE ResetStat = 0xFF;
BYTE ResetExtra = 0xFF;

#if !defined( WDT_DISABLED_FOR_DEBUGGING )
void WdtIsr( WORD data )
{
    WDT_HIT();
    SetTimer( WATCHDOG_TIMER, WDT_KICK_INTERVAL );
    EventLogAdd( EVENTLOG_WATCHDOG_HIT_ISR );
}
#endif

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
            ResetStat = ROCKET_RESET_PGFAULT;
            break;
        case RESET_COOKIE_INITIATED_DEEPSLEEP:
            ResetStat = ROCKET_RESET_DEEPSLEEP;
            break;
        case RESET_COOKIE_INITIATED_WARMRST:
            // fallthrough
        case RESET_COOKIE_INITIATED_FASTRST:
            ResetStat = ROCKET_RESET_WARMRST;
            break;
        case RESET_COOKIE_INITIATED_BRICKWAKE:
            ResetStat = ROCKET_RESET_BRICKWAKE;
            break;
        default:
            ResetStat = ROCKET_RESET_430DOG;
            break;
    }
}

/**
 * WdtInit( void )
 *
 * ResetReason Based on HW Register (ResetExtra)
 *   0000h No interrupt pending
 *   0002h Brownout (BOR) (highest priority)
 *   0004h RST/NMI (BOR)
 *   0006h DoBOR (BOR)
 *   0008h Wakeup from LPM5 (BOR)
 *   000Ah Security violation (BOR)
 *   000Ch SVSL (POR)
 *   000Eh SVSH (POR)
 *   0010h SVML_OVP (POR)
 *   0012h SVMH_OVP (POR)
 *   0014h DoPOR (POR)
 *   0016h WDT time out (PUC)
 *   0018h WDT keyviol (PUC)
 *   001Ah KEYV flash keyviol (PUC)
 *   001Ch PLL unlock (PUC)
 *   001Eh PERF peripheral/configuration area fetch (PUC)
 *   0020h PMM key violation (PUC)
 *   0022h- Reserved for future extensions
 *   003Eh
 *
 */

void WdtInit( void )
{
    WORD wason = CoreIntDisable();

    // Get last reset reason from interrupt vector register
    ResetExtra = ReadRegWord( SYSRSTIV_REG16 );
    WriteRegWord( SYSRSTIV_REG16, 0x00 );


    switch( ResetExtra ) {
        // Full BOR occured
        case 0x00:
        case 0x02:
        case 0x04:
            ResetStat = ROCKET_RESET_NORTC;
            break;
        case 0x06:
            // ResetCookie should be trustable now... update ResetStat
            UpdateResetStat( ResetCookie );
            break;
        // MSP430 watchdog occured
        case 0x16:
            EventLogAdd( EVENTLOG_WATCHDOG_RESET );
            // reset with BOR since a watchdog occured
            RimInitiateReset( RESET_COOKIE_UNEXPECTED );
            break;
        default:
            EventLogAddN( EVENTLOG_RESET_UNEXPECETED, ResetExtra );
            RimInitiateReset( RESET_COOKIE_UNEXPECTED );
            break;

    }

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
#if defined( HOSTPROCESSOR_TAVOR_MG )
        EventLogAddN( EVENTLOG_STATE_EXTSW_LASTVAL, ExtSwLastWrittenValue );
        EventLogAddN( EVENTLOG_STATE_MSGQUE_LASTCALL, (WORD)LastMsgQCall );
#endif
        UtcBackup();
    }
#else
    UtcBackup();
#endif

    // Generate BOR
    EventLogAdd( EVENTLOG_INITIATE_RESET );

    // clear the previous reset reason flag (incase RST/NMI was flagged?)
    ResetExtra = WriteRegWord( SYSRSTIV_REG16, 0x00 );

    ResetCookie = reason;
    WriteRegWord( PMMCTL0_REG16, PMMCTL0_PMMKEY | PMMCTL0_PMMSWBOR );
}

void ConfigureRstNmi( void )
{

    RegisterIntHandlerReal( SYS_NMI_VECTOR, SysNMIFaultIsr );

    // if the battery can't be removed, we use the RST functionality of this RST/NMI pin
    // to get us out of a hung/stuck state since we can't pull the battery to remove power
#if !defined( RIM_NONREMOVABLE_BATTERY )
    RegisterIntHandlerReal( USR_NMI_VECTOR, UserNMIFaultIsr );

    // Disable NMI
    WriteRegWordAnd( SFRIE1_REG16, ~SFRIE1_NMIIE );

    // Configure NMI interrupt to occur on falling edge & use the NMI mode
    WriteRegWordOr( SFRRPCR_REG16, SFRRPCR_SYSNMIIES | SFRRPCR_SYSNMI );

    // make sure polarity/mode change didn't cause an interrupt
    WriteRegWordAnd( SFRIFG1_REG16, ~SFRIFG1_NMIIFG );

    // Enable NMI
    WriteRegWordOr( SFRIE1_REG16, SFRIE1_NMIIE );
#endif // RIM_NONREMOVABLE_BATTERY

    // connect BACKUP to DVCC/AVCC to charge it (and allow us to run off backup
    // if we lose power).
#if defined( BACKUP_PWR_EN )
    PadConfig( BACKUP_PWR_EN, PAD_OUTPUT_HIGH );
#endif
}


/**
 * Handler for NMI's due to Oscillator Faults, Flash Memory Access violations
 */
__interrupt void UserNMIFaultIsr( void )
{
    // Read and clear interrupt sources
    volatile WORD flags = ReadRegWord( SFRIFG1_REG16 );
    EventLogAddN( EVENTLOG_PMU430_USRNMI_INT, flags );
    WriteRegWordAnd( SFRIFG1_REG16, ~( SFRIFG1_OFIFG | SFRIFG1_VMAIFG | SFRIFG1_NMIIFG ) );

    if ( flags & SFRIFG1_NMIIFG ) {
        // If interrupt was on falling edge... Battery was removed
        if( ReadRegWord( SFRRPCR_REG16 ) & SFRRPCR_SYSNMIIES ) {

            // disable NMI while we change polarity
            WriteRegWordAnd( SFRIE1_REG16, ~SFRIE1_NMIIE );

            // Reconfigure NMI edge to detect rising edge now (signaling power applied again)
            WriteRegWordAnd( SFRRPCR_REG16, ~SFRRPCR_SYSNMIIES );

            // Make sure its still clear!
            WriteRegWordAnd( SFRIFG1_REG16, ~SFRIFG1_NMIIFG );

            // re-enable NMI
            WriteRegWordOr( SFRIE1_REG16, SFRIE1_NMIIE );

            // Do a quick shutdown of non-PMIC die registers/things
            // since we just lost power
#if !defined( PLATFORM_RIMEVALBOARD )
            HostCtrlPowerLost();
#endif // PLATFORM_RIMEVALBOARD

            PreEnterDeepSleep();

            // disable REF system to reduce current consumption
            WriteRegWord( REFCTL0_REG16, REFCTL0_REFTCOFF | REFCTL0_REFMSTR );

            #if !defined( WDT_DISABLED_FOR_DEBUGGING )
            // re-init the timer mux so we can continue kicking the watchdog
            TimerInit();
            WdtStartTimer();
            WDT_HIT();
            #endif


            EventLogAdd( EVENTLOG_ENTER_SUPERCAP_LPM );

            // Go into LPM mode when we exit NMI ISR
            _bis_SR_register_on_exit( LPM3_SR_BITS | STATUS_REG_GIE  );

        // if interrupt was on rising edge... Battery was inserted
        } else {
            EventLogAdd( EVENTLOG_EXIT_SUPERCAP_LPM );

            // reset the MSP now
            RimInitiateReset( RESET_COOKIE_INITIATED_LPM );
        }
    }

    if( flags & ( SFRIFG1_OFIFG | SFRIFG1_VMAIFG ) ) {

        // give the xtal ~2seconds to stabilize (this is assuming we are running
        // at the reduced 1Mhz core speed since HwDelay is actually calibrated
        // for 16Mhz
        HwDelay( 125000000 );
        ConfigureXt1External( 0 );
        EventLogAddN( EVENTLOG_NMI_ERROR, ReadRegWord( SFRIFG1_REG16 ) );

        if( ReadRegWord( SFRIFG1_REG16 ) & ( SFRIFG1_OFIFG | SFRIFG1_VMAIFG ) ) {
            // reset the MSP now
            RimInitiateReset( RESET_COOKIE_INITIATED_ERROR );

        }
    }
}

__interrupt void SysNMIFaultIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_SYSNMI_INT );
}

