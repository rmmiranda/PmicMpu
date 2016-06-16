/*****************************************************************************
 * Filename:    hostctrl.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     January 18, 2011
 *
 * Description: Definitions for host control code (copied from
 * 				rocket_hostctrl.h file by Bryon Hummel).
 ****************************************************************************/
#ifndef __HOSTCTRL_H__
#define __HOSTCTRL_H__

#include "watchdog.h"

// bitfield describing what exists on the supply being sequenced
#define SEQFLAG_DDR         (1<<0)
#define SEQFLAG_FLASH       (1<<1)
#define SEQFLAG_LCD         (1<<2)
#define SEQFLAG_PWRUP       (1<<3)  // Entry executed during power-up only
#define SEQFLAG_PWRDN       (1<<4)  // Entry executed during power-down only

// utility macro
#define MS_TO_RTC_TICKS(x)        ((x)*33)

// Bitfield definitions for LDO/BUCK start & sequence settings
#define TYPE_MASK   (3<<0)
#define TYPE_LDO    (0<<0)
#define TYPE_SM     (1<<0)
#define TYPE_RTC    (2<<0)
#define DSG_EN      (1<<2)
#define ECO_EN      (1<<3)
#define FPWM_EN     (1<<4)

#if defined( PROCESSOR_MQ31 )
// indicate the sequence is toggling an RTC out not an buck/ldo
#define RTCOUT_SEQ_EN (0xFB)
#endif
#define GPIO_SEQ_ALT (0xFC)
// placeholder event in sequence that does nothing
#define SEQ_NOEVENT (0xFD)
// indicate the sequence is toggling a GPIO not an buck/ldo
#define GPIO_SEQ_EN (0xFE)
// indicates the switcher for the sequence is not on Rocket and is external
#define MAX8649_SW  (0xFF)

// Available Reset Flags
#define RESET_PMU430_CORE   ( 1<<0 )        // Triggers the PMU430 core to be reset
#define RESET_DDR_SUPPLY    ( 1<<1 )        // Forces the DDR supply to be powered off
#define RESET_LCD_IO        ( 1<<2 )        // Forces the LCD_RST_N IO to be powered down
#define RESET_WAIT_EXTPWR   ( 1<<3 )        // At end of powerdown sequence we wait for external power
                                            // before starting up
#define RESET_SUPERCAP      ( 1<<4 )        // Indicate we are going to enter SUPERCAP mode
#define RESET_HOST_WARM     ( 1<<5 )        // Host had a WARM reset, don't cycle supplies/ios

void HostCtrlInit( void );
void HostCtrlInitiateReset( WORD reason, WORD rstFlags );
void HostCtrlPowerLost( void );
void HostCtrlPmicRegisterReset( WORD rstFlags );

#define SUPSEQ_POS_UNCHANGED    ( 0xFF )
#define SUPSEQ_DIR_PWRUP        ( TRUE )
#define SUPSEQ_DIR_PWRDN        ( FALSE )
void SupplySequenceRun( BYTE sequencePos, BOOL sequenceDir );

WORD HostCtrlGetResetReason( void );
WORD HostCtrlGetResetFlags( void );

typedef enum {
    HOSTCTRL_STATE_INIT,
    HOSTCTRL_STATE_NORTCN,
    HOSTCTRL_STATE_VCCMAIN,
    HOSTCTRL_STATE_SEQUENCING_TIMEDOUT,
    HOSTCTRL_STATE_POWERED,
    HOSTCTRL_STATE_DISCHARGE,
    HOSTCTRL_STATE_PMU430_RESET,
    #if defined( HOSTPROCESSOR_TAVOR_MG )
    HOSTCTRL_STATE_SYSEN_DEBOUNCE,
    #endif
    HOSTCTRL_STATE_WAIT_EXTPWR,
    HOSTCTRL_STATE_WAIT_VSYS,
} HostCtrlState;

void HostCtrlTimerExpiry( WORD data );
void ConfigSupplyVoltageDefaults( void );
void HostCtrlGpioReset( WORD rstFlags );
void HostCtrlDisableSupplies( WORD rstFlags );

void HostCtrlUpdateState( HostCtrlState newState );

void HostCtrlInitProc( void );
void HostCtrlPrepResetProc( void );
void HostCtrlGpioResetProc( void );

#endif
