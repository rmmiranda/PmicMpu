#ifndef _CONFIG_PWRSEQ_H_
#define _CONFIG_PWRSEQ_H_

#include "hostctrl.h"

#define PLATFORM_SUPPLY_SEQUENCE                               \
	{ MS_TO_RTC_TICKS(0.0), PMIC_EN_CMD0, CMD0_EN_SM1 , SEQFLAG_DDR },          \
	{ MS_TO_RTC_TICKS(0.0), PMIC_EN_CMD1, CMD1_EN_LDO4 , 0 },                   \
	{ MS_TO_RTC_TICKS(1.0), PMIC_EN_CMD0, CMD0_EN_SM2 , 0 },                    \
	{ MS_TO_RTC_TICKS(1.0), PMIC_EN_CMD1, CMD1_EN_LDO0 , 0 },                   \
	{ MS_TO_RTC_TICKS(3.0), PMIC_EN_CMD1, CMD1_EN_LDO5 , 0 },                   \
	{ MS_TO_RTC_TICKS(5.0), PMIC_EN_CMD0, CMD0_EN_SM0  , 0 },                   \
	{ MS_TO_RTC_TICKS(5.0), PMIC_EN_CMD1, CMD1_EN_LDO1 , 0 },                   \
	{ MS_TO_RTC_TICKS(6.0), GPIO_SEQ_EN, CORE_SW_EN  , 0 },                     \
	{ MS_TO_RTC_TICKS(8.0), GPIO_SEQ_EN, RST_1V8_N  , 0 }

// during a hard reset, these supply enables will NOT be disabled
#define CMD0_HARD_RESET_MASK  ( CMD0_EN_SM1 )
#define CMD1_HARD_RESET_MASK  ( 0 )
#define CMD2_HARD_RESET_MASK  ( 0 )

// during a warm reset, these supply enables will NOT be disabled
#define CMD0_WARM_RESET_MASK  ( CMD0_EN_SM0 | CMD0_EN_SM1 | CMD0_EN_SM2 | CMD0_EN_SM3 )
#define CMD1_WARM_RESET_MASK  ( CMD1_EN_LDO0 | CMD1_EN_LDO4 | CMD1_EN_LDO5  )
#define CMD2_WARM_RESET_MASK  ( 0 )

// LDO/BUCK Default Voltage
#define SM0_STARTS_SETTING ( LDOSM_DCHG_EN | SM0_1V2500 )      // SM0
#define SM1_STARTS_SETTING ( LDOSM_DCHG_EN | SM1_1V750 )       // SM1
#define SM2_STARTS_SETTING ( LDOSM_DCHG_EN | SM2_1V800 )       // SM2
#define SM3_STARTS_SETTING ( LDOSM_DCHG_EN | SM3_2V950 )       // SM3

#define LDO0_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO
#define LDO1_STARTS_SETTING ( LDOSM_DCHG_EN | LVLDO_1V200 )    // LVLDO
#define LDO2_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO
#define LDO3_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V500 )    // GPLDO
#define LDO4_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO
#define LDO5_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_3V100 )    // GPLDO
#define LDO6_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO
#define LDO7_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V500 )    // GPLDO
#define LDO8_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO
#define LDO9_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO
#define LDO10_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_2V85 )    // HVLDO
#define LDO11_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_2V70 )    // HVLDO
#define LDO12_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_2V80 )    // HVLDO

// configure MODE voltages for external switcher
#define EXT_SUPPLY_MODE0    (770)
#define EXT_SUPPLY_MODE1    (770)
#define EXT_SUPPLY_MODE2    (1160)
#define EXT_SUPPLY_MODE3    (1350)

// LDOs enable/disable controlled by external triggers (GPIOS)
#define GPIO_PWR_SEQUENCE       1                  // enable the IO control of the following supplies
//                                Ctrl
//                                Reg           EnableMask     GPIO Trigg
// ECO Mode Enable
#define CLK_ON_SUPPLY           { PMIC_EN_CMD0, CMD0_EN_ECO,   CLK_ON }

#define CLK_ON_TOGGLES_LDO11    1

#endif /*_CONFIG_PWRSEQ_H_*/
