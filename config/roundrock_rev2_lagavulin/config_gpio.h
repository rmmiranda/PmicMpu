#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

#define HOST_I2C_SDA                PAD_PORT_0_0
#define HOST_I2C_SCL                PAD_PORT_0_1
#define PMIC_INT_N                  PAD_PORT_0_3
#define WLAN_LNA_EN                 PAD_PORT_0_4
#define WLAN_ACTIVITY               PAD_PORT_0_7

#define PWR_I2C_SDA                 PAD_PORT_1_0
#define PWR_I2C_SCL                 PAD_PORT_1_1
#define RST_1V8_N                   PAD_PORT_1_2
#define PMIC_SPI_SIMO				PAD_PORT_1_4
#define PMIC_SPI_SOMI				PAD_PORT_1_5
#define PMIC_SPI_CLK				PAD_PORT_1_6
#define PMIC_SPI_CS					PAD_PORT_1_7

#define RST_2V8_N                   PAD_PORT_3_0
#define CLK_ON                      PAD_PORT_3_2
#define WB_TX_EN                    PAD_PORT_3_5
#define WLAN_PA_EN                  PAD_PORT_3_6
#define TX_EN                       PAD_PORT_3_7

#define NO_RTC_N                    PAD_PORT_4_0
#define NO_PWR_N                    PAD_PORT_4_1
#define WAKEUP                      PAD_PORT_4_2
#define SYS_EN                      PAD_PORT_4_3

#endif /*_CONFIG_GPIO_H_*/
