#ifndef _CONFIG_PWRSEQ_H_
#define _CONFIG_PWRSEQ_H_

#include "hostctrl.h"

#define PLATFORM_SUPPLY_SEQUENCE                               \
    { 0,   GPIO_SEQ_EN, V1_2DIG_SW_EN, 0 },                    \
    { 0,   PMIC_EN_CMD0, CMD0_EN_SM0 , 0 },                    \
    { 0,   GPIO_SEQ_EN, VCC_MPU_SW_EN, 0 },                    \
    { 0,   PMIC_EN_CMD0, CMD0_EN_SM1 , 0 },                    \
    { 0,   GPIO_SEQ_EN, NRESPWRON, 0 }

// during a hard reset, these supply enables will NOT be disabled
#define CMD0_HARD_RESET_MASK  ( CMD0_EN_SM1 )
#define CMD1_HARD_RESET_MASK  ( 0 )
#define CMD2_HARD_RESET_MASK  ( 0 )

// during a warm reset, these supply enables will NOT be disabled
#define CMD0_WARM_RESET_MASK  ( 0 )
#define CMD1_WARM_RESET_MASK  ( 0 )
#define CMD2_WARM_RESET_MASK  ( 0 )

// LDO/BUCK Default Voltage
#define SM0_STARTS_SETTING ( LDOSM_DCHG_EN | SM0_1V1000 )      // SM0
#define SM1_STARTS_SETTING ( LDOSM_DCHG_EN | SM1_1V100 )       // SM1
#define SM2_STARTS_SETTING ( LDOSM_DCHG_EN | SM2_1V800 )       // SM2
#define SM3_STARTS_SETTING ( LDOSM_DCHG_EN | SM3_2V950 )       // SM3

#define LDO0_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V750 )    // GPLDO
#define LDO1_STARTS_SETTING ( LDOSM_DCHG_EN | LVLDO_1V200 )    // LVLDO
#define LDO2_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO
#define LDO3_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V500 )    // GPLDO
#define LDO4_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V800 )    // GPLDO
#define LDO5_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_3V100 )    // GPLDO
#define LDO6_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO
#define LDO7_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_1V800 )    // GPLDO
#define LDO8_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO
#define LDO9_STARTS_SETTING ( LDOSM_DCHG_EN | GPLDO_2V850 )    // GPLDO
#define LDO10_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_2V85 )    // HVLDO
#define LDO11_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_2V85 )    // HVLDO
#define LDO12_STARTS_SETTING ( LDOSM_DCHG_EN | HVLDO_1V80 )    // HVLDO


#endif /*_CONFIG_PWRSEQ_H_*/
