#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

#define AUD_PROG_EN                 PAD_PORT_1_0        // AUD_PROG_EN
#define CLK_ON                      PAD_PORT_1_1        // SYS_26M_EN           // verify this is right
#define TX_EN                       PAD_PORT_1_2        // TX_EN
#define BWF_26M_EN                  PAD_PORT_1_3        // BWF_26M_EN
#define KEYVOL_UP_DN                PAD_PORT_1_4        // KEYVOL_UP_DN
#define GPS_TCXO_EN                 PAD_PORT_1_5        // GPS_TCXO_EN
#define WLAN_LNA_EN                 PAD_PORT_1_6        // WLAN_LNA_EN
#define WLAN_PA_EN                  PAD_PORT_1_7        // WLAN_PA_EN

#define NO_PWR_N                    PAD_PORT_2_0        // BATTFAULT_N
#define NO_RTC_N                    PAD_PORT_2_1        // RESET_IN_N           // verify this is right
#define SRST_N                      PAD_PORT_2_2        // SRST_N               // what is this signal? looks to be DNP'd on SCH
#define SYS_EN                      PAD_PORT_2_3        // PMIC_SYS_EN
#define RESET_OUT_N                 PAD_PORT_2_4        // RESET_OUT_N          // what is this signal? looks to be DNP'd on SCH
#define LCD_RST_N                   PAD_PORT_2_5        // LCD_RST_N
#define PMIC_NPWRINT                PAD_PORT_2_6            // Internal - Do not modify
#define FG_SOC_INT                  PAD_PORT_2_7            // Internal - Do not modify

#define PMIC_SPI_CS                 PAD_PORT_3_0            // Internal - Do not modify
#define LS_IN                       PAD_PORT_3_1        // LS_IN
#define WB_PA_TEMP                  PAD_PORT_3_2        // WB_PA_TEMP
#define GPS_TEMP                    PAD_PORT_3_3        // GPS_TEMP
#define EDGE_PA_TEMP                PAD_PORT_3_4        // EDGE_PA_TEMP
#define BB_TEMP                     PAD_PORT_3_5        // BB_TEMP
#define WLAN_PA_TEMP                PAD_PORT_3_6        // WLAN_PA_TEMP
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
//#define                           PAD_PORT_5_5        // NC
#define BACKUP_PWR_EN               PAD_PORT_5_6            // Internal - Do not modify
#define FG_I2C_SDA                  PAD_PORT_5_7            // Internal - Do not modify

#define CORE_SW_VID0                PAD_PORT_J_0        // CORE_SW_VID0
#define CORE_SW_EN                  PAD_PORT_J_1        // CORE_SW_EN
#define WB_TX_EN                    PAD_PORT_J_2        // WB_EX_EN
#define LS_PWR                      PAD_PORT_J_3        // LS_PWR

#endif /*_CONFIG_GPIO_H_*/
