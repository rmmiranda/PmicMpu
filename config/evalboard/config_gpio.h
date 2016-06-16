#ifndef _CONFIG_GPIO_H_
#define _CONFIG_GPIO_H_

//#define                             PAD_PORT_1_0
//#define                             PAD_PORT_1_1
//#define                             PAD_PORT_1_2
//#define                             PAD_PORT_1_3
//#define                             PAD_PORT_1_4
//#define                             PAD_PORT_1_5
//#define                             PAD_PORT_1_6
//#define                             PAD_PORT_1_7

//#define                             PAD_PORT_2_0
//#define                             PAD_PORT_2_1
//#define                             PAD_PORT_2_2
//#define                             PAD_PORT_2_3
//#define                             PAD_PORT_2_4
//#define                             PAD_PORT_2_5
#define PMIC_NPWRINT                PAD_PORT_2_6            // Internal - Do not modify
#define FG_SOC_INT                  PAD_PORT_2_7            // Internal - Do not modify

#define PMIC_SPI_CS                 PAD_PORT_3_0            // Internal - Do not modify
//#define                             PAD_PORT_3_1
//#define                             PAD_PORT_3_2
//#define                             PAD_PORT_3_3
//#define                             PAD_PORT_3_4
//#define                             PAD_PORT_3_5
//#define                             PAD_PORT_3_6
#define FG_I2C_SCL                  PAD_PORT_3_7            // Internal - Do not modify

#define HOST_I2C_SCL                PAD_PORT_4_0
#define HOST_I2C_SDA                PAD_PORT_4_1
#define I2C_RCKT_SCL                PAD_PORT_4_2        // I2C_RCKT_SCL
#define I2C_RCKT_SDA                PAD_PORT_4_3        // I2C_RCKT_SDA
//#define                             PAD_PORT_4_4
#define PWR_I2C_SCL                 PAD_PORT_4_5        // PMIC_SCL
#define PWR_I2C_SDA                 PAD_PORT_4_6        // PMIC_SDA
//#define                             PAD_PORT_4_7

#define PMIC_SPI_SIMO               PAD_PORT_5_0
#define PMIC_SPI_SOMI               PAD_PORT_5_1
#define PMIC_SPI_CLK                PAD_PORT_5_2
//#define                             PAD_PORT_5_3
//#define                             PAD_PORT_5_4
//#define                             PAD_PORT_5_5
#define BACKUP_PWR_EN               PAD_PORT_5_6            // Internal - Do not modify
#define FG_I2C_SDA                  PAD_PORT_5_7            // Internal - Do not modify

//#define                             PAD_PORT_J_0
//#define                             PAD_PORT_J_1
//#define                             PAD_PORT_J_2
//#define                             PAD_PORT_J_3

#endif /*_CONFIG_GPIO_H_*/
