#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

//#define NC                        PAD_PORT_1_0        // NC
#define CLK_ON                      PAD_PORT_1_1        // SYS_26M_EN
#define TX_EN                       PAD_PORT_1_2        // TX_EN
#define BWF_26M_EN                  PAD_PORT_1_3        // BWF_26M_EN
#define KEYVOL_UP_DN                PAD_PORT_1_4        // KEYVOL_UP_DN
#define DIGRF_CLK_EN                PAD_PORT_1_5        // DIGRF_CLK_EN
//#define NC                        PAD_PORT_1_6        // NC
#define RF_ACTIVITY                 PAD_PORT_1_7        // RF_ACTIVITY

#define NO_PWR_N                    PAD_PORT_2_0        // BATTFAULT_N
#define NO_RTC_N                    PAD_PORT_2_1        // RESET_IN_N
#define SRST_N                      PAD_PORT_2_2        // GPIO_RST_N
#define SYS_EN                      PAD_PORT_2_3        // PMIC_SYS_EN
#define FLT_EN_N                    PAD_PORT_2_4        // FLT_EN_N
#define LCD_RST_N                   PAD_PORT_2_5        // LCD_RST_N
#define PMIC_NPWRINT                PAD_PORT_2_6            // Internal - Do not modify
#define FG_SOC_INT                  PAD_PORT_2_7            // Internal - Do not modify

#define PMIC_SPI_CS                 PAD_PORT_3_0            // Internal - Do not modify
//#define NC                        PAD_PORT_3_1        // NC
#define TEMP_APP_CORE               PAD_PORT_3_2        // Temp sensor for apps core to support thermal back-up plan
#define GPS_TEMP                    PAD_PORT_3_3        // GPS_TEMP
#define TEMP_32KHZ_XTAL             PAD_PORT_3_4        // Temp sensor for 32 kHz oscillator to support sleep clock calibration
#define ADC_REF                     PAD_PORT_3_5        // ADC_REF
//#define NC                        PAD_PORT_3_6        // NC
#define FG_I2C_SCL                  PAD_PORT_3_7            // Internal - Do not modify

#define HOST_I2C_SCL                PAD_PORT_4_0        // SYS_SCL
#define HOST_I2C_SDA                PAD_PORT_4_1        // SYS_SDA
#define I2C_RCKT_SCL                PAD_PORT_4_2        // I2C_RCKT_SCL
#define I2C_RCKT_SDA                PAD_PORT_4_3        // I2C_RCKT_SDA
#define RST_1V8_N                   PAD_PORT_4_4        // RST_1V8_N
#define PWR_I2C_SCL                 PAD_PORT_4_5        // PMIC_SCL
#define PWR_I2C_SDA                 PAD_PORT_4_6        // PMIC_SDA
#define PMIC_INT_N                  PAD_PORT_4_7        // PMIC_INT_N

#define PMIC_SPI_SIMO               PAD_PORT_5_0
#define PMIC_SPI_SOMI               PAD_PORT_5_1
#define PMIC_SPI_CLK                PAD_PORT_5_2
#define PMIC_32K_CLK                PAD_PORT_5_3		// SYS_32K_CLK 
#define EXT_32K                     PAD_PORT_5_4        // PMIC_32K_CLK (formerly EXT_32k?)
//#define NC                        PAD_PORT_5_5        // NC
#define BACKUP_PWR_EN               PAD_PORT_5_6            // Internal - Do not modify
#define FG_I2C_SDA                  PAD_PORT_5_7            // Internal - Do not modify

#define CORE_SW_VID0                PAD_PORT_J_0        // CORE_SW_VID0
#define CORE_SW_EN                  PAD_PORT_J_1        // CORE_SW_EN
//#define NC                        PAD_PORT_J_2        // NC
#define END_KEY                     PAD_PORT_J_3        // END_KEY

#endif /*_CONFIG_GPIO_H_*/
