/*****************************************************************************
 * Filename:    hostctrl_tavorpv2.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:
 *
 * Description: Power up / Power down / reset control for TavorPV
 ****************************************************************************/

#include "critical.h"
#include "registers.h"
#include "watchdog.h"
#include "intvector.h"
#include "gpio.h"
#if defined( PROCESSOR_PMU430 )
#include "rocket_sfr.h"
#include "rocket_ucs.h"
#include "msp430_rtc.h"
#include "rocket_metrics.h"
#include "rocket_pmic.h"
#else
#if defined( PROCESSOR_MQ31 )
#include "rtc.h"
#include "lagavulin_pmic.h"
#include "lagavulin_metrics.h"
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430
#include "timer.h"
#include "timer_mux.h"
#include "msgque.h"
#include "api.h"
#include "eventlog.h"
#include "config_pwrseq.h"
#include "hostctrl.h"
#include "pmic_intctrl.h"
#include "i2c.h"
#include "delay.h"
#if defined( RIM_MAX8649 ) || defined( RIM_TPS62361 )
#include "max8649.h"
#endif

// TavorPV2 Powerup Sequence Time Defines (pg. 72)
#define T_VBHRSTH_TICKS     312                                     // 9.5ms   (VCC_IO_BATT enabled to NRESET_IN high)
#define T_BFHSEH_TICKS      328                                     // TBD 10ms?  (NBATT_FAULT high to SYS_EN high)

//#define T_SEQ_ERROR         32768                                 // 1s
#define T_SEQ_ERROR         (1312)                                  // 40ms

#define VLOWSYS_WAIT_MS        1000        // number of ms to wait for VSYS to become valid on bootup
                                           // otherwise, enter LPM and wait for external power to be applied
                                           // It takes about 300ms from nRST/NMI -> VSYS becoming valid

//#define HwDelay(x)         __delay_cycles( (x)/(62.5) )

// bits indicating USB or AC power is present
#if defined( PROCESSOR_PMU430 )
#if defined( RIM_CHARGING_CONTACTS )
 #define EXT_PWR_AVAILABLE  ( PMIC_STAT_PPCHG1_AC_PRESENT | PMIC_STAT_PPCHG1_USB_PRESENT )
#else
 #define EXT_PWR_AVAILABLE  ( PMIC_STAT_PPCHG1_AC_PRESENT)
#endif
#else
#if defined( PROCESSOR_MQ31 )
#if defined( RIM_CHARGING_CONTACTS )
 #define EXT_PWR_AVAILABLE  ( CHGINT_OK_CHGINA | CHGINT_OK_CHGINB )
#else
 #define EXT_PWR_AVAILABLE  ( CHGINT_OK_CHGINA )
#endif
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

WORD AdcRawConvertVsys( void );
#define VSYS_ADC_THRESHOLD     0x777  //  (0x777*2500*3)/0xFFF = 3500mV

static HostCtrlState State;

extern BOOL AppCallsAllowed;

#if defined( SYS_EN )
void SysEnIsr()
{
    BOOL padState = PadRead( SYS_EN );
#if defined( PROCESSOR_MQ31 )
    PmicInterruptDisable( INT_GPIO_IRQ );
#endif
    PadConfig( SYS_EN, PAD_INTERRUPT_CLEAR );

    if( State == HOSTCTRL_STATE_VCCMAIN ) {
        QueueCall( HostCtrlTimerExpiry, 0 );
    } else {
        EventLogAddN( EVENTLOG_HOST_RESET_N_INT, padState );
        HostCtrlInitiateReset( RESET_COOKIE_INITIATED_HOST, RESET_PMU430_CORE | RESET_LCD_IO );
    }
}
#endif

void HostCtrlGpioResetProc( void )
{
#if defined( NO_PWR_N )
    PadConfig( NO_PWR_N, PAD_OUTPUT_LOW );
#endif
#if defined( NO_RTC_N )
    PadConfig( NO_RTC_N, PAD_OUTPUT_LOW );
#endif

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

#if defined( PROCESSOR_PMU430 )
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

#if defined( NO_PWR_N )
    PadConfig( NO_PWR_N, PAD_OUTPUT_LOW );
#endif
#if defined( NO_RTC_N )
    PadConfig( NO_RTC_N, PAD_OUTPUT_LOW );
#endif

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
#else
#if defined( PROCESSOR_MQ31 )
static void PmicFaultHandler( void )
{
	const BYTE mask[] = { PLATFORM_HARD_RESET_MASK_LIST };
	const BYTE size = sizeof(mask) / sizeof(mask[0]);
	BYTE reg;
    BYTE faults[2];
    BYTE i;

    // read the status registers so we know why we faulted
    faults[0] = PmicRead( PMIC_PDSTATUSREG1 );
    faults[1] = PmicRead( PMIC_PDSTATUSREG1 );

#if defined( NO_PWR_N )
    PadConfig( NO_PWR_N, PAD_OUTPUT_LOW );
#endif
#if defined( NO_RTC_N )
    PadConfig( NO_RTC_N, PAD_OUTPUT_LOW );
#endif

#if defined( RESET_AP_IN_N )
    PadConfig( RESET_AP_IN_N, PAD_OUTPUT_LOW );
#endif

    // disable only supply voltages which are not on the mask list
    for( reg=PMIC_VOUTCONFIG1B1 ; reg<=PMIC_L23_CNFG1 ; reg++ ) {
    	if( (reg <= PMIC_VOUTCONFIG1B5) || (reg >= PMIC_L01_CNFG1) ) {
    		// search if supply voltage is on the mask list
        	for( i=0; i<size; i++ ) {
        		if( reg == mask[i] ) {
        			break;
        		}
        	}
        	if( i == size) {
        		// supply voltage is not on the mask list
        		// supply voltage is not on the mask list
        		if( reg == PMIC_LDO_RSVD ) {
        			// reserve supply voltage
        		} else if( reg <= PMIC_VOUTCONFIG1B5 ) {
        			// SM supply voltage
        			PmicClear( reg, BUCK_PWR_MD_NORMAL );
        		} else {
        			// LDO supply voltage
        			PmicClear( reg, LDO_PWR_MD_NORMAL );
        		}
        	}
    	}
    }

    // print out the fault reasons in the log
    EventLogAddN( EVENTLOG_RESET_PMICFAULT, faults[0] );
    EventLogAddN( EVENTLOG_RESET_PMICFAULT, faults[1] );

    // initiate reset
    RimInitiateReset( RESET_COOKIE_INITIATED_PMICFAULT );
}
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

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

#if defined( PROCESSOR_PMU430 )
    // configure PG Fault handling
    // NOTE: all PG Fault interrupts use the same handler
    PmicRegisterIntHandler( INT_PWRGD_FLT_LDO0, PmicFaultHandler );

    // on MG only enable the power good fault for
    PmicInterruptEnable( INT_PWRGD_FLT_SM2 );   // 1.8DIG
    PmicInterruptEnable( INT_PWRGD_FLT_SM1 );   // 1.8DDR
    PmicInterruptEnable( INT_PWRGD_FLT_SM0 );   // 1.2DIG

#else
#if defined( PROCESSOR_MQ31 )
    // configure and unmask all PG Fault handling
    for( i = INT_LDO_A_IRQ ; i <= INT_SYSMON_IRQ ; i++ ) {
    	if( i == INT_CHARGER_IRQ ) {
    		continue;
    	}
        PmicRegisterIntHandler( (PmicInterrupt)i, PmicFaultHandler );
        PmicInterruptEnable( (PmicInterrupt)i );
    }
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430



#if defined( PROCESSOR_PMU430 )
    // if Apps exist its safe to call their entrypoint/init routine
    if( AppCallsAllowed ) {
        RocketMetrics.AppImage->entrypoint();
    }

    // Enable SM3
    PmicSet( PMIC_EN_CMD0, CMD0_EN_SM3 );
#else
#if defined( PROCESSOR_MQ31 )
    //if( AppCallsAllowed ) {
        AppEntrypoint();
    //}

    // Enable SM5
    PmicSet( PMIC_VOUTCONFIG1B5, BUCK_PWR_MD_NORMAL );
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

    State = HOSTCTRL_STATE_NORTCN;
    SetTimer( HOSTCTRL_TIMER, T_VBHRSTH_TICKS );
}

static void ExternalPwrApplied( void )
{
#if defined( PROCESSOR_PMU430 )
    PmicInterruptDisable( INT_USB_DET );
#if defined( RIM_CHARGING_CONTACTS )
    PmicInterruptDisable( INT_AC_DET );
#endif
#else
#if defined( PROCESSOR_MQ31 )
    PmicInterruptDisable( INT_CHARGER_IRQ );
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

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

#if defined( PROCESSOR_MQ31 )
    		WriteRegWordAnd( RTCCFG_REG16, ~RTCCFG_OUTENMSK );
      		WriteRegWordOr( RTCCFG_REG16, RTCCFG_OUTEN1 );
#endif // PROCESSOR_MQ31

      		PadConfig( NO_PWR_N, PAD_OUTPUT_HIGH );
            PadConfig( NO_RTC_N, PAD_OUTPUT_HIGH );

            // Debounce SYS_EN (sometimes as NO_RTC_N is released
            // SYS_EN glitches triggering us to enter a reset loop)
            State = HOSTCTRL_STATE_SYSEN_DEBOUNCE;
            SetTimer( HOSTCTRL_TIMER, 16 );
            break;

        case HOSTCTRL_STATE_SYSEN_DEBOUNCE:

            State = HOSTCTRL_STATE_VCCMAIN;
            if( !PadRead( SYS_EN ) ) {
                // setup to catch SYS_EN going high from host
                PadConfig( SYS_EN, PAD_INTERRUPT_CLEAR );
                PadConfig( SYS_EN, PAD_INTERRUPT_RISING );
#if defined( PROCESSOR_MQ31 )
                PmicInterruptEnable( INT_GPIO_IRQ );
#endif
                // Wait for SYS_EN signal from Host
                // if this timer times out; host didn't power up correctly
                SetTimer( HOSTCTRL_TIMER, T_BFHSEH_TICKS );
                break;
            } else {
                // fallthrough to HOSTCTRL_STATE_VCCMAIN
            }

        case HOSTCTRL_STATE_VCCMAIN:

            // stop the guard timer setup above to time us out if we fail to powerup
            StopTimer( HOSTCTRL_TIMER );
            // if SYS_EN went high properly, continue powering up
            if( PadRead( SYS_EN ) ) {
                    SupplySequenceRun( 0, SUPSEQ_DIR_PWRUP );
                    // setup to catch SYS_EN going low from host
                    PadConfig( SYS_EN, PAD_INTERRUPT_CLEAR );
                    PadConfig( SYS_EN, PAD_INTERRUPT_FALLING );
#if defined( PROCESSOR_MQ31 )
                    PmicInterruptEnable( INT_GPIO_IRQ );
#endif

            } else {
                // otherwise something went wrong, and shut down
                HostCtrlGpioReset( RESET_LCD_IO );

                // shut down all supplies since something went wrong
                HostCtrlDisableSupplies( RESET_DDR_SUPPLY );

                // Sequencing timed out; something went wrong
                // Wait 40ms before we try again
                State = HOSTCTRL_STATE_INIT;
                SetTimer( HOSTCTRL_TIMER, T_SEQ_ERROR );
            }

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

#if defined( PROCESSOR_PMU430 )
                if( !(PmicRead( PMIC_STAT_PPCHG1 ) & (1<<7 )) ) {
#else
#if defined( PROCESSOR_MQ31 )
                if( !(PmicRead( PMIC_STATUSSYS ) & (1<<3)) ) {
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

                    if( AdcRawConvertVsys() > VSYS_ADC_THRESHOLD ) {
                        break;
                    }
                }

#if !defined( PROCESSOR_MQ31 )
                HwDelay( 1000000 );
#endif // PROCESSOR_MQ31
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
#if defined( PROCESSOR_PMU430 )
            if( PmicRead( PMIC_STAT_PPCHG1 ) & EXT_PWR_AVAILABLE ) {
#else
#if defined( PROCESSOR_MQ31 )
            if( PmicRead( PMIC_CHG_INT_OK ) & (EXT_PWR_AVAILABLE) ) {
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430
                State = HOSTCTRL_STATE_WAIT_VSYS;
                QueueCall( HostCtrlTimerExpiry, 0 );

            // otherwise, wait for it to be connected
            } else {

#if defined( PROCESSOR_PMU430 )
                // cant powerup system, insufficient power
                // wait for an external source to be applied
                PmicRegisterIntHandler( INT_USB_DET, ExternalPwrApplied );
                PmicInterruptEnable( INT_USB_DET );

                #if defined( RIM_CHARGING_CONTACTS )
                PmicRegisterIntHandler( INT_AC_DET, ExternalPwrApplied );
                PmicInterruptEnable( INT_AC_DET );
                #endif
#else
#if defined( PROCESSOR_MQ31 )
                // cant powerup system, insufficient power
                // wait for an external source to be applied
                PmicRegisterIntHandler( INT_CHARGER_IRQ, ExternalPwrApplied );
                PmicInterruptEnable( INT_CHARGER_IRQ );
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

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
#if defined( SYS_EN )
    PadConfig( SYS_EN, PAD_INTERRUPT_DISABLE );
    PadConfig( SYS_EN, PAD_INTERRUPT_CLEAR );
#endif
}

void HostCtrlInitProc( void )
{
#if defined( SYS_EN )
    PadRegisterIntHandler( SYS_EN, SysEnIsr );
    PadConfig( SYS_EN, PAD_INPUT_PULLDOWN );
#endif

    RegisterTimerExpiry( HOSTCTRL_TIMER, HostCtrlTimerHandler, 0 );

    State = HOSTCTRL_STATE_WAIT_VSYS;
    HostCtrlTimerExpiry( 0 );
}
