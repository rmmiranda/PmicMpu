/*****************************************************************************
 * Filename:    hostctrl_omap4430.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:
 *
 * Description: Power up / Power down / reset control for OMAP4430
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

//#define T_SEQ_ERROR         32768                                 // 1s
#define T_SEQ_ERROR         (1312)                                  // 40ms

#define VLOWSYS_WAIT_MS        1000        // number of ms to wait for VSYS to become valid on bootup
                                           // otherwise, enter LPM and wait for external power to be applied
                                           // It takes about 300ms from nRST/NMI -> VSYS becoming valid

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
#if defined( PROCESSOR_PMU430 )
#define VSYS_ADC_THRESHOLD     0x777  //  (0x777*2500*3)/0xFFF = 3500mV
#else
#if defined( PROCESSOR_MQ31 )
#define VSYS_ADC_THRESHOLD     0xAF0  //   0xAF0 * 1.25mV = 3500mV
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

static HostCtrlState State;

extern BOOL AppCallsAllowed;

#if defined( NRESWARM )
static void nreswarmIsr()
{
    BOOL padState = PadRead( NRESWARM );
#if defined( PROCESSOR_MQ31 )
    PmicInterruptDisable( INT_GPIO_IRQ );
#endif
    PadConfig( NRESWARM, PAD_INTERRUPT_CLEAR );

    // NRESWARM went low, trigger a reset
    EventLogAddN( EVENTLOG_HOST_RESET_N_INT, padState );
    HostCtrlInitiateReset( RESET_COOKIE_INITIATED_HOST, RESET_PMU430_CORE | RESET_LCD_IO );
}
#endif

void HostCtrlGpioResetProc( void )
{
#if defined( SYS_32K_CLK0 )
    PadConfig( SYS_32K_CLK0, PAD_OUTPUT_LOW );
    PadConfig( SYS_32K_CLK0, PAD_STRENGTH_FULL );
#endif

#if defined( SYS_32K_CLK1 )
    PadConfig( SYS_32K_CLK1, PAD_OUTPUT_LOW );
    PadConfig( SYS_32K_CLK1, PAD_STRENGTH_FULL );
#endif

#if defined( NRESPWRON )
    PadConfig( NRESPWRON, PAD_OUTPUT_LOW );
#endif

#if defined( MSECURE )
    PadConfig( MSECURE, PAD_INPUT_ENABLE );
#endif
#if defined( NRESWARM )
    PadConfig( NRESWARM, PAD_INPUT_ENABLE );
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
    BYTE faults[2];
    BYTE reg;

    // read the status registers so we know why we faulted
    faults[0] = PmicRead( PMIC_PDSTATUSREG1 );
    faults[1] = PmicRead( PMIC_PDSTATUSREG1 );

    PmicRead( PMIC_FFIQ );
    PmicRead( PMIC_FIRQ );

    PmicRead( PMIC_CHG_INT );
    PmicRead( PMIC_IRQ_FLASH1 );
    PmicRead( PMIC_IRQ_FLASH2 );
    PmicRead( PMIC_EIF3 );
    PmicRead( PMIC_EIF4 );
    PmicRead( PMIC_EFIF3 );
    PmicRead( PMIC_EFIF4 );
    PmicRead( PMIC_INTFLAGSYSIRQ );
    PmicRead( PMIC_INTFLAGSYSFIQ );
    PmicRead( PMIC_BUCK_IRQ );
    PmicRead( PMIC_L01_08_LVL2_FIQ );
    PmicRead( PMIC_L09_16_LVL2_FIQ );
    PmicRead( PMIC_L17_23_LVL2_FIQ );
    PmicRead( PMIC_L01_08_LVL2_IRQ );
    PmicRead( PMIC_L09_16_LVL2_IRQ );
    PmicRead( PMIC_L17_23_LVL2_IRQ );
    PmicRead( PMIC_LEDBST_IRQ );
    PmicRead( PMIC_RGBLED_IRQ_ST );

	// turn off all supplies and reset
    for( reg=PMIC_VOUTCONFIG1B1 ; reg<=PMIC_L23_CNFG1 ; reg++ ) {
    	if( (reg <= PMIC_VOUTCONFIG1B5) || (reg >= PMIC_L01_CNFG1) ) {
    		if( reg == PMIC_LDO_RSVD ) {
    			// reserve supply voltage
    		} else if( reg <= PMIC_VOUTCONFIG1B5 ) {
    			// SM supply voltage
    			PmicClear( reg, BUCK_PWR_MD_NORMAL );
    		} else {
    			// LDO supply voltage
       		    if( PMIC_L23_CNFG1 == reg )
                    PmicClear( reg, (1 << 5) );
                else
    			    PmicClear( reg, LDO_PWR_MD_NORMAL );
    		}
    	}
    }

    // print out the fault reasons in the log
    EventLogAddN( EVENTLOG_RESET_PMICFAULT, faults[0] );
    EventLogAddN( EVENTLOG_RESET_PMICFAULT, faults[1] );

    RimInitiateReset( RESET_COOKIE_INITIATED_PMICFAULT );

}

static void PmicHPDHandler(void)
{
    EventLogAdd( EVENTLOG_PMIC_HPD );
    /* For now do nothing and just let it fall through.
     * The root handler will notify the host if the host
     * is interested in the HPD event.
     */
}

#define L17_23_LVL2_LD023_HDP   ( 1 << 7 )

static void PmicLDOCFiqHandler( void )
{
    BYTE flag17_23;

    // Clear all LDO interrupts
    PmicRead( PMIC_L01_08_LVL2_FIQ );
    PmicRead( PMIC_L09_16_LVL2_FIQ );
    flag17_23 = PmicRead( PMIC_L17_23_LVL2_FIQ );

    if( flag17_23 & L17_23_LVL2_LD023_HDP )
        PmicHPDHandler();
    else
        PmicFaultHandler();
}

static void PmicLDOCIrqHandler( void )
{
    BYTE flag17_23;

    // Clear all LDO interrupts
    PmicRead( PMIC_L01_08_LVL2_IRQ );
    PmicRead( PMIC_L09_16_LVL2_IRQ );
    flag17_23 = PmicRead( PMIC_L17_23_LVL2_IRQ );

    if( flag17_23 & L17_23_LVL2_LD023_HDP )
        PmicHPDHandler();
    else
        PmicFaultHandler();
}
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

static void HostCtrlStateInit( void )
{
    BYTE i;

#if defined( RIM_MAX8649 ) || defined( RIM_TPS62361 )
    //TODO: Verify this is right... don't reset the DDR supply output incase this is a warm reset
    //#if defined( V1_2DIG_SW_EN )
    //PadConfig( V1_2DIG_SW_EN, PAD_OUTPUT_LOW );
    //#endif
    #if defined( VCC_MPU_SW_EN )
    PadConfig( VCC_MPU_SW_EN, PAD_OUTPUT_LOW );
    #endif
#endif

#if defined( V1_8DIG_SW_EN )
    PadConfig( V1_8DIG_SW_EN, PAD_OUTPUT_LOW );
#endif

    HostCtrlDisableSupplies( HostCtrlGetResetFlags() );
    HostCtrlGpioReset( HostCtrlGetResetFlags() );

#if defined( PROCESSOR_PMU430 )
    // configure PG Fault handling
    // NOTE: all PG Fault interrupts use the same handler
    PmicRegisterIntHandler( INT_PWRGD_FLT_LDO0, PmicFaultHandler );
    PmicRegisterIntHandler( INT_LOWSYS, PmicFaultHandler );
#if !defined( DISABLE_PG_FAULTS )
    // unmask all the PG Faults
    for( i=INT_PWRGD_FLT_LDO0 ; i<=INT_PWRGD_FLT_SM3 ; i++ ) {
        // except LDO5 (USBLDO). Don't trip a PG fault with it
        if( i != INT_PWRGD_FLT_LDO5 ) {
            PmicInterruptEnable( (PmicInterrupt)i );
        }
    }
    PmicInterruptEnable( INT_LOWSYS );
#endif
#else
#if defined( PROCESSOR_MQ31 )
    // configure PG Fault handling
    // NOTE: all PG Fault interrupts use the same handler
    for( i = INT_GPIO_IRQ ; i <= INT_SYSMON_FIQ ; i++ ) {
    	switch( i ) {
    		case INT_GPIO_IRQ:
    		case INT_LIGHT_IRQ:
    		case INT_CHARGER_IRQ:
    		case INT_GPIO_FIQ:
    		case INT_RSVD_FIQ:
    		case INT_CHARGER_FIQ:
    			break;

            case INT_LDO_C_FIQ:
                PmicRegisterIntHandler( (PmicInterrupt)i, PmicLDOCFiqHandler );
    	        PmicInterruptEnable( (PmicInterrupt)i );
                break;
            case INT_LDO_C_IRQ:
                PmicRegisterIntHandler( (PmicInterrupt)i, PmicLDOCIrqHandler );
    	        PmicInterruptEnable( (PmicInterrupt)i );
                break;

    		default:
    	        PmicRegisterIntHandler( (PmicInterrupt)i, PmicFaultHandler );
    	        PmicInterruptEnable( (PmicInterrupt)i );
    			break;
    	}
    }

#if !defined( DISABLE_PG_FAULTS )
    // unmask all the PG Faults except LDO9 (USBLDO)
	PmicWrite( PMIC_INTENSYS, 0x07u );
	PmicWrite( PMIC_BUCK_IRQ_MASK, ~0xF8u );
	PmicWrite( PMIC_L01_08_MASK, ~0xFFu );
	PmicWrite( PMIC_L09_16_MASK, ~0xFEu );
	PmicWrite( PMIC_L17_23_MASK, ~0xFFu );
#endif
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

#if defined( PROCESSOR_PMU430 )
    // if Apps exist its safe to call their entrypoint/init routine
    if( AppCallsAllowed ) {
        RocketMetrics.AppImage->entrypoint();
    }
#else
#if defined( PROCESSOR_MQ31 )
    // if Apps exist its safe to call their entrypoint/init routine
    if( AppCallsAllowed ) {
          AppEntrypoint();
    }
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

    State = HOSTCTRL_STATE_NORTCN;

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
            // -- fallthrough

        case HOSTCTRL_STATE_NORTCN:

            State = HOSTCTRL_STATE_VCCMAIN;
            // -- fallthrough

        case HOSTCTRL_STATE_VCCMAIN:

            // run the rest of the supply sequencing
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


            // setup to catch SYS_EN going low from host
            PadConfig( NRESWARM, PAD_INTERRUPT_CLEAR );
            PadConfig( NRESWARM, PAD_INTERRUPT_FALLING );

            I2cRomInit();                                 //AS+ Configure I2C interface for OMAP4 ROM sequence and starts execution of I2C interrupt routine

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

#if !defined( PLATFORM_RIMPANDA )
                    if( AdcRawConvertVsys() > VSYS_ADC_THRESHOLD ) {
                        break;
                    }
#else
                    break;
#endif
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
                PmicClearSet( PMIC_CHG_INT_MASK, CHGINT_MASK_CHGINA | CHGINT_MASK_CHGINB, EXT_PWR_AVAILABLE );
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
#if defined( NRESWARM )
    PadConfig( NRESWARM, PAD_INTERRUPT_DISABLE );
    PadConfig( NRESWARM, PAD_INTERRUPT_CLEAR );
#endif
}

void HostCtrlInitProc( void )
{
    RegisterTimerExpiry( HOSTCTRL_TIMER, HostCtrlTimerHandler, 0 );

#if defined( NRESWARM )
    PadRegisterIntHandler( NRESWARM, nreswarmIsr );
#endif


    State = HOSTCTRL_STATE_WAIT_VSYS;
    HostCtrlTimerExpiry( 0 );
}
