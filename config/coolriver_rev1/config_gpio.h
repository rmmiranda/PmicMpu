#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

#define HOST_I2C_SDA                PAD_PORT_0_0
#define HOST_I2C_SCL                PAD_PORT_0_1
#define NRESPWRON                   PAD_PORT_0_2
#define NRESWARM                    PAD_PORT_0_3
#define END_KEY_N                   PAD_PORT_0_4
#define PMIC_INT_N                  PAD_PORT_0_5
#define TEST1                       PAD_PORT_0_6
#define NOT_CONNECTED1              PAD_PORT_0_7

#define PWR_I2C_SDA                 PAD_PORT_1_0
#define PWR_I2C_SCL                 PAD_PORT_1_1
#define MSECURE                     PAD_PORT_1_2
#define NOT_CONNECTED2              PAD_PORT_0_3
#define PMIC_SPI_SIMO				PAD_PORT_1_4
#define PMIC_SPI_SOMI				PAD_PORT_1_5
#define PMIC_SPI_CLK				PAD_PORT_1_6
#define PMIC_SPI_CS					PAD_PORT_1_7

#define WLAN_PWM_REQ                PAD_PORT_3_0
#define CP_26M_CLK_EN               PAD_PORT_3_1
#define AP_26M_CLK_EN               PAD_PORT_3_2
#define KEY_VOL_UP_DN               PAD_PORT_3_3
#define CAM_FLSH_SYNC               PAD_PORT_3_4
#define TXON_MON_3G                 PAD_PORT_3_5
#define CAM_1V8_EN                  PAD_PORT_3_6
#define TXON_MON_2G                 PAD_PORT_3_7

#define LCD_RST_N                   PAD_PORT_4_0
#define VCC_MPU_SW_EN               PAD_PORT_4_1
#define V1_2DIG_SW_EN               PAD_PORT_4_2
#define V2_95USD_SW_EN              PAD_PORT_4_3

#endif /*_CONFIG_GPIO_H_*/
