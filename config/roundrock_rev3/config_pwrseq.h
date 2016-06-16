#ifndef _CONFIG_PWRSEQ_H_
#define _CONFIG_PWRSEQ_H_

#include "hostctrl.h"

#define PLATFORM_SUPPLY_SEQUENCE                                  \
    { MS_TO_RTC_TICKS(0.0), PMIC_EN_CMD0, CMD0_EN_SM2 , 0 },      \
    { MS_TO_RTC_TICKS(0.0), PMIC_EN_CMD1, CMD1_EN_LDO4 , 0 },     \
    { MS_TO_RTC_TICKS(0.0), PMIC_EN_CMD1, CMD1_EN_LDO6 , 0 },     \
    { MS_TO_RTC_TICKS(1.0), PMIC_EN_CMD2, CMD2_EN_LDO10, 0 },     \
    { MS_TO_RTC_TICKS(5.0), PMIC_EN_CMD1, CMD1_EN_LDO5 , 0 },     \
    { MS_TO_RTC_TICKS(6.5), PMIC_EN_CMD1, CMD1_EN_LDO1 , 0 },     \
    { MS_TO_RTC_TICKS(6.7), GPIO_SEQ_EN, CORE_SW_EN  , 0 }        \

// during a hard reset, these supply enables will NOT be disabled
#define CMD0_HARD_RESET_MASK  ( 0 )                                          //ignore 
#define CMD1_HARD_RESET_MASK  ( 0 )
#define CMD2_HARD_RESET_MASK  ( 0 )

// during a warm reset, these supply enables will NOT be disabled
#define CMD0_WARM_RESET_MASK  ( CMD1_EN_LDO1 | CMD0_EN_SM2 | CMD0_EN_SM3 )  //LDO_1, sm_2, sm_3
#define CMD1_WARM_RESET_MASK  ( CMD1_EN_LDO6 | CMD1_EN_LDO5  )            //6,10,5
#define CMD2_WARM_RESET_MASK  ( CMD2_EN_LDO10  )

// LDO/BUCK Default Voltage
#define SM0_STARTS_SETTING ( LDOSM_DCHG_EN | SM0_1V2000 )      // SM0    VCC_MAIN >>>not used, run off ext switcher, off
#define SM1_STARTS_SETTING ( LDOSM_DCHG_EN | SM1_1V750 )       // SM1    VSM1 >>>>not routed, off
#define SM2_STARTS_SETTING ( LDOSM_DCHG_EN | SM2_1V800 )       // SM2    v1_8Dig 
#define SM3_STARTS_SETTING ( LDOSM_DCHG_EN | SM3_2V950 )       // SM3    V_SYS

#define LDO0_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO   V3_0SIM >>>3/1.8V , treat like LDO6 on Kelowna
#define LDO1_STARTS_SETTING ( LDOSM_DCHG_EN | LVLDO_1V200 )    // LVLDO   V1_2MIPI >>>>1.2V , treat like SM0 on kelowna
#define LDO2_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO   V1_8_SLDO2_HCI1 /  VCC_LDO2 and V_LDO2_BQ , default on
#define LDO3_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V500 )    // GPLDO   V2_5_SLDO_HCI1 / VCC_BQH_LDO3,default off control by OS
#define LDO4_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO   V1_8_SLDO1_HCI1 / VCC_MVT(_PLL), Kelowna uses V1_8 dig for the PLL, treat like SM2
#define LDO5_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_3V100 )    // GPLDO   V3_3LEDUSB >>>>3.3V default on, treat like LDO5 on Kelowna
#define LDO6_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO   V2_85EMMC / VCC_LDO5, treat like LDO0 on Kelowna
#define LDO7_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO   V2_8AUDDIG >>>>kelowna runs this off V2_85DIG so treat thesame
#define LDO8_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO   GPIOs                                         >>>>>>>>>when  does this turn on?
#define LDO9_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO   V_SDMEM >>>> 2.85V treat like LDO2 on Kelowna
#define LDO10_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_2V80 )    // HVLDO   V2_85DIG >>> 2.85V treat like LDO4 on Kelowna
#define LDO11_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_2V85 )    // HVLDO   (ROCKET VLDO11) no routing  off
#define LDO12_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_1V80 )    // HVLDO   (ROCKET VLDO12) no routing  off

// configure MODE voltages for external switcher               //whatever they should be for C2/E0 and TPS62361 switcher. +5%? as the sns lines don't go to MG
#define EXT_SUPPLY_MODE0    (750)
#define EXT_SUPPLY_MODE1    (750)
#define EXT_SUPPLY_MODE2    (1125)
#define EXT_SUPPLY_MODE3    (1350)

// LDOs enable/disable controlled by external triggers (GPIOS)
#define GPIO_PWR_SEQUENCE       1                  // enable the IO control of the following supplies
//                                Ctrl
//                                Reg           EnableMask     GPIO Trigg
// ECO Mode Enable
#define CLK_ON_SUPPLY           { PMIC_EN_CMD0, CMD0_EN_ECO,   CLK_ON }


#endif /*_CONFIG_PWRSEQ_H_*/
