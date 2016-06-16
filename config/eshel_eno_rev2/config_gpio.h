#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

#define RESET_COMMS_OUT_N           PAD_PORT_1_0        // RESET_COMMS_OUT_N
#define APPS_26M_EN                 PAD_PORT_1_1        // APPS_26M_EN
#define TX_EN                       PAD_PORT_1_2        // TX_EN
#define DIGRF_CLK_EN                PAD_PORT_1_3        // DIGRF_CLK_EN
#define END_KEY                     PAD_PORT_1_4        // END_KEY
#define GPS_BWF_EN                  PAD_PORT_1_5        // COMBO_ACTIVE
#define KEYVOL_UP_DN                PAD_PORT_1_6        // KEYVOL_UP_DN
#define RF_ACTIVITY                 PAD_PORT_1_7        // RF_ACTIVITY

#define COMMS_26M_EN                PAD_PORT_2_0        // COMMS_26M_EN
#define RESET_APPS_IN_N             PAD_PORT_2_1        // RESET_APPS_IN_N
#define RESET_APPS_GPIO_N           PAD_PORT_2_2        // RESET_APPS_GPIO_N
#define COMMS_WDT_RESET_N           PAD_PORT_2_3        // COMMS_WDT_RESET_N
#define FLT_EN_N                    PAD_PORT_2_4        // FLT_EN_N
#define LCD_RST_N                   PAD_PORT_2_5        // LCD_RST_N
#define PMIC_NPWRINT                PAD_PORT_2_6            // Internal - Do not modify
#define FG_SOC_INT                  PAD_PORT_2_7            // Internal - Do not modify

#define PMIC_SPI_CS                 PAD_PORT_3_0            // Internal - Do not modify
#define LS_IN                       PAD_PORT_3_1        // LS_IN
#define WB_PA_TEMP                  PAD_PORT_3_2        // WB_PA_TEMP
#define GPS_TEMP                    PAD_PORT_3_3        // GPS_TEMP
//#define NC                          PAD_PORT_3_4      // NC
#define ADC_REF                     PAD_PORT_3_5        // ADC_REF
//#define NC                          PAD_PORT_3_6      // NC
#define FG_I2C_SCL                  PAD_PORT_3_7            // Internal - Do not modify

#define HOST_I2C_SCL                PAD_PORT_4_0        // SYS_SCL
#define HOST_I2C_SDA                PAD_PORT_4_1        // SYS_SDA
#define I2C_RCKT_SCL                PAD_PORT_4_2        // I2C_RCKT_SCL
#define I2C_RCKT_SDA                PAD_PORT_4_3        // I2C_RCKT_SDA
#define RST_EMMC_N                  PAD_PORT_4_4        // RST_EMMC_N
#define PWR_I2C_SCL                 PAD_PORT_4_5        // PMIC_SCL
#define PWR_I2C_SDA                 PAD_PORT_4_6        // PMIC_SDA
#define PMIC_INT_N                  PAD_PORT_4_7        // PMIC_INT_N

#define PMIC_SPI_SIMO               PAD_PORT_5_0
#define PMIC_SPI_SOMI               PAD_PORT_5_1
#define PMIC_SPI_CLK                PAD_PORT_5_2
#define PMIC_32K_CLK                PAD_PORT_5_3		// SYS_32K_CLK  TODO: look at naming
#define EXT_32K                     PAD_PORT_5_4        // PMIC_32K_CLK (formerly EXT_32k?)
//#define                           PAD_PORT_5_5        // NC
#define BACKUP_PWR_EN               PAD_PORT_5_6            // Internal - Do not modify
#define FG_I2C_SDA                  PAD_PORT_5_7            // Internal - Do not modify

#define CORE_SW_VID0                PAD_PORT_J_0        // CORE_SW_VID0
#define CORE_SW_EN                  PAD_PORT_J_1        // CORE_SW_EN
#define WB_TX_EN                    PAD_PORT_J_2        // WB_EX_EN
#define LS_PWR                      PAD_PORT_J_3        // LS_PWR

#endif /*_CONFIG_GPIO_H_*/
