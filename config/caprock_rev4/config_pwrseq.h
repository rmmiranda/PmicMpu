#ifndef _CONFIG_PWRSEQ_H_
#define _CONFIG_PWRSEQ_H_

#include "hostctrl.h"


// TODO: verify SM2 actually supplies DDR on caprock?
#define PLATFORM_SUPPLY_SEQUENCE                               \
	{ 0,   PMIC_EN_CMD0, CMD0_EN_SM2   , /*SEQFLAG_DDR*/ 0 },  \
	{ 41,  PMIC_EN_CMD2, CMD2_EN_LDO10 , 0 },                  \
	{ 41,  PMIC_EN_CMD1, CMD1_EN_LDO6  , 0 },                  \
	{ 82,  PMIC_EN_CMD0, CMD0_EN_SM0   , 0 },                  \
	{ 82,  PMIC_EN_CMD1, CMD1_EN_LDO1 | CMD1_EN_LDO4 , 0 },    \
	{ 123, PMIC_EN_CMD1, CMD1_EN_LDO5  , 0 }                   \

// during a hard reset, these supply enables will NOT be disabled
#define CMD0_HARD_RESET_MASK  ( /*CMD0_EN_SM2*/ 0 )
#define CMD1_HARD_RESET_MASK  ( 0 )
#define CMD2_HARD_RESET_MASK  ( 0 )


// LDO_TYPE/DSG_EN/ECO_EN , LDO_NUM, INIT_VALUE
#define SM0_STARTS_SETTING { TYPE_SM | DSG_EN, 0, 0x30 }    // 1.8V
#define SM1_STARTS_SETTING { TYPE_SM | DSG_EN, 1, 0x24 }    // 1.2V
#define SM2_STARTS_SETTING { TYPE_SM | DSG_EN, 2, 0x30 }    // 1.8V
#define SM3_STARTS_SETTING { TYPE_SM | DSG_EN, 3, 0x3D }    // 3.1V

#define LDO0_STARTS_SETTING { TYPE_LDO | DSG_EN, 0, 0x24 }    // 1.8V
#define LDO1_STARTS_SETTING { TYPE_LDO | DSG_EN, 1, 0x10 }    // 1.2V
#define LDO2_STARTS_SETTING { TYPE_LDO | DSG_EN, 2, 0x32 }    // 2.5V
#define LDO3_STARTS_SETTING { TYPE_LDO | DSG_EN, 3, 0x24 }    // 1.8V
#define LDO4_STARTS_SETTING { TYPE_LDO | DSG_EN, 4, 0x24 }    // 1.8V
#define LDO5_STARTS_SETTING { TYPE_LDO | DSG_EN, 5, 0x39 }    // 2.85V
#define LDO6_STARTS_SETTING { TYPE_LDO | DSG_EN, 6, 0x3D }    // 3.1V
#define LDO7_STARTS_SETTING { TYPE_LDO | DSG_EN, 7, 0x39 }    // 2.85V
#define LDO8_STARTS_SETTING { TYPE_LDO | DSG_EN, 8, 0x00 }    // 
#define LDO9_STARTS_SETTING { TYPE_LDO | DSG_EN, 9, 0x39 }    // 2.85V
#define LDO10_STARTS_SETTING { TYPE_LDO | DSG_EN, 10, 0x15 }    // 2.85V
#define LDO11_STARTS_SETTING { TYPE_LDO | DSG_EN, 11, 0x00 }    //
#define LDO12_STARTS_SETTING { TYPE_LDO | DSG_EN, 12, 0x00 }    //

#define RTC_STARTS_SETTING { TYPE_RTC, 0, 0x01 }    // 2.5V

#endif /*_CONFIG_PWRSEQ_H_*/
