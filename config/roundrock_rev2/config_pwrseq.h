#ifndef _CONFIG_PWRSEQ_H_
#define _CONFIG_PWRSEQ_H_

#include "hostctrl.h"

#define PLATFORM_SUPPLY_SEQUENCE                               \
	{ 0,   PMIC_EN_CMD0, CMD0_EN_SM2   , 0 },                  \
	{ 41,  PMIC_EN_CMD2, CMD2_EN_LDO10 , 0 },                  \
	{ 41,  PMIC_EN_CMD1, CMD1_EN_LDO6  , 0 },                  \
	{ 82,  GPIO_SEQ_EN,  CORE_SW_EN    , 0 },                  \
	{ 82,  PMIC_EN_CMD1, CMD1_EN_LDO1 | CMD1_EN_LDO4 , 0 },    \
	{ 123, PMIC_EN_CMD1, CMD1_EN_LDO5  , 0 }                   \


// during a hard reset, these supply enables will NOT be disabled
#define CMD0_HARD_RESET_MASK  ( 0 )
#define CMD1_HARD_RESET_MASK  ( 0 )
#define CMD2_HARD_RESET_MASK  ( 0 )

// during a warm reset, these supply enables will NOT be disabled
#define CMD0_WARM_RESET_MASK  ( CMD0_EN_SM2 | CMD0_EN_SM3 )
#define CMD1_WARM_RESET_MASK  ( CMD1_EN_LDO1 | CMD1_EN_LDO4 | CMD1_EN_LDO5 | CMD1_EN_LDO6 )
#define CMD2_WARM_RESET_MASK  ( CMD2_EN_LDO10 )

// LDO_TYPE/DSG_EN/ECO_EN , LDO_NUM, INIT_VALUE
#define SM0_STARTS_SETTING ( LDOSM_DCHG_EN | 0x30 )    // 1.2V
#define SM1_STARTS_SETTING ( LDOSM_DCHG_EN | 0x24 )    // 1.8V
#define SM2_STARTS_SETTING ( LDOSM_DCHG_EN | 0x30 )    // 1.8V
#define SM3_STARTS_SETTING ( LDOSM_DCHG_EN | 0x3D )    // 3.1V

#define LDO0_STARTS_SETTING ( LDOSM_DCHG_EN | 0x24 )    // 1.8V
#define LDO1_STARTS_SETTING ( LDOSM_DCHG_EN | 0x10 )    // 1.2V
#define LDO2_STARTS_SETTING ( LDOSM_DCHG_EN | 0x32 )    // 2.5V
#define LDO3_STARTS_SETTING ( LDOSM_DCHG_EN | 0x24 )    // 1.8V
#define LDO4_STARTS_SETTING ( LDOSM_DCHG_EN | 0x24 )    // 1.8V
#define LDO5_STARTS_SETTING ( LDOSM_DCHG_EN | 0x39 )    // 2.85V
#define LDO6_STARTS_SETTING ( LDOSM_DCHG_EN | 0x3D )    // 3.1V
#define LDO7_STARTS_SETTING ( LDOSM_DCHG_EN | 0x39 )    // 2.85V
#define LDO8_STARTS_SETTING ( LDOSM_DCHG_EN | 0x00 )    // 
#define LDO9_STARTS_SETTING ( LDOSM_DCHG_EN | 0x39 )    // 2.85V
#define LDO10_STARTS_SETTING ( LDOSM_DCHG_EN | 0x15 )    // 2.85V
#define LDO11_STARTS_SETTING ( LDOSM_DCHG_EN | 0x00 )    //
#define LDO12_STARTS_SETTING ( LDOSM_DCHG_EN | 0x00 )    //

#endif /*_CONFIG_PWRSEQ_H_*/
