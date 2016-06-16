/*****************************************************************************
 * Filename:    rocket_pmic.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     June 25, 2009
 *
 * Description: Registers and API for TI's Rocket PMIC
 ****************************************************************************/
#ifndef __ROCKET_PMIC_H__
#define __ROCKET_PMIC_H__

// Rocket PowerDie internal register addresses
#define PMIC_EN_CMD0            0x00
#define PMIC_EN_CMD1            0x01
#define PMIC_EN_CMD2            0x02
#define PMIC_MBSTCTRL0          0x03
#define PMIC_MBSTCTRL1          0x04
#define PMIC_MBSTCTRL2          0x05
#define PMIC_ABSTCTRL0          0x06
#define PMIC_ABSTCTRL1          0x07
#define PMIC_PWM_BLU            0x08
#define PMIC_PWM_GRN            0x09
#define PMIC_PWM_RED            0x0A
#define PMIC_RGB_CTRL           0x0B
#define PMIC_PWMCOL3            0x0C
#define PMIC_PWMCOL2            0x0D
#define PMIC_PWMCOL1            0x0E
#define PMIC_PWMCOL0            0x0F
#define PMIC_MATRIXCTRL         0x10
#define PMIC_CTRL_PPCHG0        0x11
#define PMIC_CTRL_PPCHG1        0x12
#define PMIC_CTRL_PPCHG2        0x13
#define PMIC_CTRL_PPCHG3        0x14
#define PMIC_FLASHCTRL0         0x15
#define PMIC_FLASHCTRL1         0x16
#define PMIC_FLASHCTRL2         0x17
#define PMIC_FLASHCTRL3         0x18
#define PMIC_VSM3               0x19
#define PMIC_VSM2               0x1A
#define PMIC_TSM2               0x1B
#define PMIC_VSM1               0x1C
#define PMIC_VSM0               0x1D
#define PMIC_TSM0               0x1E
#define PMIC_BUCKCTRL           0x1F
#define PMIC_DIG_PWM            0x20
#define PMIC_VLDO0              0x21
#define PMIC_VLDO1              0x22
#define PMIC_VLDO2              0x23
#define PMIC_VLDO3              0x24
#define PMIC_VLDO4              0x25
#define PMIC_VLDO5              0x26
#define PMIC_VLDO6              0x27
#define PMIC_VLDO7              0x28
#define PMIC_VLDO8              0x29
#define PMIC_VLDO9              0x2A
#define PMIC_VLDO10             0x2B
#define PMIC_VLDO11             0x2C
#define PMIC_VLDO12             0x2D
#define PMIC_VRTC               0x2E
#define PMIC_ADCIN_SEL          0x2F
//#define PMIC_RSVD               0x30
//#define PMIC_RSVD               0x31
#define PMIC_LDO_MON            0x32
#define PMIC_STAT_PPCHG0        0x33
#define PMIC_STAT_PPCHG1        0x34
#define PMIC_STAT_PPCHG2        0x35
#define PMIC_INTFLAGS0          0x36
#define PMIC_INTFLAGS1          0x37
#define PMIC_INTFLAGS2          0x38
#define PMIC_STAT0              0x39
#define PMIC_STAT1              0x3A
#define PMIC_STAT2              0x3B
#define PMIC_STAT3              0x3C
#define PMIC_STAT4              0x3D
#define PMIC_INTMASK0           0x3E
#define PMIC_INTMASK1           0x3F
#define PMIC_INTMASK2           0x40
#define PMIC_INTMASK3           0x41
#define PMIC_INTMASK4           0x42
#define PMIC_INTMASK5           0x43
#define PMIC_MASKREV            0x44
#define PMIC_OPTIONS            0x45
#define PMIC_OPTIONS_UNLOCK     0x48

// used to mask off reserved flag bits
#define PMIC_INTFLAGS0_MASK     0xFF
#define PMIC_INTFLAGS1_MASK     0xFF
#define PMIC_INTFLAGS2_MASK     0x7F

#define LDOSM_DCHG_EN           (1<<7)
#define LDO_ECO_EN              (1<<6)

#define CMD0_EN_SM0             (1<<0)
#define CMD0_EN_SM1             (1<<1)
#define CMD0_EN_SM2             (1<<2)
#define CMD0_EN_SM3             (1<<3)
#define CMD0_PGDGL              (1<<6)
#define CMD0_EN_ECO             (1<<7)

#define CMD1_EN_LDO0            (1<<0)
#define CMD1_EN_LDO1            (1<<1)
#define CMD1_EN_LDO2            (1<<2)
#define CMD1_EN_LDO3            (1<<3)
#define CMD1_EN_LDO4            (1<<4)
#define CMD1_EN_LDO5            (1<<5)
#define CMD1_EN_LDO6            (1<<6)
#define CMD1_EN_LDO7            (1<<7)

#define CMD2_EN_LDO8            (1<<0)
#define CMD2_EN_LDO9            (1<<1)
#define CMD2_EN_LDO10           (1<<2)
#define CMD2_EN_LDO11           (1<<3)
#define CMD2_EN_LDO12           (1<<4)

#define PMIC_BUCKCTRL_SM0_ECO   (1<<0)
#define PMIC_BUCKCTRL_SM0_FPWM  (1<<1)
#define PMIC_BUCKCTRL_SM1_ECO   (1<<2)
#define PMIC_BUCKCTRL_SM1_FPWM  (1<<3)
#define PMIC_BUCKCTRL_SM2_ECO   (1<<4)
#define PMIC_BUCKCTRL_SM2_FPWM  (1<<5)
#define PMIC_BUCKCTRL_SM3_ECO   (1<<6)
#define PMIC_BUCKCTRL_SM3_FPWM  (1<<7)

#define VRTC_FGLDO_EN           (1<<7)
#define VRTC_CLSEL_MASK         (3<<2)
#define VRTC_CLSEL_1P5          (0<<2)
#define VRTC_CLSEL_6P5          (1<<2)
#define VRTC_CLSEL_7P5          (2<<2)
#define VRTC_CLSEL_12P5         (3<<2)
#define VRTC_VRTC_MASK          (3<<0)
#define VRTC_VRTC_2V2           (0<<0)
#define VRTC_VRTC_2V5           (1<<0)
#define VRTC_VRTC_2V8           (2<<0)
#define VRTC_VRTC_3V1           (3<<0)

    // INTMASK 0 ...
#define PMIC_INTMASK0_BATDET    (1<<0)
#define PMIC_INTMASK0_USBDET    (1<<1)
#define PMIC_INTMASK0_ACDET     (1<<2)
#define PMIC_INTMASK0_PWRSTAT   (1<<3)
#define PMIC_INTMASK0_SYSCHG    (1<<4)
#define PMIC_INTMASK0_BATOV     (1<<5)
#define PMIC_INTMASK0_PWROV     (1<<6)
#define PMIC_INTMASK0_LOWSYS    (1<<7)
    // INTMASK 1 ...
#define PMIC_INTMASK1_RDCECHG   (1<<0)
#define PMIC_INTMASK1_PCHGTMR   (1<<1)
#define PMIC_INTMASK1_CHGTMR    (1<<2)
#define PMIC_INTMASK1_CHGTERM   (1<<3)
#define PMIC_INTMASK1_WDTMR     (1<<4)
#define PMIC_INTMASK1_BATTEMP   (1<<5)
#define PMIC_INTMASK1_TSHUT     (1<<6)

    // INTMASK 2 ...
#define PMIC_INTMASK2_FLEDTMR   (1<<0)
#define PMIC_INTMASK2_MBSTOV    (1<<1)
#define PMIC_INTMASK2_ABSTOV    (1<<2)
#define PMIC_INTMASK2_RGBACT    (1<<3)
#define PMIC_INTMASK2_FTMPWRN   (1<<4)
#define PMIC_INTMASK2_FTMPREG   (1<<5)
#define PMIC_INTMASK2_FLSHSTO   (1<<6)

    // INTMASK 3 ...
#define PMIC_INTMASK3_SM0PG     (1<<0)
#define PMIC_INTMASK3_SM1PG     (1<<1)
#define PMIC_INTMASK3_SM2PG     (1<<2)
#define PMIC_INTMASK3_SM3PG     (1<<3)

    // INTMASK 4 ...
#define PMIC_INTMASK4_LDO0PG    (1<<0)
#define PMIC_INTMASK4_LDO1PG    (1<<1)
#define PMIC_INTMASK4_LDO2PG    (1<<2)
#define PMIC_INTMASK4_LDO3PG    (1<<3)
#define PMIC_INTMASK4_LDO4PG    (1<<4)
#define PMIC_INTMASK4_LDO5PG    (1<<5)
#define PMIC_INTMASK4_LDO6PG    (1<<6)
#define PMIC_INTMASK4_LDO7PG    (1<<7)
    // INTMASK 5 ...
#define PMIC_INTMASK5_LDO8PG    (1<<0)
#define PMIC_INTMASK5_LDO9PG    (1<<1)
#define PMIC_INTMASK5_LDO10PG   (1<<2)
#define PMIC_INTMASK5_LDO11PG   (1<<3)
#define PMIC_INTMASK5_LDO12PG   (1<<4)


#define PMIC_STAT_PPCHG1_USB_PRESENT    (1<<0)
#define PMIC_STAT_PPCHG1_USB_CON        (1<<1)
#define PMIC_STAT_PPCHG1_USB_OVP        (1<<2)
#define PMIC_STAT_PPCHG1_AC_PRESENT     (1<<3)
#define PMIC_STAT_PPCHG1_AC_CON         (1<<4)
#define PMIC_STAT_PPCHG1_AC_OVP         (1<<5)
#define PMIC_STAT_PPCHG1_BAT_PRESENT    (1<<6)
#define PMIC_STAT_PPCHG1_LOWSYS         (1<<7)

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

// Voltage Configuration Defines
enum {
    SM0_0V6000,
    SM0_0V6125,
    SM0_0V6250,
    SM0_0V6375,
    SM0_0V6500,
    SM0_0V6625,
    SM0_0V6750,
    SM0_0V6875,
    SM0_0V7000,
    SM0_0V7125,
    SM0_0V7250,
    SM0_0V7375,
    SM0_0V7500,
    SM0_0V7625,
    SM0_0V7750,
    SM0_0V7875,
    SM0_0V8000,
    SM0_0V8125,
    SM0_0V8250,
    SM0_0V8375,
    SM0_0V8500,
    SM0_0V8625,
    SM0_0V8750,
    SM0_0V8875,
    SM0_0V9000,
    SM0_0V9125,
    SM0_0V9250,
    SM0_0V9375,
    SM0_0V9500,
    SM0_0V9625,
    SM0_0V9750,
    SM0_0V9875,
    SM0_1V0000,
    SM0_1V0125,
    SM0_1V0250,
    SM0_1V0375,
    SM0_1V0500,
    SM0_1V0625,
    SM0_1V0750,
    SM0_1V0875,
    SM0_1V1000,
    SM0_1V1125,
    SM0_1V1250,
    SM0_1V1375,
    SM0_1V1500,
    SM0_1V1625,
    SM0_1V1750,
    SM0_1V1875,
    SM0_1V2000,
    SM0_1V2250,
    SM0_1V2500,
    SM0_1V2750,
    SM0_1V3000,
    SM0_1V3250,
    SM0_1V3500,
    SM0_1V3750,
    SM0_1V4000,
    SM0_1V4250,
    SM0_1V4500,
    SM0_1V4750,
    SM0_1V5000,
    SM0_1V5250,
    SM0_1V5500,
    SM0_1V5750
};

enum {
    SM1_0V800,
    SM1_0V825,
    SM1_0V850,
    SM1_0V875,
    SM1_0V900,
    SM1_0V925,
    SM1_0V950,
    SM1_0V975,
    SM1_1V000,
    SM1_1V025,
    SM1_1V050,
    SM1_1V075,
    SM1_1V100,
    SM1_1V125,
    SM1_1V150,
    SM1_1V175,
    SM1_1V200,
    SM1_1V225,
    SM1_1V250,
    SM1_1V275,
    SM1_1V300,
    SM1_1V325,
    SM1_1V350,
    SM1_1V375,
    SM1_1V400,
    SM1_1V425,
    SM1_1V450,
    SM1_1V475,
    SM1_1V500,
    SM1_1V525,
    SM1_1V550,
    SM1_1V575,
    SM1_1V600,
    SM1_1V650,
    SM1_1V700,
    SM1_1V750,
    SM1_1V800,
    SM1_1V850,
    SM1_1V900,
    SM1_1V950,
    SM1_2V000,
    SM1_2V050,
    SM1_2V100,
    SM1_2V150,
    SM1_2V200,
    SM1_2V250,
    SM1_2V300,
    SM1_2V350,
    SM1_2V400,
    SM1_2V450,
    SM1_2V500,
    SM1_2V550,
    SM1_2V600,
    SM1_2V650,
    SM1_2V700,
    SM1_2V750,
    SM1_2V800,
    SM1_2V850,
    SM1_2V900,
    SM1_2V950,
    SM1_3V000,
    SM1_3V100,
    SM1_3V200,
    SM1_3V300
};

enum {
    SM2_0V500,
    SM2_0V525,
    SM2_0V550,
    SM2_0V575,
    SM2_0V600,
    SM2_0V625,
    SM2_0V650,
    SM2_0V675,
    SM2_0V700,
    SM2_0V725,
    SM2_0V750,
    SM2_0V775,
    SM2_0V800,
    SM2_0V825,
    SM2_0V850,
    SM2_0V875,
    SM2_0V900,
    SM2_0V925,
    SM2_0V950,
    SM2_0V975,
    SM2_1V000,
    SM2_1V025,
    SM2_1V050,
    SM2_1V075,
    SM2_1V100,
    SM2_1V125,
    SM2_1V150,
    SM2_1V175,
    SM2_1V200,
    SM2_1V225,
    SM2_1V250,
    SM2_1V275,
    SM2_1V300,
    SM2_1V325,
    SM2_1V350,
    SM2_1V375,
    SM2_1V400,
    SM2_1V425,
    SM2_1V450,
    SM2_1V475,
    SM2_1V500,
    SM2_1V525,
    SM2_1V550,
    SM2_1V575,
    SM2_1V600,
    SM2_1V625,
    SM2_1V650,
    SM2_1V675,
    SM2_1V800,
    SM2_1V900,
    SM2_2V000,
    SM2_2V100,
    SM2_2V200,
    SM2_2V300,
    SM2_2V400,
    SM2_2V500,
    SM2_2V600,
    SM2_2V700,
    SM2_2V800,
    SM2_2V900,
    SM2_3V000,
    SM2_3V100,
    SM2_3V200,
    SM2_3V300
};

enum {
    SM3_0V800,
    SM3_0V825,
    SM3_0V850,
    SM3_0V875,
    SM3_0V900,
    SM3_0V925,
    SM3_0V950,
    SM3_0V975,
    SM3_1V000,
    SM3_1V025,
    SM3_1V050,
    SM3_1V075,
    SM3_1V100,
    SM3_1V125,
    SM3_1V150,
    SM3_1V175,
    SM3_1V200,
    SM3_1V225,
    SM3_1V250,
    SM3_1V275,
    SM3_1V300,
    SM3_1V325,
    SM3_1V350,
    SM3_1V375,
    SM3_1V400,
    SM3_1V425,
    SM3_1V450,
    SM3_1V475,
    SM3_1V500,
    SM3_1V525,
    SM3_1V550,
    SM3_1V575,
    SM3_1V600,
    SM3_1V650,
    SM3_1V700,
    SM3_1V750,
    SM3_1V800,
    SM3_1V850,
    SM3_1V900,
    SM3_1V950,
    SM3_2V000,
    SM3_2V050,
    SM3_2V100,
    SM3_2V150,
    SM3_2V200,
    SM3_2V250,
    SM3_2V300,
    SM3_2V350,
    SM3_2V400,
    SM3_2V450,
    SM3_2V500,
    SM3_2V550,
    SM3_2V600,
    SM3_2V650,
    SM3_2V700,
    SM3_2V750,
    SM3_2V800,
    SM3_2V850,
    SM3_2V900,
    SM3_2V950,
    SM3_3V000,
    SM3_3V100,
    SM3_3V200,
    SM3_3V300
};

enum {
    LVLDO_0V800,
    LVLDO_0V825,
    LVLDO_0V850,
    LVLDO_0V875,
    LVLDO_0V900,
    LVLDO_0V925,
    LVLDO_0V950,
    LVLDO_0V975,
    LVLDO_1V000,
    LVLDO_1V025,
    LVLDO_1V050,
    LVLDO_1V075,
    LVLDO_1V100,
    LVLDO_1V125,
    LVLDO_1V150,
    LVLDO_1V175,
    LVLDO_1V200,
    LVLDO_1V225,
    LVLDO_1V250,
    LVLDO_1V275,
    LVLDO_1V300,
    LVLDO_1V325,
    LVLDO_1V350,
    LVLDO_1V375,
    LVLDO_1V400,
    LVLDO_1V425,
    LVLDO_1V450,
    LVLDO_1V475,
    LVLDO_1V500,
    LVLDO_1V525,
    LVLDO_1V550,
    LVLDO_1V575
};

enum {
    GPLDO_0V800,
    GPLDO_0V825,
    GPLDO_0V850,
    GPLDO_0V875,
    GPLDO_0V900,
    GPLDO_0V925,
    GPLDO_0V950,
    GPLDO_0V975,
    GPLDO_1V000,
    GPLDO_1V025,
    GPLDO_1V050,
    GPLDO_1V075,
    GPLDO_1V100,
    GPLDO_1V125,
    GPLDO_1V150,
    GPLDO_1V175,
    GPLDO_1V200,
    GPLDO_1V225,
    GPLDO_1V250,
    GPLDO_1V275,
    GPLDO_1V300,
    GPLDO_1V325,
    GPLDO_1V350,
    GPLDO_1V375,
    GPLDO_1V400,
    GPLDO_1V425,
    GPLDO_1V450,
    GPLDO_1V475,
    GPLDO_1V500,
    GPLDO_1V525,
    GPLDO_1V550,
    GPLDO_1V575,
    GPLDO_1V600,
    GPLDO_1V650,
    GPLDO_1V700,
    GPLDO_1V750,
    GPLDO_1V800,
    GPLDO_1V850,
    GPLDO_1V900,
    GPLDO_1V950,
    GPLDO_2V000,
    GPLDO_2V050,
    GPLDO_2V100,
    GPLDO_2V150,
    GPLDO_2V200,
    GPLDO_2V250,
    GPLDO_2V300,
    GPLDO_2V350,
    GPLDO_2V400,
    GPLDO_2V450,
    GPLDO_2V500,
    GPLDO_2V550,
    GPLDO_2V600,
    GPLDO_2V650,
    GPLDO_2V700,
    GPLDO_2V750,
    GPLDO_2V800,
    GPLDO_2V850,
    GPLDO_2V900,
    GPLDO_2V950,
    GPLDO_3V000,
    GPLDO_3V100,
    GPLDO_3V200,
    GPLDO_3V300
};

enum {
    HVLDO_1V80,
    HVLDO_1V85,
    HVLDO_1V90,
    HVLDO_1V95,
    HVLDO_2V00,
    HVLDO_2V05,
    HVLDO_2V10,
    HVLDO_2V15,
    HVLDO_2V20,
    HVLDO_2V25,
    HVLDO_2V30,
    HVLDO_2V35,
    HVLDO_2V40,
    HVLDO_2V45,
    HVLDO_2V50,
    HVLDO_2V55,
    HVLDO_2V60,
    HVLDO_2V65,
    HVLDO_2V70,
    HVLDO_2V75,
    HVLDO_2V80,
    HVLDO_2V85,
    HVLDO_2V90,
    HVLDO_2V95,
    HVLDO_3V00,
    HVLDO_3V05,
    HVLDO_3V10,
    HVLDO_3V15,
    HVLDO_3V20,
    HVLDO_3V25,
    HVLDO_3V30,
    HVLDO_3V35,
};

#endif // __ROCKET_PMIC_H__
