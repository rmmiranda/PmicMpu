#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

#define FLT_TOL_CAL                 PAD_PORT_1_0        // POWER.ROCKET_P1_0
//#define LCD_RST_N                   PAD_PORT_1_1        // RESET.DISPLAY_N, DNP
#define TX_EN                       PAD_PORT_1_2        // TX_EN
#define ROCKET_THER_EN              PAD_PORT_1_3        // POWER.ROCKET_P1_3
#define KEYVOL_UP_DN                PAD_PORT_1_4        // POWER.KEYVOL_UP_DN
#define DIGRF_CLK_EN                PAD_PORT_1_5        // DIGRF_CLK_EN
#define TCXO_VCC_EN_SW              PAD_PORT_1_6        // POWER.TCXO_VCC_EN_SW, DNP
#define RF_ACTIVITY                 PAD_PORT_1_7        // RF_ACTIVITY

#define NO_PWR_N                    PAD_PORT_2_0        // BATTFAULT_N
#define NO_RTC_N                    PAD_PORT_2_1        // RESET_IN_N
#define SRST_N                      PAD_PORT_2_2        // GPIO_RST_N
#define SYS_EN                      PAD_PORT_2_3        // PMIC_SYS_EN
#define BWF_26M_EN                  PAD_PORT_2_4        // BWF_TCXO_EN
#define CLK_ON                      PAD_PORT_2_5        // SYS_26M_EN
#define PMIC_NPWRINT                PAD_PORT_2_6            // Internal - Do not modify
#define FG_SOC_INT                  PAD_PORT_2_7            // Internal - Do not modify

#define PMIC_SPI_CS                 PAD_PORT_3_0            // Internal - Do not modify
#define LS_IN                       PAD_PORT_3_1        // LS_IN
#define ROCKET_TEMP                 PAD_PORT_3_2        // POWER.ADC_P3_2, thermistor placed near Rocket by 32kHz xtal
#define REF_THER                    PAD_PORT_3_3        // POWER.ADC_P3_3, reference thermistor // TODO, could this be ADC_REF?
#define WLAN_TEMP                   PAD_PORT_3_4        // POWER.ADC_P3_4, thermistor placed near WLAN
#define GPS_TEMP                    PAD_PORT_3_5        // POWER.ADC_P3_5, thermistor placed near AUDIO_GPS
#define MAIN_PA_TEMP                PAD_PORT_3_6        // POWER.ADC_P3_6, thermistor placed near MAIN_PA
#define FG_I2C_SCL                  PAD_PORT_3_7            // Internal - Do not modify

#define HOST_I2C_SCL                PAD_PORT_4_0        // PMIC_C_SCL
#define HOST_I2C_SDA                PAD_PORT_4_1        // PMIC_C_SDA
#define I2C_RCKT_SCL                PAD_PORT_4_2        // POWER.I2C_RCKT_SCL // used to go to external switcher
#define I2C_RCKT_SDA                PAD_PORT_4_3        // POWER.I2C_RCKT_SDA // used to go to external switcher
//#define NC                        PAD_PORT_4_4        // TP1425
#define PWR_I2C_SCL                 PAD_PORT_4_5        // PMIC_SCL
#define PWR_I2C_SDA                 PAD_PORT_4_6        // PMIC_SDA
#define PMIC_INT_N                  PAD_PORT_4_7        // PMIC_INT_N

#define PMIC_SPI_SIMO               PAD_PORT_5_0
#define PMIC_SPI_SOMI               PAD_PORT_5_1
#define PMIC_SPI_CLK                PAD_PORT_5_2
#define PMIC_32K_CLK                PAD_PORT_5_3        // 32KHZ_OUT
#define EXT_32K                     PAD_PORT_5_4        // PMIC_32K_CLK (formerly EXT_32k?)
#define TAVOR_THER_EN               PAD_PORT_5_5        // POWER.ROCKET_P5_5
#define BACKUP_PWR_EN               PAD_PORT_5_6            // Internal - Do not modify
#define FG_I2C_SDA                  PAD_PORT_5_7            // Internal - Do not modify

#define CORE_SW_VID0                PAD_PORT_J_0        // POWER.CORE_SW_VID0
#define CORE_SW_EN                  PAD_PORT_J_1        // POWER.CORE_SW_EN
#define LS_PWR                      PAD_PORT_J_2        // LS_PWR
#define END_KEY                     PAD_PORT_J_3        // END_KEY

#endif /*_CONFIG_GPIO_H_*/
