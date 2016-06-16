#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

//#define                             PAD_PORT_1_0
#define CLK_ON                      PAD_PORT_1_1
#define TX_EN                       PAD_PORT_1_2
#define TP_229                      PAD_PORT_1_3
#define TP_228                      PAD_PORT_1_4
#define WB_TX_EN                    PAD_PORT_1_5
#define WLAN_LNA_EN                 PAD_PORT_1_6
#define WLAN_PA_EN                  PAD_PORT_1_7

#define NO_PWR_N                    PAD_PORT_2_0
#define NO_RTC_N                    PAD_PORT_2_1
#define WAKEUP                      PAD_PORT_2_2
#define SYS_EN                      PAD_PORT_2_3
#define BQ_GPIO_4                   PAD_PORT_2_4
#define LDO4_PG                     PAD_PORT_2_5            // TP240
#define PMIC_NPWRINT                PAD_PORT_2_6            // Internal - Do not modify
#define FG_SOC_INT                  PAD_PORT_2_7            // Internal - Do not modify

#define PMIC_SPI_CS                 PAD_PORT_3_0            // Internal - Do not modify
#define ADCIN_A1                    PAD_PORT_3_1
#define ADCIN_A2                    PAD_PORT_3_2
#define ADCIN_A3                    PAD_PORT_3_3
#define ADCIN_A4                    PAD_PORT_3_4
#define PA_TEMP                     PAD_PORT_3_5
#define ADCIN_LIGHTSENSE            PAD_PORT_3_6
#define FG_I2C_SCL                  PAD_PORT_3_7            // Internal - Do not modify

#define HOST_I2C_SCL                PAD_PORT_4_0
#define HOST_I2C_SDA                PAD_PORT_4_1
#define RST_1V8_N                   PAD_PORT_4_2
#define I2C_RCKT_SDA                PAD_PORT_4_3
#define I2C_RCKT_SCL                PAD_PORT_4_4
#define PWR_I2C_SCL                 PAD_PORT_4_5
#define PWR_I2C_SDA                 PAD_PORT_4_6
#define PMIC_INT_N                  PAD_PORT_4_7

#define PMIC_SPI_SIMO               PAD_PORT_5_0
#define PMIC_SPI_SOMI               PAD_PORT_5_1
#define PMIC_SPI_CLK                PAD_PORT_5_2
//#define RST_2V8_N                   PAD_PORT_5_3			// this looks wrong? should be 5.5
#define EXT_32K                     PAD_PORT_5_4
#define RST_2V8_N                   PAD_PORT_5_5
#define BACKUP_PWR_EN               PAD_PORT_5_6            // Internal - Do not modify
#define FG_I2C_SDA                  PAD_PORT_5_7            // Internal - Do not modify

#define CORE_SW_VID0                PAD_PORT_J_0
#define CORE_SW_EN                  PAD_PORT_J_1
#define PMU430_TMS                  PAD_PORT_J_2
#define PMU430_TCK                  PAD_PORT_J_3


#endif /*_CONFIG_GPIO_H_*/
