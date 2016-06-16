#ifndef _CONFIG_PWRSEQ_H_
#define _CONFIG_PWRSEQ_H_

#define PLATFORM_SUPPLY_SEQUENCE                                \
	{   0,  PMIC_L04_CNFG1,     LDO_PWR_MD_NORMAL,  SEQFLAG_PWRDN },\
	{   0,  PMIC_VOUTCONFIG1B4, BUCK_PWR_MD_NORMAL, 0 },        \
	{   0,  PMIC_L22_CNFG1,     LDO_PWR_MD_NORMAL,  0 },        \
	{  33,  PMIC_L07_CNFG1,	    LDO_PWR_MD_NORMAL,  0 },        \
	{  49,  PMIC_VOUTCONFIG1B3, BUCK_PWR_MD_NORMAL, 0 },        \
	{  66,  PMIC_L05_CNFG1,     LDO_PWR_MD_NORMAL,  0 },        \
	{  98,  PMIC_VOUTCONFIG1B5, BUCK_PWR_MD_NORMAL, 0 },\
	{ 115,  PMIC_VOUTCONFIG1B1, BUCK_PWR_MD_NORMAL, 0 },        \
	{ 131,  GPIO_SEQ_EN,        VCC_MPU_SW_EN,      0 },        \
	{ 147,  PMIC_VOUTCONFIG1B2, BUCK_PWR_MD_NORMAL, 0 },        \
	{ 164,  PMIC_L09_CNFG1,     LDO_PWR_MD_NORMAL,  0 },        \
	{ 164,  PMIC_L04_CNFG1,     LDO_PWR_MD_NORMAL,  SEQFLAG_PWRUP },\
	{ 180,  PMIC_L06_CNFG1,     LDO_PWR_MD_NORMAL,  0 },        \
	{ 197,  GPIO_SEQ_EN,        V2_95USD_SW_EN,     0 },        \
	{ 246,  RTCOUT_SEQ_EN,      RTCCFG_OUTEN1,      0 },        \
	{ 246,  RTCOUT_SEQ_EN,      RTCCFG_OUTEN2,      0 },        \
	{ 246,  GPIO_SEQ_EN,        NRESPWRON,          0 },


// during a hard reset, these supply will NOT be disabled
#define PLATFORM_HARD_RESET_MASK_LIST	\
	PMIC_VOUTCONFIG1B3,					\
	PMIC_VOUTCONFIG1B5

// during a warm reset, these supply will NOT be disabled
#define PLATFORM_WARM_RESET_MASK_LIST	\
	PMIC_VOUTCONFIG1B3,					\
	PMIC_VOUTCONFIG1B5

// LDO_TYPE/DSG_EN/ECO_EN , LDO_NUM, INIT_VALUE
#define BUCK01_STARTS_SETTING ( 0x40 )   // VCC_CORE:  1.4V
#define BUCK02_STARTS_SETTING ( 0x60 )   // VCC_IVA:   1.8V
#define BUCK03_STARTS_SETTING ( 0x60 )   // V1_8DIG:   1.8V
#define BUCK04_STARTS_SETTING ( 0xBC )   // V2_95DIG:  2.95V
#define BUCK05_STARTS_SETTING ( 0x30 )   // V1_2DIG:   1.2V

#define LDO01_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO02_STARTS_SETTING ( 0x28 )    // V1_8CAM:   1.8V
#define LDO03_STARTS_SETTING ( 0x12 )    // V1_7EFUSE: 1.7V
#define LDO04_STARTS_SETTING ( 0x29 )    // V2_85DIG:  2.85V
#define LDO05_STARTS_SETTING ( 0x29 )    // V2_85EMMC: 2.85V
#define LDO06_STARTS_SETTING ( 0x29 )    // V2_85USD:  2.85V
#define LDO07_STARTS_SETTING ( 0x22 )    // V2_5SENS:  2.5V
#define LDO08_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO09_STARTS_SETTING ( 0x2E )    // V3_1USB:   3.1V
#define LDO10_STARTS_SETTING ( 0x2E )    // V3_1VIB:   3.1V
#define LDO11_STARTS_SETTING ( 0x14 )    // V1_8VCCGPIO3
#define LDO12_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO13_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO14_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO15_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO16_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO17_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO18_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO19_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO20_STARTS_SETTING ( 0x00 )    // NC:        0.8V
#define LDO21_STARTS_SETTING ( 0x26 )    // V2_7ANT:   2.7V
#define LDO22_STARTS_SETTING ( 0x14 )    // V1_8COD:   1.8V
#define LDO23_STARTS_SETTING ( 0x83 )    // V5_0HDMI:  5.0V

#endif /*_CONFIG_PWRSEQ_H_*/
