/*****************************************************************************
 * Filename:    hostctrl_eshel.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:
 *
 * Description: Power up / Power down / reset control for Eshel
 ****************************************************************************/

#include "critical.h"
#include "registers.h"
#include "watchdog.h"
#include "intvector.h"
#include "gpio.h"
#include "rocket_sfr.h"
#include "rocket_ucs.h"
#include "msp430_rtc.h"
#include "timer.h"
#include "timer_mux.h"
#include "msgque.h"
#include "api.h"
#include "eventlog.h"
#include "rocket_pmic.h"
#include "config_pwrseq.h"
#include "hostctrl.h"
#include "pmic_intctrl.h"
#include "i2c.h"
#include "rocket_metrics.h"
#include "delay.h"
#if defined( RIM_MAX8649 ) || defined( RIM_TPS62361 )
#include "max8649.h"
#endif

// Eshel SOD timing
#define T_VBHRSTH_TICKS     164                                     // 5ms

//#define T_SEQ_ERROR         32768                                 // 1s
#define T_SEQ_ERROR         (1312)                                  // 40ms

#define VLOWSYS_WAIT_MS        1000        // number of ms to wait for VSYS to become valid on bootup
                                           // otherwise, enter LPM and wait for external power to be applied
                                           // It takes about 300ms from nRST/NMI -> VSYS becoming valid

// bits indicating USB or AC power is present
#if defined( RIM_CHARGING_CONTACTS )
 #define EXT_PWR_AVAILABLE  ( PMIC_STAT_PPCHG1_AC_PRESENT | PMIC_STAT_PPCHG1_USB_PRESENT )
#else
 #define EXT_PWR_AVAILABLE  ( PMIC_STAT_PPCHG1_AC_PRESENT)
#endif

WORD AdcRawConvertVsys( void );
#define VSYS_ADC_THRESHOLD     0x777  //  (0x777*2500*3)/0xFFF = 3500mV

static HostCtrlState State;

extern BOOL AppCallsAllowed;

void HostCtrlGpioResetProc( void )
{
#if defined( EXT_32K )
    PadConfig( EXT_32K, PAD_OUTPUT_LOW );
    PadConfig( EXT_32K, PAD_STRENGTH_FULL );
#endif

#if defined( PMIC_32K_CLK )
    PadConfig( PMIC_32K_CLK, PAD_OUTPUT_LOW );
    PadConfig( PMIC_32K_CLK, PAD_STRENGTH_FULL );
#endif
}

void HostCtrlUpdateState( HostCtrlState newState )
{
    State = newState;
}

static void PmicFaultHandler( void )
{
    BYTE faults[8];
    BYTE i;

    // read the status registers so we know why we faulted
    for( i=0 ; i<5 ; i++ ) {
        faults[i] = PmicRead( PMIC_STAT0+i );
    }
    faults[5] = PmicRead( PMIC_STAT_PPCHG0 );
    faults[6] = PmicRead( PMIC_STAT_PPCHG1 );
    faults[7] = PmicRead( PMIC_STAT_PPCHG2 );

#if defined( RESET_AP_IN_N )
    PadConfig( RESET_AP_IN_N, PAD_OUTPUT_LOW );
#endif

    // turn off all supplies and reset
    PmicClear( PMIC_EN_CMD0, ~CMD0_HARD_RESET_MASK );
    PmicClear( PMIC_EN_CMD1, ~CMD1_HARD_RESET_MASK );
    PmicClear( PMIC_EN_CMD2, ~CMD2_HARD_RESET_MASK );

    // print out the fault reasons in the log
    for( i=0 ; i<8 ; i++ ) {
        EventLogAddN( EVENTLOG_RESET_PMICFAULT, faults[i] );
    }

    RimInitiateReset( RESET_COOKIE_INITIATED_PMICFAULT );

}

static void HostCtrlStateInit( void )
{
    BYTE i;

#if defined( RIM_MAX8649 ) || defined( RIM_TPS62361 )
    #if defined( CORE_SW_VID0 )
    PadConfig( CORE_SW_VID0, PAD_OUTPUT_LOW );
    #endif
    #if defined( CORE_SW_EN )
    PadConfig( CORE_SW_EN, PAD_OUTPUT_LOW );
    #endif
#endif

    HostCtrlDisableSupplies( HostCtrlGetResetFlags() );
    HostCtrlGpioReset( HostCtrlGetResetFlags() );

    // if fault tolerance switch is present, enable it
#if defined( FLT_EN_N )
    PadConfig( FLT_EN_N, PAD_OUTPUT_HIGH );
#endif

    // configure PG Fault handling
    // NOTE: all PG Fault interrupts use the same handler
    PmicRegisterIntHandler( INT_PWRGD_FLT_LDO0, PmicFaultHandler );
    PmicRegisterIntHandler( INT_LOWSYS, PmicFaultHandler );
    // unmask all the PG Faults
    for( i=INT_PWRGD_FLT_LDO0 ; i<=INT_PWRGD_FLT_SM3 ; i++ ) {
        // except LD5 (USBLDO). Don't trip a PG fault with it
        if( i != INT_PWRGD_FLT_LDO5 ) {
            PmicInterruptEnable( (PmicInterrupt)i );
        }
    }
    PmicInterruptEnable( INT_LOWSYS );

    // if Apps exist its safe to call their entrypoint/init routine
    if( AppCallsAllowed ) {
        RocketMetrics.AppImage->entrypoint();
    }

    // Enable SM3
    PmicSet( PMIC_EN_CMD0, CMD0_EN_SM3 );

    State = HOSTCTRL_STATE_NORTCN;
    SetTimer( HOSTCTRL_TIMER, T_VBHRSTH_TICKS );
}

static void ExternalPwrApplied( void )
{
    PmicInterruptDisable( INT_USB_DET );
#if defined( RIM_CHARGING_CONTACTS )
    PmicInterruptDisable( INT_AC_DET );
#endif

    // once external power is applied, make sure VSYS comes up
    // to a valid voltage before running through INIT
    State = HOSTCTRL_STATE_WAIT_VSYS;
    QueueCall( HostCtrlTimerExpiry, 0 );
    return;
}

void HostCtrlTimerExpiry( WORD data )
{
    WORD i;
    switch( State ) {
        case HOSTCTRL_STATE_INIT:

            HostCtrlStateInit();
            break;

        case HOSTCTRL_STATE_NORTCN:

            #if defined( EXT_32K )
            // Begin Output of 32KHz clock to Host
            PadConfig( EXT_32K, PAD_OUTPUT_HIGH );
            PadConfig( EXT_32K, PAD_ALT_MODE );       // ACLK Out
            #endif

            #if defined( PMIC_32K_CLK )
            PadConfig( PMIC_32K_CLK, PAD_OUTPUT_HIGH );
            PadConfig( PMIC_32K_CLK, PAD_ALT_MODE );
            #endif

            // Wait 500uS and then begin the VCC_MAIN/system supply sequencing
            State = HOSTCTRL_STATE_VCCMAIN;
            SetTimer( HOSTCTRL_TIMER, 16 );
            break;

        case HOSTCTRL_STATE_VCCMAIN:

            SupplySequenceRun( 0, SUPSEQ_DIR_PWRUP );

            break;
        case HOSTCTRL_STATE_DISCHARGE:

            HostCtrlDisableSupplies( HostCtrlGetResetFlags() );

            if( HostCtrlGetResetFlags() & RESET_PMU430_CORE ) {
                State = HOSTCTRL_STATE_PMU430_RESET;
            } else if( HostCtrlGetResetFlags() & RESET_WAIT_EXTPWR ) {
                State = HOSTCTRL_STATE_WAIT_EXTPWR;
            } else{
                State = HOSTCTRL_STATE_INIT;
            }

            HostCtrlPmicRegisterReset( HostCtrlGetResetFlags() );
            PmicIntCtrlHostUninit();

            // Wait 40ms before we try again
            SetTimer( HOSTCTRL_TIMER, T_SEQ_ERROR );

            break;

        case HOSTCTRL_STATE_POWERED:

            EventLogAdd( EVENTLOG_HOST_POWERUP_COMPLETE );

            // now that the host is powered, enable the comm interface
            // and adjust VCC_MAIN
#if defined( RIM_MAX8649 ) || defined( RIM_TPS62361 )
            Max8649Init();
#endif
            I2cInit();

            break;

        case HOSTCTRL_STATE_PMU430_RESET:
            RimInitiateReset( HostCtrlGetResetReason() );
            break;

        case HOSTCTRL_STATE_WAIT_VSYS:

            // enable this ASAP incase we just browned out and are waiting for an external supply
            if( AppCallsAllowed ) {
                EventLogAdd( EVENTLOG_RTC_INIT );
                UtcInit();
            }

            // give up to VLOWSYS_WAIT_MS for VSYS to become valid
            for( i=0 ;i<VLOWSYS_WAIT_MS ; i++ ) {
                if( !(PmicRead( PMIC_STAT_PPCHG1 ) & (1<<7 )) ) {

                    if( AdcRawConvertVsys() > VSYS_ADC_THRESHOLD ) {
                        break;
                    }
                }
                HwDelay( 1000000 );
            }

            if( i < VLOWSYS_WAIT_MS ) {
                State = HOSTCTRL_STATE_INIT;
                QueueCall( HostCtrlTimerExpiry, 0 );
                break;
            } else {
                State = HOSTCTRL_STATE_WAIT_EXTPWR;
                // fallthrough
            }

        case HOSTCTRL_STATE_WAIT_EXTPWR:


            // if power is already present, move to next state right away
            if( PmicRead( PMIC_STAT_PPCHG1 ) & EXT_PWR_AVAILABLE ) {
                State = HOSTCTRL_STATE_WAIT_VSYS;
                QueueCall( HostCtrlTimerExpiry, 0 );

            // otherwise, wait for it to be connected
            } else {
                // cant powerup system, insufficient power
                // wait for an external source to be applied
                PmicRegisterIntHandler( INT_USB_DET, ExternalPwrApplied );
                PmicInterruptEnable( INT_USB_DET );

                #if defined( RIM_CHARGING_CONTACTS )
                PmicRegisterIntHandler( INT_AC_DET, ExternalPwrApplied );
                PmicInterruptEnable( INT_AC_DET );
                #endif

                // if we reset due to insufficient power and are waiting
                // on VSYS to come up, disable all the supplies (DDR included)
                HostCtrlDisableSupplies( RESET_DDR_SUPPLY );
            }


            break;

        default:
            break;
    }

}

void HostCtrlTimerHandler( WORD unused )
{
    QueueCall( HostCtrlTimerExpiry, 0 );
}

void HostCtrlPrepResetProc( void )
{
}

void HostCtrlInitProc( void )
{
    RegisterTimerExpiry( HOSTCTRL_TIMER, HostCtrlTimerHandler, 0 );

    State = HOSTCTRL_STATE_WAIT_VSYS;
    HostCtrlTimerExpiry( 0 );
}
