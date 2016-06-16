#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

//#define AUD_PROG_EN                 PAD_PORT_1_0        // not used 
#define CLK_ON                      PAD_PORT_1_1        // SYS_26M_EN
#define TX_EN                       PAD_PORT_1_2        // TX_EN 
//#define BWF_26M_EN                  PAD_PORT_1_3        // not used
//#define KEYVOL_UP_DN                PAD_PORT_1_4        // not used
#define WB_TX_EN	            PAD_PORT_1_5        // GPS_TCXO_EN
#define WLAN_LNA_EN                 PAD_PORT_1_6        // WLAN_LNA_EN
#define WLAN_PA_EN                  PAD_PORT_1_7        // WLAN_PA_EN

#define NO_PWR_N                    PAD_PORT_2_0        // BATTFAULT_N
#define NO_RTC_N                    PAD_PORT_2_1        // RESET_IN_N
#define SRST_N                      PAD_PORT_2_2        // set as input
#define SYS_EN                      PAD_PORT_2_3        // PMIC_SYS_EN
#define RESET_OUT_N                 PAD_PORT_2_4        // bq_gpio4 - set low to not disturb LCD_RST_N
#define LCD_RST_N                   PAD_PORT_2_5        // set as input   
#define PMIC_NPWRINT                PAD_PORT_2_6            // Internal - Do not modify
#define FG_SOC_INT                  PAD_PORT_2_7            // Internal - Do not modify

#define PMIC_SPI_CS                 PAD_PORT_3_0            // Internal - Do not modify
//#define WLAN_PA_TEMP                PAD_PORT_3_1        // not used
//#define BB_TEMP                     PAD_PORT_3_2        // not used
//#define GPS_TEMP                    PAD_PORT_3_3        // not used
//#define EDGE_PA_TEMP                PAD_PORT_3_4        // not used
#define WB_PA_TEMP                  PAD_PORT_3_5        // WB_PA_TEMP
#define LS_IN                       PAD_PORT_3_6        //LS in
#define FG_I2C_SCL                  PAD_PORT_3_7            // Internal - Do not modify

#define HOST_I2C_SCL                PAD_PORT_4_0        // SYS_SCL   //I2C link to onboard FG - No tavor
#define HOST_I2C_SDA                PAD_PORT_4_1        // SYS_SDA   //I2C link to onboard FG - no Tavor
#define RST_1V8_N                   PAD_PORT_4_2        //  RST_1V8_N, go high with sys_en
#define I2C_RCKT_SDA                PAD_PORT_4_3        // I2C_RCKT_SDA // switcher I2C
#define I2C_RCKT_SCL                PAD_PORT_4_4        // I2C_RCKT_SCL, switcher i2c
#define PWR_I2C_SCL                 PAD_PORT_4_5        // PMIC_SCL   //TPWR_SCL tavor scl 
#define PWR_I2C_SDA                 PAD_PORT_4_6        // PMIC_SDA   //TPWR_SDA tavor sda 
#define PMIC_INT_N                  PAD_PORT_4_7        // PMIC_INT_N

#define PMIC_SPI_SIMO               PAD_PORT_5_0        //not used
#define PMIC_SPI_SOMI               PAD_PORT_5_1        //not used
#define PMIC_SPI_CLK                PAD_PORT_5_2        //not used
#define PMIC_32K_CLK                PAD_PORT_5_3	// RST_2V8_N, looped to 5_5, high z
#define EXT_32K                     PAD_PORT_5_4        // PMIC_32K_CLK (formerly EXT_32k?) EXT32k
#define RST_2V8_N                   PAD_PORT_5_5        //   RST_2V8_N, go high with sys_en
#define BACKUP_PWR_EN               PAD_PORT_5_6            // Internal - Do not modify
#define FG_I2C_SDA                  PAD_PORT_5_7            // Internal - Do not modify

#define CORE_SW_VID0                PAD_PORT_J_0        // CORE_SW_VID0
#define CORE_SW_EN                  PAD_PORT_J_1        // CORE_SW_EN 
//#define GPS_TCXO_EN                 PAD_PORT_J_2        // not used, just jtag
//#define LS_PWR                      PAD_PORT_J_3        // not used, just JTAG

#endif /*_CONFIG_GPIO_H_*/
