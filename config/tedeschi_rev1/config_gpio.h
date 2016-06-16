#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

#define DIGRF_CLK_EN                PAD_PORT_1_0        // DIGRF_CLK_EN
#define AP_26M_REQ_EN               PAD_PORT_1_1        // AP_26M_REQ_EN
#define TXON_MON_2G                 PAD_PORT_1_2        // 2G_TXON_MON
#define TXON_MON_3G                 PAD_PORT_1_3        // 3G_TXON_MON
#define END_KEY                     PAD_PORT_1_4        // END_KEY_N
#define WLAN_PWM_REQ                PAD_PORT_1_5        // WLAN_PWM_REQ
#define KEYVOL_UP_DN                PAD_PORT_1_6        // KEYVOL_UP_DN
#define RF_ACTIVITY                 PAD_PORT_1_7        // RF_ACTIVITY

#define CP_26M_REQ_EN               PAD_PORT_2_0        // CP_26M_CLK_EN
#define NRESPWRON                   PAD_PORT_2_1        // NRESPWRON
#define NRESWARM                    PAD_PORT_2_2        // NRESWARM
#define MSECURE                     PAD_PORT_2_3        // MSECURE
#define ESHEL_DVFM0                 PAD_PORT_2_4        // ESHEL_DVFM0
#define ESHEL_DVFM1                 PAD_PORT_2_5        // ESHEL_DVFM1
#define PMIC_NPWRINT                PAD_PORT_2_6        // Internal - Do not modify
#define FG_SOC_INT                  PAD_PORT_2_7        // Internal - Do not modify

#define PMIC_SPI_CS                 PAD_PORT_3_0        // Internal - Do not modify
//#define                             PAD_PORT_3_1        //
#define ADC_MUX                     PAD_PORT_3_2        // ADC_MUX
#define WBG_XTAL_TEMP               PAD_PORT_3_3        // WBG_XTAL_TEMP
#define FLT_EN_N                    PAD_PORT_3_4        // FLT_EN_N
#define ADC_REF                     PAD_PORT_3_5        // ADC_REF
#define V1_8DIG_PWM                 PAD_PORT_3_6        // V1_8DIG_PWM
#define FG_I2C_SCL                  PAD_PORT_3_7        // Internal - Do not modify

#define HOST_I2C_SCL                PAD_PORT_4_0        // PM_SCL
#define HOST_I2C_SDA                PAD_PORT_4_1        // PM_SDA
#define I2C_RCKT_SCL                PAD_PORT_4_2        // PM2REG_I2C_SCL
#define I2C_RCKT_SDA                PAD_PORT_4_3        // PM2REG_I2C_SDA
#define LCD_RST_N                   PAD_PORT_4_4        // LCD_RST_N
#define PWR_I2C_SCL                 PAD_PORT_4_5        // SR_SCL
#define PWR_I2C_SDA                 PAD_PORT_4_6        // SR_SDA
#define PMIC_INT_N                  PAD_PORT_4_7        // PMIC_INT_N

#define PMIC_SPI_SIMO               PAD_PORT_5_0
#define PMIC_SPI_SOMI               PAD_PORT_5_1
#define PMIC_SPI_CLK                PAD_PORT_5_2
#define SYS_32K_CLK1                PAD_PORT_5_3        // SYS_32K_CLK1
#define SYS_32K_CLK0                PAD_PORT_5_4        // SYS_32K_CLK0
//#define                             PAD_PORT_5_5        //
#define BACKUP_PWR_EN               PAD_PORT_5_6        // Internal - Do not modify
#define FG_I2C_SDA                  PAD_PORT_5_7        // Internal - Do not modify

#define V1_8DIG_SW_EN               PAD_PORT_J_0        // V1_8DIG_SW_EN
#define V1_2DIG_SW_EN               PAD_PORT_J_1        // V1_2DIG_SW_EN
#define VCC_MPU_SW_EN               PAD_PORT_J_2        // VCC_MPU_SW_EN
//#define                             PAD_PORT_J_3        //

#endif /*_CONFIG_GPIO_H_*/
