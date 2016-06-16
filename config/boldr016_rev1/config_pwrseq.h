#ifndef _CONFIG_PWRSEQ_H_
#define _CONFIG_PWRSEQ_H_

#include "hostctrl.h"

#define PLATFORM_SUPPLY_SEQUENCE                               \
	{ 0,   PMIC_EN_CMD0, CMD0_EN_SM1  , SEQFLAG_DDR },         \
	{ 0,   PMIC_EN_CMD0, CMD0_EN_SM2  , 0 },                   \
	{ 0,   PMIC_EN_CMD1, CMD1_EN_LDO0 , 0 },                   \
	{ 33,  PMIC_EN_CMD1, CMD1_EN_LDO4 , 0 },                   \
	{ 164, PMIC_EN_CMD1, CMD1_EN_LDO5 , 0 },                   \
	{ 213, PMIC_EN_CMD0, CMD0_EN_SM0  , 0 },                   \
	{ 221, GPIO_SEQ_EN,  CORE_SW_EN   , 0 },                   \
	{ 287, GPIO_SEQ_EN,  RST_1V8_N    , 0 }

// during a hard reset, these supply enables will NOT be disabled
#define CMD0_HARD_RESET_MASK  ( CMD0_EN_SM1 )
#define CMD1_HARD_RESET_MASK  ( 0 )
#define CMD2_HARD_RESET_MASK  ( 0 )

// during a warm reset, these supply enables will NOT be disabled
#define CMD0_WARM_RESET_MASK  ( CMD0_EN_SM0 | CMD0_EN_SM1 | CMD0_EN_SM2 | CMD0_EN_SM3 )
#define CMD1_WARM_RESET_MASK  ( CMD1_EN_LDO0 | CMD1_EN_LDO4 | CMD1_EN_LDO5  )
#define CMD2_WARM_RESET_MASK  ( 0 )

// during a warm reset, these supply enables will NOT be disabled
#define CMD0_WARM_RESET_MASK  ( CMD0_EN_SM0 | CMD0_EN_SM1 | CMD0_EN_SM2 | CMD0_EN_SM3 )
#define CMD1_WARM_RESET_MASK  ( CMD1_EN_LDO0 | CMD1_EN_LDO4 | CMD1_EN_LDO5  )
#define CMD2_WARM_RESET_MASK  ( 0 )

// LDO_TYPE/DSG_EN/ECO_EN , LDO_NUM, INIT_VALUE
#define SM0_STARTS_SETTING ( LDOSM_DCHG_EN | 0x30 )    // 1.2V
#define SM1_STARTS_SETTING ( LDOSM_DCHG_EN | 0x23 )    // 1.75V
#define SM2_STARTS_SETTING ( LDOSM_DCHG_EN | 0x30 )    // 1.8V
#define SM3_STARTS_SETTING ( LDOSM_DCHG_EN | 0x3B )    // 2.95V

#define LDO0_STARTS_SETTING ( LDOSM_DCHG_EN | 0x39 )    // 2.85V
#define LDO1_STARTS_SETTING ( LDOSM_DCHG_EN | 0x10 )    // 1.2V
#define LDO2_STARTS_SETTING ( LDOSM_DCHG_EN | 0x39 )    // 2.85V
#define LDO3_STARTS_SETTING ( LDOSM_DCHG_EN | 0x32 )    // 2.5V
#define LDO4_STARTS_SETTING ( LDOSM_DCHG_EN | 0x39 )    // 2.85V
#define LDO5_STARTS_SETTING ( LDOSM_DCHG_EN | 0x3F )    // 3.3V
#define LDO6_STARTS_SETTING ( LDOSM_DCHG_EN | 0x39 )    // 2.85V
#define LDO7_STARTS_SETTING ( LDOSM_DCHG_EN | 0x24 )    // 1.8V
#define LDO8_STARTS_SETTING ( LDOSM_DCHG_EN | 0x39 )    // 2.85V
#define LDO9_STARTS_SETTING ( LDOSM_DCHG_EN | 0x24 )    // 1.8V
#define LDO10_STARTS_SETTING ( LDOSM_DCHG_EN | 0x15 )    // 2.85V
#define LDO11_STARTS_SETTING ( LDOSM_DCHG_EN | 0x15 )    // 2.85V
#define LDO12_STARTS_SETTING ( LDOSM_DCHG_EN | 0x00 )    // 1.8V

// LDOs enable/disable controlled by external triggers (GPIOS)
#define GPIO_PWR_SEQUENCE       1                  // enable the IO control of the following supplies
//                                Ctrl
//                                Reg           EnableMask     GPIO Trigg
// V1_8_GPS
#define GPS_TCXO_EN_SUPPLY      { PMIC_EN_CMD2, CMD2_EN_LDO12, GPS_TCXO_EN }

// ECO Mode Enable
#define CLK_ON_SUPPLY           { PMIC_EN_CMD0, CMD0_EN_ECO,   CLK_ON }


#endif /*_CONFIG_PWRSEQ_H_*/