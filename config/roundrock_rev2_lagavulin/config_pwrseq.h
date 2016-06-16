#ifndef _CONFIG_PWRSEQ_H_
#define _CONFIG_PWRSEQ_H_

#define PLATFORM_SUPPLY_SEQUENCE                               \
	{ 00,  PMIC_VOUTCONFIG1B3, BUCK_PWR_MD_NORMAL, 0 },        \
	{ 15,  PMIC_L10_CNFG1,     LDO_PWR_MD_NORMAL,  0 },        \
	{ 15,  PMIC_L19_CNFG1,	   LDO_PWR_MD_NORMAL,  0 },        \
	{ 53,  PMIC_VOUTCONFIG1B1, BUCK_PWR_MD_NORMAL, 0 },        \
	{ 53,  PMIC_L01_CNFG1,     LDO_PWR_MD_NORMAL,  0 },        \
	{ 53,  PMIC_L04_CNFG1,     LDO_PWR_MD_NORMAL,  0 },        \
	{ 88,  PMIC_L05_CNFG1,     LDO_PWR_MD_NORMAL,  0 }         \


// during a hard reset, these supply will NOT be disabled
#define PLATFORM_HARD_RESET_MASK_LIST	0x00
	// empty list

// during a warm reset, these supply will NOT be disabled
#define PLATFORM_WARM_RESET_MASK_LIST	\
	PMIC_VOUTCONFIG1B1,					\
	PMIC_VOUTCONFIG1B3,					\
	PMIC_VOUTCONFIG1B5,					\
	PMIC_L01_CNFG1,						\
	PMIC_L04_CNFG1,						\
	PMIC_L05_CNFG1,						\
	PMIC_L10_CNFG1,						\
	PMIC_L19_CNFG1

// LDO_TYPE/DSG_EN/ECO_EN , LDO_NUM, INIT_VALUE
#define BUCK01_STARTS_SETTING ( 0x40 )   // 1.4V MG workaround to switch from 624MHz point !!! Please revert to 1.2V (0x30)
#define BUCK02_STARTS_SETTING ( 0x00 )   //
#define BUCK03_STARTS_SETTING ( 0x60 )   // 1.8V
#define BUCK04_STARTS_SETTING ( 0x00 )   //
#define BUCK05_STARTS_SETTING ( 0xC8 )   // 3.1V

#define LDO01_STARTS_SETTING ( 0x10 )    // 1.2V
#define LDO02_STARTS_SETTING ( 0x00 )    //
#define LDO03_STARTS_SETTING ( 0x14 )    // 1.8V
#define LDO04_STARTS_SETTING ( 0x14 )    // 1.8V
#define LDO05_STARTS_SETTING ( 0x29 )    // 2.85V
#define LDO06_STARTS_SETTING ( 0x00 )    //
#define LDO07_STARTS_SETTING ( 0x29 )    // 2.85V
#define LDO08_STARTS_SETTING ( 0x00 )    //
#define LDO09_STARTS_SETTING ( 0x29 )    // 2.85V
#define LDO10_STARTS_SETTING ( 0x29 )    // 2.85V
#define LDO11_STARTS_SETTING ( 0x00 )    //
#define LDO12_STARTS_SETTING ( 0x00 )    //
#define LDO13_STARTS_SETTING ( 0x14 )    // 1.8V
#define LDO14_STARTS_SETTING ( 0x22 )    // 2.5V
#define LDO15_STARTS_SETTING ( 0x00 )    //
#define LDO16_STARTS_SETTING ( 0x00 )    //
#define LDO17_STARTS_SETTING ( 0x00 )    //
#define LDO18_STARTS_SETTING ( 0x00 )    //
#define LDO19_STARTS_SETTING ( 0x32 )    // 3.1V 2E
#define LDO20_STARTS_SETTING ( 0x00 )    //
#define LDO21_STARTS_SETTING ( 0x00 )    //
#define LDO22_STARTS_SETTING ( 0x00 )    //
#define LDO23_STARTS_SETTING ( 0x00 )    //

#endif /*_CONFIG_PWRSEQ_H_*/
