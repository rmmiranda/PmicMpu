/*****************************************************************************
 * Filename:    pmic_intctrl.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     January 25, 2011
 *
 * Description: PMIC interrupt controller interface allowing PMIC and host
 * 				to share access.
 ****************************************************************************/
#ifndef __PMIC_INTCTRL_H__
#define __PMIC_INTCTRL_H__

typedef enum {
#if defined( PROCESSOR_PMU430 )
    INT_RGB_ACTIVE,
    INT_ABST_OV,
    INT_MBST_OV,
    INT_FLASH_FLT,
    INT_PWRGD_FLT_LDO0,
    INT_PWRGD_FLT_LDO1,
    INT_PWRGD_FLT_LDO2,
    INT_PWRGD_FLT_LDO3,
    INT_PWRGD_FLT_LDO4,
    INT_PWRGD_FLT_LDO5,
    INT_PWRGD_FLT_LDO6,
    INT_PWRGD_FLT_LDO7,
    INT_PWRGD_FLT_LDO8,
    INT_PWRGD_FLT_LDO9,
    INT_PWRGD_FLT_LDO10,
    INT_PWRGD_FLT_LDO11,
    INT_PWRGD_FLT_LDO12,
    INT_PWRGD_FLT_SM0,
    INT_PWRGD_FLT_SM1,
    INT_PWRGD_FLT_SM2,
    INT_PWRGD_FLT_SM3,
    INT_OVR_TEMP,
    INT_BAT_TEMP,
    INT_CHG_TERM,
    INT_TCHG_FLT,
    INT_TPRECHG_FLT,
    INT_LOWSYS,
    INT_REDUCE_CHG_THERMAL_DPPM,
    INT_PWR_OVP_AC_USB,
    INT_BAT_OVP,
    INT_SYS_CHANGE,
    INT_PWR_CHANGE_ACFET_USBFET,
    INT_AC_DET,
    INT_BAT_DET,
    INT_USB_DET,
    INT_CHG_WD,
    INT_FLSH_TREG,
    INT_FLSH_TWARN,
    INT_FLSH_STO
#elif defined( PROCESSOR_MQ31 )
    INT_GPIO_IRQ,
    INT_LIGHT_IRQ,
    INT_LDO_A_IRQ,
    INT_LDO_B_IRQ,
    INT_LDO_C_IRQ,
    INT_BUCK_IRQ,
    INT_CHARGER_IRQ,
    INT_SYSMON_IRQ,
    INT_GPIO_FIQ,
    INT_RSVD_FIQ,
    INT_LDO_A_FIQ,
    INT_LDO_B_FIQ,
    INT_LDO_C_FIQ,
    INT_BUCK_FIQ,
    INT_CHARGER_FIQ,
    INT_SYSMON_FIQ,
#else
#error PMIC interrupt module not defined for this processor
#endif
} PmicInterrupt;

/**
 * Enables the specified interrupt for the pmic core
 */
void PmicInterruptEnable( PmicInterrupt sig );

/**
 * Disables the specified interrupt for the pmic core
 */
void PmicInterruptDisable( PmicInterrupt sig );

/**
 * Registers a pmic core handler for the the specified interrupt
 */
void PmicRegisterIntHandler( PmicInterrupt sig, FUNCTION function );

/**
 * Init PMIC Interrupt controller interface
 */
void PmicIntCtrlInit( void );

/**
 * Uninit PMIC Interrupt controller interface
 */
void PmicIntCtrlHostUninit( void );

/**
 * Lagavulin API write interface (for i2c control/access of PMIC interrupt control registers)
 */
void PmicIntctrlRegWrite( BYTE reg, BYTE value );

/**
 * Lagavulin API read interface (for i2c control/access of PMIC interrupt control registers)
 */
BYTE PmicIntctrlRegRead( BYTE reg );


#define INTCTRL_UPDATE_REQ      ( 1u << 0 )
#define INTCTRL_UPDATE_QUEUED   ( 1u << 1 )
extern BYTE IntctrlUpdateStat;

#endif // __PMIC_INTCTRL_H__

