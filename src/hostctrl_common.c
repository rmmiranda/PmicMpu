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
#include "rocket_pmic.h"
#include "rocket_metrics.h"
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
#include "adc.h"


#if defined( PROCESSOR_PMU430 )
#define T_OVERHEAD_TICKS    6
// this list MUST be in the same order  from registers VSM3 to VLDO12
static const BYTE SLR_DFLT[] = {
    SM3_STARTS_SETTING,     // VSM3
    SM2_STARTS_SETTING,     // VSM2
    0x00,                   // TSM2
    SM1_STARTS_SETTING,     // VSM1
    SM0_STARTS_SETTING,     // VSM0
    0x00,                   // TSM0
    0x00,                   // BUCKCTRL
    0x00,                   // DIGPWM
    LDO0_STARTS_SETTING,    // VLDO0
    LDO1_STARTS_SETTING,    // VLDO1
    LDO2_STARTS_SETTING,    // VLDO2
    LDO3_STARTS_SETTING,    // VLDO3
    LDO4_STARTS_SETTING,    // VLDO4
    LDO5_STARTS_SETTING,    // VLDO5
    LDO6_STARTS_SETTING,    // VLDO6
    LDO7_STARTS_SETTING,    // VLDO7
    LDO8_STARTS_SETTING,    // VLDO8
    LDO9_STARTS_SETTING,    // VLDO9
    LDO10_STARTS_SETTING,   // VLDO10
    LDO11_STARTS_SETTING,   // VLDO11
    LDO12_STARTS_SETTING,   // VLDO12
};
#elif defined( PROCESSOR_MQ31 )
#define T_OVERHEAD_TICKS    12
// this list MUST be in the same order  from registers PMIC_VOUT1 to PMIC_VOUT5
static const BYTE SM_DFLT[] = {
	BUCK01_STARTS_SETTING,  // VSM1
	BUCK03_STARTS_SETTING,  // VSM3
	BUCK02_STARTS_SETTING,  // VSM2
	BUCK04_STARTS_SETTING,  // VSM4
	BUCK05_STARTS_SETTING,  // VSM5
};
// this list MUST be in the same order  from registers PMIC_L01_CNFG1 to PMIC_L23_CNFG1
static const BYTE LDO_DFLT[] = {
    LDO01_STARTS_SETTING,   // VLDO01
    LDO02_STARTS_SETTING,   // VLDO02
    LDO03_STARTS_SETTING,   // VLDO03
    LDO04_STARTS_SETTING,   // VLDO04
    LDO05_STARTS_SETTING,   // VLDO05
    LDO06_STARTS_SETTING,   // VLDO06
    LDO07_STARTS_SETTING,   // VLDO07
    LDO08_STARTS_SETTING,   // VLDO08
    LDO09_STARTS_SETTING,   // VLDO09
    LDO10_STARTS_SETTING,   // VLDO10
    LDO11_STARTS_SETTING,   // VLDO11
    LDO12_STARTS_SETTING,   // VLDO12
    LDO13_STARTS_SETTING,   // VLDO13
    LDO14_STARTS_SETTING,   // VLDO14
    LDO15_STARTS_SETTING,   // VLDO15
    LDO16_STARTS_SETTING,   // VLDO16
    0x00,					// reserved
    LDO17_STARTS_SETTING,   // VLDO17
    LDO18_STARTS_SETTING,   // VLDO18
    LDO19_STARTS_SETTING,   // VLDO19
    LDO20_STARTS_SETTING,   // VLDO20
    LDO21_STARTS_SETTING,   // VLDO21
    LDO22_STARTS_SETTING,   // VLDO22
    LDO23_STARTS_SETTING,   // VLDO23
};
#else
#error Common host controller module not defined for this processor
#endif

static const struct {
    WORD ticks;     // rtc ticks to wait before enabling/disabling the LDO
    BYTE enReg;
    BYTE enMask;
    BYTE seqFlags;   // bitfield describing what exists on the supply being sequenced
} SSEQ[] = {
    PLATFORM_SUPPLY_SEQUENCE
};
static const BYTE SEQUENCE_SIZE = sizeof( SSEQ ) / sizeof( SSEQ[0] );
static BYTE SeqPos = 0;
static BOOL SeqDir = TRUE;
static BOOL ResetInProgress = FALSE;
static WORD InitiateResetReason;
static WORD InitiateResetFlags;

extern BOOL AppCallsAllowed;

/************************** Internal Functions *******************************/

static void SupplySequenceRunInternal( WORD unused )
{
    if( SeqDir ) {
        while( 1 ) {
            if( SeqPos < SEQUENCE_SIZE ) {
                if( !( SSEQ[SeqPos].seqFlags & SEQFLAG_PWRDN ) ) {
                    if( SSEQ[SeqPos].enReg == GPIO_SEQ_EN ) {
                        PadConfig( SSEQ[SeqPos].enMask, PAD_OUTPUT_HIGH );
                    } else if( SSEQ[SeqPos].enReg == GPIO_SEQ_ALT ) {
                        PadConfig( SSEQ[SeqPos].enMask, PAD_OUTPUT_LOW );
                        PadConfig( SSEQ[SeqPos].enMask, PAD_ALT_MODE );
                    } else if( SSEQ[SeqPos].enReg == SEQ_NOEVENT ) {
                        // do nothing
#if defined( PROCESSOR_MQ31 )
                    } else if( SSEQ[SeqPos].enReg == RTCOUT_SEQ_EN ) {
                      	WriteRegWordOr( RTCCFG_REG16, SSEQ[SeqPos].enMask );
#endif
                    } else {
                        PmicSet( SSEQ[SeqPos].enReg, SSEQ[SeqPos].enMask );
                    }
                }

                SeqPos++;
            }
            // is there more sequence to run?
            if( SeqPos < SEQUENCE_SIZE ) {
                // if the next sequence isn't at this tick in time, setup a timer
                if( SSEQ[SeqPos].ticks != SSEQ[SeqPos-1].ticks ) {
                    SetTimer( SUPPLY_SEQUENCE_TIMER, (SSEQ[SeqPos].ticks - SSEQ[SeqPos-1].ticks)-T_OVERHEAD_TICKS );
                    // break out since we don't have more to do for this tick
                    break;
                }
            } else {
                SeqPos = SEQUENCE_SIZE-1;
                // sequence complete
                HostCtrlUpdateState( HOSTCTRL_STATE_POWERED );
                QueueCall( HostCtrlTimerExpiry, 0 );
                break;
            }
        }

    } else {
        while( 1 ) {
            if( SeqPos < SEQUENCE_SIZE ) {
                if( !( SSEQ[SeqPos].seqFlags & SEQFLAG_PWRUP ) ) {
                    if( SSEQ[SeqPos].enReg == GPIO_SEQ_EN ) {
                        PadConfig( SSEQ[SeqPos].enMask, PAD_OUTPUT_LOW );
                    } else if( SSEQ[SeqPos].enReg == GPIO_SEQ_ALT ) {
                        PadConfig( SSEQ[SeqPos].enMask, PAD_OUTPUT_LOW );
                    } else if( SSEQ[SeqPos].enReg == SEQ_NOEVENT ) {
                        // do nothing
#if defined( PROCESSOR_MQ31 )
                    } else if( SSEQ[SeqPos].enReg == RTCOUT_SEQ_EN ) {
                      	WriteRegWordAnd( RTCCFG_REG16, ~(SSEQ[SeqPos].enMask) );
#endif
                    } else {
                        if( (SSEQ[SeqPos].seqFlags & SEQFLAG_DDR) ) {
                           if( InitiateResetFlags & RESET_DDR_SUPPLY ) {
                                PmicClear( SSEQ[SeqPos].enReg, SSEQ[SeqPos].enMask );
                           }
                        } else {
                            PmicClear( SSEQ[SeqPos].enReg, SSEQ[SeqPos].enMask );
                        }
                    }
                }
                SeqPos--;
            }

            // is there more sequence to run?
            if( SeqPos < SEQUENCE_SIZE ) {
                // if the next sequence isn't at this tick in time, setup a timer
                if( SSEQ[SeqPos].ticks != SSEQ[SeqPos+1].ticks ) {
                        SetTimer( SUPPLY_SEQUENCE_TIMER, (SSEQ[SeqPos+1].ticks - SSEQ[SeqPos].ticks)-T_OVERHEAD_TICKS );
                        // break out since we don't have more to do for this tick
                        break;
                    }
            } else {
                    SeqPos = 0;
                    // sequence complete
                    HostCtrlUpdateState( HOSTCTRL_STATE_DISCHARGE );
                    QueueCall( HostCtrlTimerExpiry, 0 );
                    break;
            }
        }
    }
}

/************************** External Functions *******************************/

/******************************************************************************
 * HostCtrlDisableSupplies()
 *
 * Disable PMIC supplies. DDR remains powered unless RESET_DDR_SUPPLY is
 * passed in.
 *****************************************************************************/
#if defined( PROCESSOR_PMU430 )
void HostCtrlDisableSupplies( WORD rstFlags )
{
    if( rstFlags & RESET_DDR_SUPPLY ) {
        PmicWrite( PMIC_EN_CMD0, 0 );
        PmicWrite( PMIC_EN_CMD1, 0 );
        PmicWrite( PMIC_EN_CMD2, 0 );
    } else {
        PmicClear( PMIC_EN_CMD0, ~CMD0_HARD_RESET_MASK );
        PmicClear( PMIC_EN_CMD1, ~CMD1_HARD_RESET_MASK );
        PmicClear( PMIC_EN_CMD2, ~CMD2_HARD_RESET_MASK );
    }
}
#else
#if defined( PROCESSOR_MQ31 )
void HostCtrlDisableSupplies( WORD rstFlags )
{
	BYTE reg;
	BYTE index;

    if( rstFlags & RESET_DDR_SUPPLY ) {
        // disable all supply voltages
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
    } else {
    	const BYTE mask[] = { PLATFORM_HARD_RESET_MASK_LIST };
    	const BYTE size = sizeof(mask) / sizeof(mask[0]);
        // disable only supply voltages which are not on the mask list
        for( reg=PMIC_VOUTCONFIG1B1 ; reg<=PMIC_L23_CNFG1 ; reg++ ) {
        	if( (reg <= PMIC_VOUTCONFIG1B5) || (reg >= PMIC_L01_CNFG1) ) {
        		// search if supply voltage is on the mask list
            	for( index=0; index<size; index++ ) {
            		if( reg == mask[index] ) {
            			break;
            		}
            	}
            	if( index == size) {
            		// supply voltage is not on the mask list
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
        }
    }
}
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

/******************************************************************************
 * SupplySequenceRun()
 *
 * sequencePos: position to start sequence from or SUPSEQ_POS_UNCHANGED
 * sequenceDir: direction of sequence SUPSEQ_DIR_PWRUP/SUPSEQ_DIR_PWRDN
 *
 * Triggers supply sequence to begin
 *****************************************************************************/
void SupplySequenceRun( BYTE sequencePos, BOOL sequenceDir )
{
    if( SUPSEQ_POS_UNCHANGED != 0xFF ) {
        SeqPos = sequencePos;
    }
    SeqDir = sequenceDir;
    SupplySequenceRunInternal( 0 );
}

/******************************************************************************
 * HostCtrlInit()
 *
 * Init host control
 *****************************************************************************/
void HostCtrlInit( void )
{
    BYTE data;

    RegisterTimerExpiry( SUPPLY_SEQUENCE_TIMER, SupplySequenceRunInternal, 0 );

    InitiateResetFlags = 0;

#if defined( PROCESSOR_PMU430 )
    // ADC expects RTC voltage to be 2.5V
    data = PmicRead( PMIC_VRTC ) & ~0x03;
    PmicWrite( PMIC_VRTC, data | 0x01 );
#else
#if defined( PROCESSOR_MQ31 )
    // ADC expects RTC voltage to be 2.5V
    PmicWrite( PMIC_VRTCREG, 0x07 );
    PmicWrite( PMIC_OUTBBATTOFFTH, 0x05 );
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430


    // Processor Specific Init and kicks off power up sequencing
    HostCtrlInitProc();
}


/******************************************************************************
 * HostCtrlPmicRegisterReset()
 *
 * Restore pmic registers to their default state
 *****************************************************************************/
#if defined( PROCESSOR_PMU430 )
void HostCtrlPmicRegisterReset( WORD rstFlags )
{
    BYTE reg;
    BYTE val = 0;
    BOOL resetLcd = (rstFlags & RESET_LCD_IO) ? TRUE:FALSE;

    for( reg = PMIC_MBSTCTRL0 ; reg<=PMIC_FLASHCTRL3 ; reg++ ) {

        // Skip initing the MBST registers in the fast reset case
        if( (reg == PMIC_MBSTCTRL0) && !resetLcd ) {
            reg = PMIC_MBSTCTRL2;
            continue;
        // skip initing the keypad matrix backlight in the fast reset case
        } else if( (reg == PMIC_PWMCOL3) && !resetLcd ) {
            reg = PMIC_MATRIXCTRL;
            continue;
        } else if( reg == PMIC_CTRL_PPCHG0 ) {
#if defined( USB_500mA_REQUIRED )
            // default to ILIM_USB: 0b01 (500mA)  and PPSEL 0b11 (AC/USB/BAT)
            val = 0x70;
#else
            // default to ILIM_USB: 0b00 (100mA)  and PPSEL 0b11 (AC/USB/BAT)
            val = 0x30;
#endif
        } else if( reg == PMIC_CTRL_PPCHG2 ) {
            // default to VLOWSYS: 3.05V  and VCHG: 3.95V
            val = 0x30;
        } else {
            val = 0;
        }

        PmicWrite( reg, val );
    }

    // configure supply voltage defaults
    val = 0;
    for( reg=PMIC_VSM3 ; reg<=PMIC_VLDO12 ; reg++ ) {
        PmicWrite( reg, SLR_DFLT[ val++ ] );
    }

    // disable LDO current monitoring
    PmicWrite( PMIC_LDO_MON, 0x0F );

#if defined (PLATFORM_RIMBOLDR016) || defined (PLATFORM_RIMCURVER032)
   // turn on FG LDO, CL_SEL 7.5pF
    PmicClearSet( PMIC_VRTC, VRTC_CLSEL_MASK, VRTC_FGLDO_EN | VRTC_CLSEL_7P5 );
#else
    // turn on FG LDO, CL_SEL 6.5pF
    PmicClearSet( PMIC_VRTC, VRTC_CLSEL_MASK, VRTC_FGLDO_EN | VRTC_CLSEL_6P5 );
#endif

    // enable the PG 1ms deglitch
    PmicSet( PMIC_EN_CMD0, 1<<6 );

    // only set the INTMASK defaults on a hard reset
    if( (rstFlags & RESET_HOST_WARM) == 0 ) {
        for( reg = PMIC_INTMASK0 ; reg <= PMIC_INTMASK5 ; reg++ ) {
            PmicWrite( reg, 0xFF );
        }
    }

    // unlock OPTIONS register for a single write
    PmicWrite( PMIC_OPTIONS_UNLOCK, 0x88 );
    // Rts: 100k (NTC 47k)  ChargerWatchdog: enabled   THOTSEL: 50'C
    PmicWrite( PMIC_OPTIONS, (1<<6) | (1<<4) );
}
#else
#if defined( PROCESSOR_MQ31 )
void HostCtrlPmicRegisterReset( WORD rstFlags )
{
    BYTE reg;
    BYTE val = 0;
    BOOL resetLcd = (rstFlags & RESET_LCD_IO) ? TRUE:FALSE;

    if( resetLcd ) {

        // init the WLED boost converter
    	for( reg = PMIC_LEDBST_CNTRL_1; reg <= PMIC_LED_CURRENT_32; reg++ ) {
    		PmicWrite( reg, 0x00 );
    	}

        // init the keypad matrix backlight
    	for( reg = PMIC_KEYLED0; reg <= PMIC_KEY_CONFIG; reg++ ) {
    		PmicWrite( reg, 0x00 );
    	}

    }

    // unlock write protect access to charger registers
    PmicWrite( PMIC_CHG_CNFG_06, 0x0C );

    // default to MINVSYS: 3.1V  and CHG_CV_PRM: 4.2V
    PmicWrite( PMIC_CHG_CNFG_04, 0x37 );

    // Set AC current limit to 500mA
    PmicWrite( PMIC_CHG_CNFG_09, 0x0F );

#if defined( USB_500mA_REQUIRED )
    // Set USB current limit to 500mA
    PmicWrite( PMIC_CHG_CNFG_10, 0x19 );
#else
    // Set USB current limit to 100mA
    PmicWrite( PMIC_CHG_CNFG_10, 0x05 );
#endif

    // Make USB and AC as valid charger inputs
    PmicSet( PMIC_CHG_CNFG_12, 0x60 );

    // lock write protect access to charger registers
    PmicClear( PMIC_CHG_CNFG_06, (0x3u << 2) );

    // configure SM supply voltage defaults
    for( val=0,reg=PMIC_VOUT1 ; reg<=PMIC_VOUT5 ; reg++ ) {
        PmicWrite( reg, SM_DFLT[ val++ ] );
    }

    // configure LDO supply voltage defaults
    for( val=0,reg=PMIC_L01_CNFG1 ; reg<=PMIC_L23_CNFG1 ; reg++ ) {
        PmicWrite( reg, LDO_DFLT[ val++ ] );
    }

    // only set the INTMASK defaults on a hard reset
    if( (rstFlags & RESET_HOST_WARM) == 0 ) {
    	PmicWrite( PMIC_MFIQ, 0xFF );
    	PmicWrite( PMIC_MIRQ, 0xFF );
    	PmicWrite( PMIC_CHG_INT_MASK, 0xFF );
    	PmicWrite( PMIC_IRQ_FLASH1_MASK, 0xFF );
    	PmicWrite( PMIC_IRQ_FLASH2_MASK, 0xFF );
    	PmicWrite( PMIC_EIE3, 0x00 );
    	PmicWrite( PMIC_EIE4, 0x00 );
    	PmicWrite( PMIC_INTENSYS, 0x00 );
    	PmicWrite( PMIC_BUCK_IRQ_MASK, 0xFF );
    	PmicWrite( PMIC_L01_08_MASK, 0xFF );
    	PmicWrite( PMIC_L09_16_MASK, 0xFE );
    	PmicWrite( PMIC_L17_23_MASK, 0xFF );
    	PmicWrite( PMIC_LEDBST_IRQ_MASK, 0xFF );
    }
}
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

/******************************************************************************
 * HostCtrlGpioReset()
 *
 * Reset all GPIOs related to the host/system to their default state depending
 * on the type of reset being performed
 *****************************************************************************/
void HostCtrlGpioReset( WORD rstFlags )
{

    // if we are enterting supercap mode, use special
    // handler to mass configure all IO's at once
    if( rstFlags & RESET_SUPERCAP ) {
        PadSupercapMode();
        return;
    }

#if defined( PMIC_INT_N )
    // interrupt for host from PMIC (to test point on eval board)
    PadConfig( PMIC_INT_N, PAD_OUTPUT_HIGH );
#endif

/************************************************************
 *   Host Processor & Memory Reset Signals
 ************************************************************/

    // reset host processor specific gpio's which are part of the
    // sequencer
    HostCtrlGpioResetProc();

#if defined( RESET_APPS_IN_N )
    PadConfig( RESET_APPS_IN_N, PAD_OUTPUT_LOW );
#endif

#if defined( RESET_APPS_GPIO_N )
    PadConfig( RESET_APPS_GPIO_N, PAD_OUTPUT_LOW );
#endif

#if defined( RESET_COMMS_OUT_N )
    PadConfig( RESET_COMMS_OUT_N, PAD_INPUT_ENABLE );
#endif

#if defined( COMMS_WDT_RESET_N )
    PadConfig( COMMS_WDT_RESET_N, PAD_INPUT_ENABLE );
#endif

#if defined( RST_1V8_N )
 #if !defined( PLATFORM_RIMROUNDROCK )
    // on Kelowna we need to drive the eMMC reset line
    PadConfig( RST_1V8_N, PAD_OUTPUT_LOW );
 #else
    PadConfig( RST_1V8_N, PAD_INPUT_ENABLE );
 #endif
#endif

#if defined( RST_EMMC_N )
    PadConfig( RST_EMMC_N, PAD_OUTPUT_LOW );
#endif

    #if defined( LCD_RST_N )
    if( rstFlags & RESET_LCD_IO ) {
        PadConfig( LCD_RST_N, PAD_OUTPUT_LOW );
        #if defined( RIM_PWM )
        // disable pwm output
        PwmEnable( 0, 0 );
        #endif
    }
    #endif

#if defined( CLK_ON )
    PadConfig( CLK_ON, PAD_INPUT_ENABLE );
#endif

#if defined( COMMS_26M_EN )
    PadConfig( COMMS_26M_EN, PAD_INPUT_PULLDOWN );
#endif

#if defined( APPS_26M_EN )
    PadConfig( APPS_26M_EN, PAD_INPUT_PULLDOWN );
#endif

#if defined( AP_26M_REQ_EN )
    PadConfig( AP_26M_REQ_EN, PAD_INPUT_ENABLE );
#endif

#if defined( CP_26M_REQ_EN )
    PadConfig( CP_26M_REQ_EN, PAD_INPUT_ENABLE );
#endif

#if defined( ESHEL_DVFM0 )
    PadConfig( ESHEL_DVFM0, PAD_INPUT_ENABLE );
#endif

#if defined( ESHEL_DVFM1 )
    PadConfig( ESHEL_DVFM1, PAD_INPUT_ENABLE );
#endif

#if defined( TCXO_VCC_EN_SW )
    PadConfig( TCXO_VCC_EN_SW, PAD_OUTPUT_LOW );
#endif

/************************************************************
 *   LDO/Supply Control Signals
 ************************************************************/
#if defined( AUD_PROG_EN )
    // pull required by default
    PadConfig( AUD_PROG_EN, PAD_INPUT_PULLDOWN );
#endif

#if defined( BWF_26M_EN )
    // pull requierd by default
    PadConfig( BWF_26M_EN, PAD_INPUT_PULLDOWN );
#endif

#if defined( GPS_TCXO_EN )
    // pull required by default
    PadConfig( GPS_TCXO_EN, PAD_INPUT_PULLDOWN );
#endif

#if defined( GPS_WBF_TCXO_EN )
    PadConfig( GPS_WBF_TCXO_EN, PAD_INPUT_PULLDOWN );
#endif

#if defined( RX_ACTIVITY )
    PadConfig( RX_ACTIVITY, PAD_INPUT_ENABLE );
#endif

#if defined( TX_ACTIVITY )
    PadConfig( TX_ACTIVITY, PAD_INPUT_ENABLE );
#endif

#if defined( LS_PWR )
    PadConfig( LS_PWR, PAD_OUTPUT_LOW );
    PadConfig( LS_PWR, PAD_STRENGTH_FULL );
#endif

#if defined( V1_8DIG_PWM )
    PadConfig( V1_8DIG_PWM, PAD_OUTPUT_LOW );
#endif

#if defined( V2_85USB_HP_EN )
    PadConfig( V2_85USB_HP_EN, PAD_OUTPUT_LOW );
#endif

/************************************************************
 *  Radio/RF Control Signals
 ************************************************************/
#if defined( TX_EN )
    PadConfig( TX_EN, PAD_INPUT_ENABLE );
#endif

#if defined( WB_TX_EN )
    PadConfig( WB_TX_EN, PAD_INPUT_ENABLE );
#endif

#if defined( WLAN_LNA_EN )
    PadConfig( WLAN_LNA_EN, PAD_INPUT_ENABLE );
#endif

#if defined( WLAN_PA_EN )
    PadConfig( WLAN_PA_EN, PAD_INPUT_ENABLE );
#endif

#if defined( GPIO_RST_N )
    PadConfig( GPIO_RST_N, PAD_INPUT_ENABLE );
#endif

#if defined( RESET_OUT_N )
    PadConfig( RESET_OUT_N, PAD_INPUT_ENABLE );
#endif

#if defined( RF_ACTIVITY )
    PadConfig( RF_ACTIVITY, PAD_INPUT_ENABLE );
#endif

#if defined( DIGRF_CLK_EN )
    PadConfig( DIGRF_CLK_EN, PAD_INPUT_ENABLE );
#endif

#if defined( GPS_BWF_EN )
    PadConfig( GPS_BWF_EN, PAD_INPUT_ENABLE );
#endif

#if defined( WLAN_PWM_REQ )
    PadConfig( WLAN_PWM_REQ, PAD_INPUT_ENABLE );
#endif

#if defined( TXON_MON_2G )
    PadConfig( TXON_MON_2G, PAD_INPUT_ENABLE );
#endif

#if defined( TXON_MON_3G )
    PadConfig( TXON_MON_3G, PAD_INPUT_ENABLE );
#endif

/************************************************************
 *  Fault Tolerance Control Signals
 ************************************************************/
#if defined( END_KEY )
    PadConfig( END_KEY, PAD_INPUT_ENABLE );
    #if defined( WORKAROUND_END_KEY_PGFAULT ) || defined( END_KEY_PULLUP_ENABLE )
    PadConfig( END_KEY, PAD_INPUT_PULLUP );
    #endif //WORKAROUND_END_KEY_PGFAULT
#endif

#if defined( PWR_FAULT )
    PadConfig( PWR_FAULT, PAD_INPUT_ENABLE );
#endif

#if defined( ADC_REF )
    PadConfig( ADC_REF, PAD_INPUT_ENABLE );
#endif

#if defined( FLT_EN_N )
    PadConfig( FLT_EN_N, PAD_OUTPUT_LOW );
#endif

// Select the Rocket Can Thermistor by default. This requires to set 3 GPIO's
// the one that selects the Thermistor set to LOW and the other two to high
// impedance. To achieve High Impedance, we are setting two GPIO's as INPUT
#if defined( ROCKET_THER_EN )
    PadConfig( ROCKET_THER_EN, PAD_OUTPUT_LOW );
#endif
// We need High Impedance, so set to input
#if defined( FLT_TOL_CAL )
    PadConfig( FLT_TOL_CAL, PAD_INPUT_ENABLE );
#endif
// We need High Impedance, so set to input
#if defined( TAVOR_THER_EN )
    PadConfig( TAVOR_THER_EN, PAD_INPUT_ENABLE );
#endif

/************************************************************
 *  Other Control Signals
 ************************************************************/
#if defined( AUD_1V2_ON )
    PadConfig( AUD_1V2_ON, PAD_INPUT_ENABLE );
#endif

#if defined (TEMP_APP_CORE)
    PadConfig( TEMP_APP_CORE, PAD_INPUT_ENABLE );
#endif

#if defined (TEMP_32KHZ_XTAL)
    PadConfig( TEMP_32KHZ_XTAL, PAD_INPUT_ENABLE );
#endif

/************************************************************
 *   Old/Unused?
 ************************************************************/
#if defined( WAKEUP )
    PadConfig( WAKEUP, PAD_INPUT_ENABLE );
#endif

#if defined( RST_2V8_N )
    PadConfig( RST_2V8_N, PAD_INPUT_ENABLE );
#endif

}

/******************************************************************************
 * HostCtrlGetResetReason()
 *
 * Returns current reason for reset
 *****************************************************************************/
WORD HostCtrlGetResetReason( void )
{
    return( InitiateResetReason );
}

/******************************************************************************
 * HostCtrlGetResetFlags()
 *
 * Returns current flags for any reset overrides/behavior
 *****************************************************************************/
WORD HostCtrlGetResetFlags( void )
{
    return( InitiateResetFlags );
}

/******************************************************************************
 * HostCtrlInitiateReset()
 *
 * Triggers a reset according to the reason and modifies the reset behaviour
 * based on any rstFlags/overrides
 *****************************************************************************/
void HostCtrlInitiateReset( WORD reason, WORD rstFlags )
{
    if( ResetInProgress ) {
        return;
    }
    if( (rstFlags & RESET_HOST_WARM) == 0 ) {
        ResetInProgress = TRUE;

        HostCtrlPrepResetProc();

        if( AppCallsAllowed ) {
            AppExitpoint();
        }

        I2cUninit();

        HostCtrlGpioReset( rstFlags );
    }

    // if we are doing a soft reset we need to capture the log here
    if( (rstFlags & RESET_PMU430_CORE) == 0 ) {
        // trigger the pre-reset log to be captured to our buffer
        // so when the host boots up again we can read it.
        EventLogInit();
        // make sure we update the correct reset value which will be read
        // by the host when it boots again
        UpdateResetStat( reason );
    }

    InitiateResetReason = reason;
    InitiateResetFlags = rstFlags;

    // don't sequence the supplies if its a warm reset, just
    // turn off the ones that should be off, and reset their default voltages
    if( rstFlags & RESET_HOST_WARM ) {
#if defined( PROCESSOR_MQ31 )
    	const BYTE mask[] = { PLATFORM_WARM_RESET_MASK_LIST };
    	const BYTE size = sizeof(mask) / sizeof(mask[0]);
    	BYTE reg;
    	BYTE index;
#endif
        // stop any pending adc and re-init it
        if( AppCallsAllowed ) {
            AdcUninit();
            AdcInit();
        }

        HostCtrlPmicRegisterReset( rstFlags );

#if defined( PROCESSOR_PMU430 )
        PmicClear( PMIC_EN_CMD0, ~CMD0_WARM_RESET_MASK );
        PmicClear( PMIC_EN_CMD1, ~CMD1_WARM_RESET_MASK );
        PmicClear( PMIC_EN_CMD2, ~CMD2_WARM_RESET_MASK );
#else
#if defined( PROCESSOR_MQ31 )
        // disable only supply voltages which are not on the mask list
        for( reg=PMIC_VOUTCONFIG1B1 ; reg<=PMIC_L23_CNFG1 ; reg++ ) {
        	if( (reg <= PMIC_VOUTCONFIG1B5) || (reg >= PMIC_L01_CNFG1) ) {
        		// search if supply voltage is on the mask list
            	for( index=0; index<size; index++ ) {
            		if( reg == mask[index] ) {
            			break;
            		}
            	}
            	if( index == size) {
            		// supply voltage is not on the mask list
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
        }
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430
        ResetInProgress = FALSE;
    } else {
#if defined( PROCESSOR_MQ31 )
        // Disable active discharge on LDO4
        PmicClear( PMIC_L04_CNFG2, (1<<1) );
#endif
        SupplySequenceRun( SUPSEQ_POS_UNCHANGED, SUPSEQ_DIR_PWRDN );
    }

    return;
}

/******************************************************************************
 * HostCtrlInitiateReset()
 *
 * Called when all power sources are removed and we are entering DEEPSLEEP mode
 *****************************************************************************/
void HostCtrlPowerLost( void )
{
    // don't drive the CS IO to the power die to save some power
    PadConfig( PMIC_SPI_CS, PAD_INPUT_ENABLE );

    HostCtrlGpioReset( RESET_SUPERCAP );
}
