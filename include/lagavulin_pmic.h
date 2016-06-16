/*****************************************************************************
 * Filename:    lagavulin_pmic.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     January 04, 2011
 *
 * Description: Registers and API for Maxim's Lagavulin PMIC
 ****************************************************************************/
#ifndef __LAGAVULIN_PMIC_H__
#define __LAGAVULIN_PMIC_H__

// Lagavulin PowerDie internal register addresses

// CHGR block
#define PMIC_CHG_INT            0x00
#define PMIC_CHG_INT_MASK       0x01
#define PMIC_CHG_INT_OK         0x02
#define PMIC_CHG_DETAILS_00     0x03
#define PMIC_CHG_DETAILS_01     0x04
#define PMIC_CHG_DETAILS_02     0x05
#define PMIC_CHG_DETAILS_03     0x06
#define PMIC_CHG_CNFG_00        0x07
#define PMIC_CHG_CNFG_01        0x08
#define PMIC_CHG_CNFG_02        0x09
#define PMIC_CHG_CNFG_03        0x0A
#define PMIC_CHG_CNFG_04        0x0B
#define PMIC_CHG_CNFG_05        0x0C
#define PMIC_CHG_CNFG_06        0x0D
#define PMIC_CHG_CNFG_07        0x0E
#define PMIC_CHG_CNFG_08        0x0F
#define PMIC_CHG_CNFG_09        0x10
#define PMIC_CHG_CNFG_10        0x11
#define PMIC_CHG_CNFG_11        0x12
#define PMIC_CHG_CNFG_12        0x13
#define PMIC_CHG_CNFG_13        0x14

#define CHGINT_CHGINA			( 1u << 6 )
#define CHGINT_CHGINB			( 1u << 5 )
#define CHGINT_CHG				( 1u << 4 )
#define CHGINT_BAT				( 1u << 3 )
#define CHGINT_THM				( 1u << 2 )
#define CHGINT_BYP				( 1u << 0 )
#define CHGINT_MASK_CHGINA		( 1u << 6 )
#define CHGINT_MASK_CHGINB		( 1u << 5 )
#define CHGINT_MASK_CHG			( 1u << 4 )
#define CHGINT_MASK_BAT			( 1u << 3 )
#define CHGINT_MASK_THM			( 1u << 2 )
#define CHGINT_MASK_BYP			( 1u << 0 )
#define CHGINT_OK_CHGINA		( 1u << 6 )
#define CHGINT_OK_CHGINB		( 1u << 5 )
#define CHGINT_OK_CHG			( 1u << 4 )
#define CHGINT_OK_BAT			( 1u << 3 )
#define CHGINT_OK_THM			( 1u << 2 )
#define CHGINT_OK_BYP			( 1u << 0 )
#define CHGDETAILS_BAT_MASK		( 0x7u << 4 )

// FLASH block
#define PMIC_IFLASH1            0x15
#define PMIC_IFLASH2            0x16
#define PMIC_IFLASH_TX1         0x17
#define PMIC_IFLASH_TX2         0x18
#define PMIC_IFLASH_TX3         0x19
#define PMIC_IFLASH_STAT        0x1A
#define PMIC_ITORCH             0x1B
#define PMIC_IREF               0x1C
#define PMIC_TORCH_TIMER        0x1D
#define PMIC_FLASH_TIMER        0x1E
#define PMIC_FLASH_EN           0x1F
#define PMIC_SCAP_CHG           0x20
#define PMIC_MAX_FLASH1         0x21
#define PMIC_MAX_FLASH2         0x22
#define PMIC_MAX_FLASH3         0x23
#define PMIC_MAX_FLASH4         0x24
#define PMIC_VOUT_CNTL          0x25
#define PMIC_VOUT_FLASH1        0x26
#define PMIC_VOUT_FLASH2        0x27
#define PMIC_IRQ_FLASH1         0x28
#define PMIC_IRQ_FLASH1_MASK    0x29
#define PMIC_FLASH_STATUS1      0x2A
#define PMIC_IRQ_FLASH2         0x2B
#define PMIC_IRQ_FLASH2_MASK    0x2C
#define PMIC_SCAP_SUPPLEMENT    0x2D

// BBATT block
#define PMIC_VBATTREG           0x30
#define PMIC_VRTCREG            0x31
#define PMIC_OUTBBATTOFFTH      0x32

#define OUTBBATT				( 1u << 0 )

// MLDO block
#define PMIC_AVDDEN             0x33
#define PMIC_AVDDSTAT           0x34
#define PMIC_DVDDSTAT           0x35

// FG_ADC block
#define PMIC_FGADC_STATUS       0x36
#define PMIC_FGADC_CFG          0x37
#define PMIC_FGADC_VOLT1        0x38
#define PMIC_FGADC_VOLT0        0x39
#define PMIC_FGADC_VDBNCE1      0x3A
#define PMIC_FGADC_VDBNCE0      0x3B
#define PMIC_FGADC_VNPEAK1      0x3C
#define PMIC_FGADC_VNPEAK0      0x3D
#define PMIC_FGADC_IAVG1        0x3E
#define PMIC_FGADC_IAVG0        0x3F
#define PMIC_FGADC_AIN1         0x40
#define PMIC_FGADC_AIN0         0x41
#define PMIC_FGADC_ACQ_3        0x42
#define PMIC_FGADC_ACQ_2        0x43
#define PMIC_FGADC_ACQ_1        0x44
#define PMIC_FGADC_ACQ_0        0x45
#define PMIC_FGADC_GVOLT        0x46
#define PMIC_FGADC_VOFF         0x47
#define PMIC_FGADC_GCURR_1      0x48
#define PMIC_FGADC_GCURR_2      0x49

#define FGADC_CFG_DISFG			( 1u << 0 )
#define FGADC_CFG_ENNPD			( 1u << 7 )

// GPIO block
#define PMIC_PD3                0x50
#define PMIC_PD4                0x51
#define PMIC_PE3                0x52
#define PMIC_PE4                0x53
#define PMIC_PO3                0x54
#define PMIC_PO4                0x55
#define PMIC_PI3                0x56
#define PMIC_PI4                0x57
#define PMIC_EIE3               0x58
#define PMIC_EIE4               0x59
#define PMIC_EIPS3              0x5A
#define PMIC_EIPS4              0x5B
#define PMIC_EIES3              0x5C
#define PMIC_EIES4              0x5D
#define PMIC_EIF3               0x5E
#define PMIC_EIF4               0x5F
#define PMIC_EFIF3              0x60
#define PMIC_EFIF4              0x61
#define PMIC_AE3                0x62
#define PMIC_AE4                0x63

// ADCMUX block
#define PMIC_ADCMUX             0x65

// ADCREF block
#define PMIC_ADCREF             0x66
#define ADCREF_SW               ( 1u << 3 )
#define ADCREF_REFVAL_1P25V		( 1u << 2 )
#define ADCREF_REFEN			( 1u << 1 )
#define ADCREF_BGEN				( 1u << 0 )

// SYS-GEN, INT-TOP, OFF-ST and 32kCRY blocks
#define PMIC_LSCNFG             0x70
#define PMIC_INTENSYS           0x71
#define PMIC_INTPRSYS           0x72
#define PMIC_STATUSSYS          0x73
#define PMIC_INTFLAGSYSIRQ      0x74
#define PMIC_INTFLAGSYSFIQ      0x75
#define PMIC_MFIQ               0x76
#define PMIC_FFIQ               0x77
#define PMIC_MIRQ               0x78
#define PMIC_FIRQ               0x79
#define PMIC_PDSTATUSREG1       0x7A
#define PMIC_ECOSYS_BYPEN       0x7B
#define PMIC_32KLOAD            0x7C
#define PMIC_32KSTAT            0x7D

#define INT_MASK_MONITOR		( 1u << 7 )
#define INT_MASK_CHARGER		( 1u << 6 )
#define INT_MASK_BUCK			( 1u << 5 )
#define INT_MASK_LDO_C			( 1u << 4 )
#define INT_MASK_LDO_B			( 1u << 3 )
#define INT_MASK_LDO_A			( 1u << 2 )
#define INT_MASK_LIGHTING		( 1u << 1 )
#define INT_MASK_GPIO			( 1u << 0 )

// BUCK block
#define PMIC_VOUT1              0x80
#define PMIC_VOUT3              0x81
#define PMIC_VOUT2              0x82
#define PMIC_VOUT4              0x83
#define PMIC_VOUT5              0x84
#define PMIC_VOUT1_DVS          0x85
#define PMIC_VOUT3_DVS          0x86
#define PMIC_VOUTCONFIG1B1      0x87
#define PMIC_VOUTCONFIG1B2      0x88
#define PMIC_VOUTCONFIG1B3      0x89
#define PMIC_VOUTCONFIG1B4      0x8A
#define PMIC_VOUTCONFIG1B5      0x8B
#define PMIC_INT_PRIORITY       0x8C
#define PMIC_BUCK_IRQ           0x8D
#define PMIC_BUCK_IRQ_MASK      0x8E
#define PMIC_BUCK_IRQ_STATUS    0x8F

#define BUCK_PWR_MD_DISABLED	( 0u << 4 )
#define BUCK_PWR_MD_DYNAMIC		( 1u << 4 )
#define BUCK_PWR_MD_LP			( 2u << 4 )
#define BUCK_PWR_MD_NORMAL		( 3u << 4 )
#define BUCK_PWR_MD_MASK		( 3u << 4 )

// LDO block
#define PMIC_LCNFG              0xF9
#define PMIC_L01_CNFG1          0x90
#define PMIC_L02_CNFG1          0x91
#define PMIC_L03_CNFG1          0x92
#define PMIC_L04_CNFG1          0x93
#define PMIC_L05_CNFG1          0x94
#define PMIC_L06_CNFG1          0x95
#define PMIC_L07_CNFG1          0x96
#define PMIC_L08_CNFG1          0x97
#define PMIC_L09_CNFG1          0x98
#define PMIC_L10_CNFG1          0x99
#define PMIC_L11_CNFG1          0x9A
#define PMIC_L12_CNFG1          0x9B
#define PMIC_L13_CNFG1          0x9C
#define PMIC_L14_CNFG1          0x9D
#define PMIC_L15_CNFG1          0x9E
#define PMIC_L16_CNFG1          0x9F
#define PMIC_L17_CNFG1          0xA1
#define PMIC_L18_CNFG1          0xA2
#define PMIC_L19_CNFG1          0xA3
#define PMIC_L20_CNFG1          0xA4
#define PMIC_L21_CNFG1          0xA5
#define PMIC_L22_CNFG1          0xA6
#define PMIC_L23_CNFG1          0xA7
#define PMIC_L01_CNFG2          0xA8
#define PMIC_L02_CNFG2          0xA9
#define PMIC_L03_CNFG2          0xAA
#define PMIC_L04_CNFG2          0xAB
#define PMIC_L05_CNFG2          0xAC
#define PMIC_L06_CNFG2          0xAD
#define PMIC_L07_CNFG2          0xAE
#define PMIC_L08_CNFG2          0xAF
#define PMIC_L09_CNFG2          0xB0
#define PMIC_L10_CNFG2          0xB1
#define PMIC_L11_CNFG2          0xB2
#define PMIC_L12_CNFG2          0xB3
#define PMIC_L13_CNFG2          0xB4
#define PMIC_L14_CNFG2          0xB5
#define PMIC_L15_CNFG2          0xB6
#define PMIC_L16_CNFG2          0xB7
#define PMIC_L17_CNFG2          0xB8
#define PMIC_L18_CNFG2          0xB9
#define PMIC_L19_CNFG2          0xBA
#define PMIC_L20_CNFG2          0xBB
#define PMIC_L21_CNFG2          0xBC
#define PMIC_L22_CNFG2          0xBD
#define PMIC_L01_08_LVL2_FIQ    0xBE
#define PMIC_L09_16_LVL2_FIQ    0xBF
#define PMIC_L17_23_LVL2_FIQ    0xC0
#define PMIC_L01_08_LVL2_IRQ    0xC1
#define PMIC_L09_16_LVL2_IRQ    0xC2
#define PMIC_L17_23_LVL2_IRQ    0xC3
#define PMIC_L01_08_FIQ_IRQ     0xC4
#define PMIC_L09_16_FIQ_IRQ     0xC5
#define PMIC_L17_23_FIQ_IRQ     0xC6
#define PMIC_L01_08_MASK        0xC7
#define PMIC_L09_16_MASK        0xC8
#define PMIC_L17_23_MASK        0xC9
#define PMIC_LDO_RSVD           0xA0

#define LDO_TV_MASK				( 0x3Fu )
#define LDO_PWR_MD_DISABLED		( 0u << 6 )
#define LDO_PWR_MD_GROUP		( 1u << 6 )
#define LDO_PWR_MD_LP			( 2u << 6 )
#define LDO_PWR_MD_NORMAL		( 3u << 6 )
#define LDO_PWR_MD_MASK			( 3u << 6 )

// WLED block
#define PMIC_LEDBST_CNTRL_1     0xCA
#define PMIC_LEDBYP_CNTL_1      0xCB
#define PMIC_LED_CURRENT_11     0xCC
#define PMIC_LED_CURRENT_12     0xCD
#define PMIC_LED_CURRENT_31     0xCE
#define PMIC_LED_CURRENT_32     0xCF
#define PMIC_LEDBST_IRQ         0xD1
#define PMIC_LEDBST_IRQ_MASK    0xD2

// RGB block
#define PMIC_RLEDSET            0xD5
#define PMIC_GLEDSET            0xD6
#define PMIC_BLEDSET            0xD7
#define PMIC_FLASHRGBON         0xD8
#define PMIC_RGBLED_IRQ_ST      0xD9

#define RLEDSET_RLEDEN			( 1u << 7 )
#define GLEDSET_GLEDEN			( 1u << 7 )
#define BLEDSET_BLEDEN			( 1u << 7 )

// KEYPD block
#define PMIC_KEYLED0            0xE0
#define PMIC_KEYLED1            0xE1
#define PMIC_KEYLED2            0xE2
#define PMIC_KEYLED3            0xE3
#define PMIC_KEYLED4            0xE4
#define PMIC_KEYLED5            0xE5
#define PMIC_KEYLED6            0xE6
#define PMIC_KEYLED7            0xE7
#define PMIC_KEYLED8            0xE8
#define PMIC_KEYLED9            0xE9
#define PMIC_KEYLED10           0xEA
#define PMIC_KEYLED11           0xEB
#define PMIC_KEYLED12           0xEC
#define PMIC_KEYLED13           0xED
#define PMIC_KEYLED14           0xEE
#define PMIC_KEYLED15           0xEF
#define PMIC_KEY_CONFIG         0xF0

// VIBMTR block
#define PMIC_MTRCONFIG          0xF1

#define MTRCONFIG_MEN			( 0x1u << 7 )
#define MTRCONFIG_DTYCYC(x)		( ((x)&0xF) << 3 )
#define MTRCONFIG_DTYCYC_MASK	( 0xFu << 3 )
#define MTRCONFIG_FREQ(x)		( (x) & 0x7 )
#define MTRCONFIG_FREQ_MASK		( 0x7 )

// TCXO block
#define PMIC_TCXO_EN            0xF2
#define PMIC_TCXO_ON_DLY        0xF3
#define PMIC_TCXO_3_5_DRV       0xF4
#define PMIC_TCXO_OUT_POLARITY  0xF5

// CHIPID block
#define PMIC_CHIPID_1           0xFA
#define PMIC_CHIPID_2           0xFB


/**
 * Writes a single byte to the PMIC at the specified address
 *
 * @param addr 7-Bit register address on PMIC
 * @param data 8-Bit data to write to register
 */
void PmicWrite( BYTE addr, BYTE data );

/**
 * Reads a single byte from the PMIC from the specified address
 *
 * @param addr 7-Bit register address on PMIC
 *
 * @return The data located at the specified address
 */
BYTE PmicRead( BYTE addr );

#define PmicSet(x,y)        PmicWrite((x),PmicRead(x)|(y))
#define PmicClear(x,y)      PmicWrite((x),PmicRead(x)&~(y))
#define PmicClearSet(x,m,y) PmicWrite((x),(PmicRead(x)&~(m))|(y))

#endif // __LAGAVULIN_PMIC_H__
