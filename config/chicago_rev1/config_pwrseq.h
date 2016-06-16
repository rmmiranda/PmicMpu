#ifndef _CONFIG_PWRSEQ_H_
#define _CONFIG_PWRSEQ_H_

#include "hostctrl.h"

#define PLATFORM_SUPPLY_SEQUENCE                                 \
    { MS_TO_RTC_TICKS(0.0),  SEQ_NOEVENT,  SEQ_NOEVENT,    0 },                    \
    { MS_TO_RTC_TICKS(0.5),  PMIC_EN_CMD0, CMD0_EN_SM1,    0 },                    \
    { MS_TO_RTC_TICKS(1.5),  PMIC_EN_CMD1, CMD1_EN_LDO3,   0 },                    \
    { MS_TO_RTC_TICKS(2.0),  PMIC_EN_CMD0, CMD0_EN_SM2 ,   0 },                    \
    { MS_TO_RTC_TICKS(3.5),  GPIO_SEQ_EN,  V1_2DIG_SW_EN,  SEQFLAG_DDR },          \
    { MS_TO_RTC_TICKS(4.0),  PMIC_EN_CMD0, CMD0_EN_SM0 ,   0 },                    \
    { MS_TO_RTC_TICKS(4.5),  GPIO_SEQ_EN,  VCC_MPU_SW_EN,  0 },                    \
    { MS_TO_RTC_TICKS(5.0),  PMIC_EN_CMD0, CMD0_EN_SM3 ,   0 },                    \
    { MS_TO_RTC_TICKS(5.5),  PMIC_EN_CMD1, CMD1_EN_LDO5,   0 },                    \
    { MS_TO_RTC_TICKS(6.0),  PMIC_EN_CMD2, CMD2_EN_LDO12,  0 },                    \
    { MS_TO_RTC_TICKS(6.5),  GPIO_SEQ_EN,  FLT_EN_N,       0 },                    \
    { MS_TO_RTC_TICKS(8.0),  GPIO_SEQ_ALT, SYS_32K_CLK0,   0 },                    \
    { MS_TO_RTC_TICKS(8.0),  GPIO_SEQ_ALT, SYS_32K_CLK1,   0 },                    \
    { MS_TO_RTC_TICKS(10.0), GPIO_SEQ_EN,  NRESPWRON,      0 }

// during a hard reset, these supply enables will NOT be disabled
#define CMD0_HARD_RESET_MASK  ( 0 )
#define CMD1_HARD_RESET_MASK  ( 0 )
#define CMD2_HARD_RESET_MASK  ( 0 )

// during a warm reset, these supply enables will NOT be disabled
#define CMD0_WARM_RESET_MASK  ( 0 )
#define CMD1_WARM_RESET_MASK  ( 0 )
#define CMD2_WARM_RESET_MASK  ( 0 )

// LDO/BUCK Default Voltage
#define SM0_STARTS_SETTING ( LDOSM_DCHG_EN | SM0_1V2250 )      // SM0       // VCC_CORE
#define SM1_STARTS_SETTING ( LDOSM_DCHG_EN | SM1_2V850 )       // SM1       // V2_85DIG
#define SM2_STARTS_SETTING ( LDOSM_DCHG_EN | SM2_1V800 )       // SM2       // V1_8DIG
#define SM3_STARTS_SETTING ( LDOSM_DCHG_EN | SM3_1V050 )       // SM3       // VCC_IVA

#define LDO0_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_3V100 )    // GPLDO     // V3_1VIB
#define LDO1_STARTS_SETTING ( LDOSM_DCHG_EN | LVLDO_1V200 )    // LVLDO     // NC
#define LDO2_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO     // NC
#define LDO3_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V500 )    // GPLDO     // V2_5SENS
#define LDO4_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO     // NC
#define LDO5_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_3V100 )    // GPLDO     // V3_3USB
#define LDO6_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO     // NC
#define LDO7_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO     // NC
#define LDO8_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO     // NC
#define LDO9_STARTS_SETTING (                 GPLDO_1V700 )    // GPLDO     // V1_7EFUSE
#define LDO10_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_2V80 )    // HVLDO     // V2_8CAM
#define LDO11_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_2V70 )    // HVLDO     // V2_7ANT
#define LDO12_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_1V80 )    // HVLDO     // V1_8AUD

// LDOs enable/disable controlled by external triggers (GPIOS)
#define GPIO_PWR_SEQUENCE       1                  // enable the IO control of the following supplies
//                                Ctrl
#define CP_26M_REQ_EN_SUPPLY      { PMIC_EN_CMD2, CMD2_EN_LDO11,  CP_26M_REQ_EN }

#endif /*_CONFIG_PWRSEQ_H_*/
