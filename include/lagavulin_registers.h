/*****************************************************************************
 * Filename:    lagavulin_registers.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     May 5, 2009
 *
 * Description: Definitions for lagavulin registers
 ****************************************************************************/
#ifndef __LAGAVULIN_REGISTERS_H__
#define __LAGAVULIN_REGISTERS_H__

/*****************************************************************************
 * Rocket Sub-Device IDs
 *
 * Each module available on rocket has its own sub-device identifier for the
 * host to use when addressing its registers.
 *
 * Each DEVID must have a unique address
 *
 ****************************************************************************/
#define BUGBUILD_ENUM                "RocketDeviceId"
 #define DEVID_ROCKET                0             // RocketRegAddr
 #define DEVID_LIGHTSENSOR           1
 #define DEVID_LED                   2
 #define DEVID_BACKLIGHT             3
 #define DEVID_ADC                   4             // RocketAdcRegAddr
// #define DEVID_UNUSED                5
 #define DEVID_UPDATER               6             // RocketUpdaterRegAddr
 #define DEVID_PMIC                  7             // RocketPmicRegAddr
 #define DEVID_ADC_RAW               8             // RocketAdcRawRegAddr
 #define DEVID_I2C_RELAY             9             // RocketI2cRelayRegAddr
 #define DEVID_GPIO                 10             // RocketGpioRegAddr
 #define DEVID_PWRI2C               11             // RocketPwrI2c
 #define DEVID_FT                   12             // RocketFt
 #define DEVID_PASLUMP              13             // RocketPaSlump
#define DEVID_NUM_DEVICES           14

 #define DEVID_FG                   0xAA
 #define DEVID_FG_ROM               0x16
 #define DEVID_MAX8649              0xC0
#undef BUGBUILD_ENUM              // RocketDeviceId

/*****************************************************************************
 * Rocket Sub-Device Module Addresses
 ****************************************************************************/

/*****************************************************************************
 * DEVID_ROCKET
 *
 * Provides general control and status functionality for Rocket.
 *
 ****************************************************************************/

// ROCKET_REG_VERSION1
// Bits 0-7 (r)  Major Version
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_VERSION1     0x01
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_VERSION2
// Bits 0-7 (r)  Minor Version
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_VERSION2     0x02
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_INTMASK0
// Bits 0-3 (rw) Interrupt Mask Bits
// Bits 4-7      Reserved
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_INTMASK0     0x03
#undef BUGBUILD_ENUM          // RocketRegAddr
  #define ROCKET_INT_PMIC        (1<<0)
  #define ROCKET_INT_ADC         (1<<1)
  #define ROCKET_INT_FG          (1<<2)
  #define ROCKET_INT_PASLUMP     (1<<3)

// ROCKET_REG_INTFLAG0
// Bits 0-1 (rw) Interrupt Flag Bits
// Bits 2-7      Reserved
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_INTFLAG0     0x04
#undef BUGBUILD_ENUM          // RocketRegAddr
 // see ROCKET_REG_INTMASK0 above for bit definitions

// ROCKET_REG_UTCCTRL
// Bit  0   (rw) Utc Disable
// Bits 1-7      Reserved
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_UTCCTRL      0x10
#undef BUGBUILD_ENUM          // RocketRegAddr
 #define ROCKET_UTC_UTCDIS      ( 1<<0 )

// ROCKET_REG_UTCCOUNT0-7
// Bits 0-7 (r)  64 bit value indicating UTC time in ms
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_UTCCOUNT0    0x11
 #define ROCKET_REG_UTCCOUNT1    0x12
 #define ROCKET_REG_UTCCOUNT2    0x13
 #define ROCKET_REG_UTCCOUNT3    0x14
 #define ROCKET_REG_UTCCOUNT4    0x15
 #define ROCKET_REG_UTCCOUNT5    0x16
 #define ROCKET_REG_UTCCOUNT6    0x17
 #define ROCKET_REG_UTCCOUNT7    0x18
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_EVTLOG0
// Bit  0   (w)  Trigger a capture of current eventlog into saved buffer
// Bit  1   (w)  Trigger saved buffer index to be reset to start of log
// Bit  2   (r)  Indicates if an eventlog exists in the saved buffer or not
// Bits 3-7      Reserved
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_EVTLOG0      0x19
#undef BUGBUILD_ENUM          // RocketRegAddr
  #define ROCKET_EVTLOG0_CAPTURE ( 1<<0 )
  #define ROCKET_EVTLOG0_REWIND  ( 1<<1 )
  #define ROCKET_EVTLOG0_EXISTS  ( 1<<2 )

// ROCKET_REG_EVTLOG0_LOG
// Bits 0-7 (r)  Repeated reads of this register return saved eventlog
//               contents and auto-incs to next byte in log after each read.
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_EVTLOG0_LOG  0x1A
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_RESETCTRL
// Bit  0   (w)  Trigger an MSP430 watchdog reset. (for testing)
// Bit  1   (w)  Trigger a call to RimInitiateReset(). (for testing)
// Bit  2   (rw) Enable KEYVOL reset functionality if available
// Bit  3   (w)  Trigger a powerdown sequence and enter "DEEPSLEEP" waiting for an external powersource to be applied
// Bit  4   (w)  Trigger a fast reset where LCD state retention is possible
// Bit  5   (w)  Notify rocket that host performed a WARM reset, so rocket must
//               re-init all its states
// Bits 5-7      Reserved
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_RESETCTRL    0x1B
#undef BUGBUILD_ENUM          // RocketRegAddr
  #define ROCKET_RESETCTRL_WATCHDOG  ( 1<<0 )
  #define ROCKET_RESETCTRL_DORESET   ( 1<<1 )
  #define ROCKET_RESETCTRL_KEYVOL_EN ( 1<<2 )
  #define ROCKET_RESETCTRL_DEEPSLEEP ( 1<<3 )
  #define ROCKET_RESETCTRL_FASTRST   ( 1<<4 )
  #define ROCKET_RESETCTRL_WARMRST   ( 1<<5 )

// ROCKET_REG_RESETSTAT
// Bits 0-7 (r)  Code indicating the reason for host power loss or reset.
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_RESETSTAT    0x1C
#undef BUGBUILD_ENUM          // RocketRegAddr
#define BUGBUILD_ENUM            "RocketResetStat"
  // ROCKET_RESET_NORTC - Indicates that a fresh MSP430 powerup occured.  This
  // occurs when power is applied and the supercap was fully drained.  No
  // previous state is known.
  #define ROCKET_RESET_NORTC     1
  // ROCKET_RESET_HOST - Indicates that the host reset was initiated by the
  // Host processor (either i2c initiated or GPIO)
  #define ROCKET_RESET_HOST      2
  // ROCKET_RESET_430DOG - Indicates that the MSP430 reset due to its internal
  // watchdog or some other hardware cause.  The contents of the MSP430's
  // SYSRSTIV register is available via the ROCKET_REG_RESETEXTRA register.
  #define ROCKET_RESET_430DOG    3
  // ROCKET_RESET_INITIATED - Indicates that RimInitiateReset() was called.
  #define ROCKET_RESET_INITIATED 4
  // ROCKET_RESET_430ERROR - Indicates that the MSP430 reset due to a firmware
  // sofware error
  #define ROCKET_RESET_430ERROR  5
  // ROCKET_RESET_SUPERCAP - Indicates that the MSP430 reset after running off
  // the supercap
  #define ROCKET_RESET_SUPERCAP  6
  // ROCKET_RESET_KEYVOL - Indicates that the MSP430 reset due to the volume
  // key reset feature
  #define ROCKET_RESET_KEYVOL    7
  // ROCKET_RESET_UPDATER - Indicates that the MSP430 reset due to the
  // BOOT code update sequence
  #define ROCKET_RESET_UPDATER   8
  // ROCKET_RESET_DEEPSLEEP - Indicates the host was powered down into deep sleep
  // at the request of the host
  #define ROCKET_RESET_DEEPSLEEP 9
  // ROCKET_RESET_WARMRST - Indicates the last reset power was maintained to memory/cpu/lcd
  #define ROCKET_RESET_WARMRST   10
  // ROCKET_RESET_PGFAULT - Indicates rockets supplie(s) browned out
  #define ROCKET_RESET_PGFAULT   11
  // ROCKET_RESET_BRICKWAKE - Indicates rocket reset after being woken from BRICK mode
  #define ROCKET_RESET_BRICKWAKE 12
#undef BUGBUILD_ENUM  // RocketResetStat


// ROCKET_REG_RESETEXTRA
// Bits 0-7 (r)  Extra info about a reset cause depending on the reason as
// indicated in ROCKET_REG_RESETSTAT.
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_RESETEXTRA   0x1D
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_LOADER_VERSION1
// Bits 0-7 (r)  Major Version
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_LOADER_VERSION1     0x1E
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_LOADER_VERSION2
// Bits 0-7 (r)  Minor Version
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_LOADER_VERSION2     0x1F
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_HWV_HARDWAREID0-3
// Bits 0-7 (r)  HWV HARDWAREID #
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_HWV_HARDWAREID0     0x20
 #define ROCKET_REG_HWV_HARDWAREID1     0x21
 #define ROCKET_REG_HWV_HARDWAREID2     0x22
 #define ROCKET_REG_HWV_HARDWAREID3     0x23
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_HWV_CLASS_PWRSEQ
// Bits 0-7 (r)  HWV Class PWRSEQ compat number
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_HWV_CLASS_PWRSEQ    0x24
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_BUILDINFO1
// Bits 0-7 (r)  MSByte of build info
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_BUILDINFO1          0x25
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_BUILDINFO2
// Bits 0-7 (r)  LSByte of build info
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_BUILDINFO2          0x26
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_LOADER_BUILDINFO1
// Bits 0-7 (r)  MSByte of build info
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_LOADER_BUILDINFO1   0x27
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_LOADER_BUILDINFO2
// Bits 0-7 (r)  LSByte of build info
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_LOADER_BUILDINFO2   0x28
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_API_VERSION_MAJOR
// Bits 0-7 (r)  Major API Version number
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_API_VERSION_MAJOR   0x29
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_API_VERSION_MINOR
// Bits 0-7 (r)  Minor API Version number
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_API_VERSION_MINOR   0x2A
#undef BUGBUILD_ENUM          // RocketRegAddr

// ROCKET_REG_SYSFLAGS0
// Bit  0 (rw)   BONSAI_ACTIVE
// Bit  1 (rw)   ALLOW_AUD_PROG
// Bit  2 (rw)   FGROM_400K_EN
// Bit  3 (rw)   WARM_RESET
// Bit  4 (rw)   LCD_RET_EN
// Bits 2-7      Reserved
#define BUGBUILD_ENUM           "RocketRegAddr"
 #define ROCKET_REG_SYSFLAGS0           0x2B
#undef BUGBUILD_ENUM          // RocketRegAddr
 #define ROCKET_SYSFLAGS0_BONSAI_ACTIVE    ( 1<<0 )
 #define ROCKET_SYSFLAGS0_ALLOW_AUD_PROG   ( 1<<1 )
 #define ROCKET_SYSFLAGS0_FGROM_400K_EN    ( 1<<2 )
 #define ROCKET_SYSFLAGS0_WARM_RESET       ( 1<<3 )
 #define ROCKET_SYSFLAGS0_LCD_RET_EN       ( 1<<4 )


/*****************************************************************************
 * DEVID_LIGHTSENSOR
 *
 * Provides control of lightsensor functionality
 *
 ****************************************************************************/

// TODO

/*****************************************************************************
 * DEVID_LED
 *
 * Provides control of LED functionality
 *
 ****************************************************************************/

// LED_REG_CONFIG0
// Bits 0-7      Reserved
#define LED_REG_CONFIG0                 ( 0x00 )
  #define LED_CONFIG0_ENABLE            ( 1<<0 )

// LED_REG_STATUS_PATTERN
// Bits 0-7      Reserved
#define LED_REG_STATUS_PATTERN          ( 0x01 )

/*****************************************************************************
 * DEVID_BACKLIGHT
 *
 * Provides control of backlight functionality
 *
 ****************************************************************************/

// TODO

/*****************************************************************************
 * DEVID_ADC
 *
 * Provides control over ADC engines available for use by host
 *
 ****************************************************************************/

// ADC_ENGINEx
// Identifier for specific ADC engine available on rocket
#define BUGBUILD_ENUM              "RocketAdcEngineId"
 #define ADC_ENGINE0                0
 #define ADC_ENGINE1                1
 #define ADC_ENGINE2                2
// #define ADC_ENGINE3                3
// #define ADC_ENGINE4                4
// #define ADC_ENGINE5                5
// #define ADC_ENGINE6                6
// #define ADC_ENGINE7                7
 #define ADC_NUM_ENGINES            3
#undef BUGBUILD_ENUM             // RocketAdcEngineId


// ADC_REG_CONFIG0(x)
// Bit  0   (rw) Enables/disables the ADC engine; writing 0 in the middle of
//               an ADC reading will stop it immediatly and discard any results
// Bit  1   (w)  Trigger the ADC engine to start
// Bits 2-6 (rw) Specifies ADC channel being measured by engine
// Bit  7        Reserved
#define ADC_REG_CONFIG0(x)              ( 0x00 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_CONFIG0          0x00
 #define ADC_REG_ENGN1_CONFIG0          0x20
 #define ADC_REG_ENGN2_CONFIG0          0x40
 #define ADC_REG_ENGN3_CONFIG0          0x60
 #define ADC_REG_ENGN4_CONFIG0          0x80
 #define ADC_REG_ENGN5_CONFIG0          0xA0
 #define ADC_REG_ENGN6_CONFIG0          0xC0
 #define ADC_REG_ENGN7_CONFIG0          0xE0
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
  #define ADC_CONFIG0_ENABLE            ( 1<<0 )
  #define ADC_CONFIG0_START             ( 1<<1 )
  #define ADC_CONFIG0_CHANNEL(x)        ( (x)<<2 )
  #define ADC_CONFIG0_CHANNEL_MASK      ( ADC_CONFIG0_CHANNEL(0x1F) )

#define BUGBUILD_ENUM                  "LagavulinAdcRegChannel"
  #define ADC_CHANNEL_BYP               0x0
  #define ADC_CHANNEL_TDIE              0x1
  #define ADC_CHANNEL_VBAT              0x2
  #define ADC_CHANNEL_VSYS              0x3
  #define ADC_CHANNEL_VCHGINA           0x4
  #define ADC_CHANNEL_VCHGINB           0x5
  #define ADC_CHANNEL_IMON              0x6
  #define ADC_CHANNEL_FLED1             0x7
  #define ADC_CHANNEL_FLED2             0x8
  #define ADC_CHANNEL_ICHG              0x9
  #define ADC_CHANNEL_A1                0xA
  #define ADC_CHANNEL_A2                0xB
  #define ADC_CHANNEL_A3                0xC
  #define ADC_CHANNEL_A4                0xD
  #define ADC_CHANNEL_A5                0xE
  #define ADC_CHANNEL_A6                0xF
  #define ADC_CHANNEL_A7                0x10
  #define ADC_CHANNEL_A8                0x11
  #define ADC_CHANNEL_MAX               0x12
#undef BUGBUILD_ENUM                 // LagavulinAdcRegChannel

// ADC_REG_CONFIG1(x)
// Bit  0   (rw) ADC sample complete interrupt enable
// Bit  1   (rw) Minimum voltage threshold exceeded interrupt enable
// Bit  2   (rw) Maximum voltage threshold exceeded interrupt enable
// Bit  3   (rw) Minimum voltage threshold2 exceeded interrupt enable
// Bit  4   (rw) Maximum voltage threshold2 exceeded interrupt enable
// BitS 5-7      Reserved
#define ADC_REG_CONFIG1(x)              ( 0x01 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_CONFIG1          0x01
 #define ADC_REG_ENGN1_CONFIG1          0x21
 #define ADC_REG_ENGN2_CONFIG1          0x41
 #define ADC_REG_ENGN3_CONFIG1          0x61
 #define ADC_REG_ENGN4_CONFIG1          0x81
 #define ADC_REG_ENGN5_CONFIG1          0xA1
 #define ADC_REG_ENGN6_CONFIG1          0xC1
 #define ADC_REG_ENGN7_CONFIG1          0xE1
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
  #define ADC_CONFIG1_ADCIE              ( 1<<0 )
  #define ADC_CONFIG1_MINIE              ( 1<<1 )
  #define ADC_CONFIG1_MAXIE              ( 1<<2 )


// ADC_REG_CONFIG2(x)
// Bit  0   (rw) ADC sample inhibit/trigger enable (inhibit signal depends on channel
//               selected)
// Bit  1   (rw) ADC sampling it triggered to start by inhibit signal
// Bits 2-4 (rw) ADC sample mode
// Bits 5-7 (rw) Reserved
#define ADC_REG_CONFIG2(x)              ( 0x02 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_CONFIG2          0x02
 #define ADC_REG_ENGN1_CONFIG2          0x22
 #define ADC_REG_ENGN2_CONFIG2          0x42
 #define ADC_REG_ENGN3_CONFIG2          0x62
 #define ADC_REG_ENGN4_CONFIG2          0x82
 #define ADC_REG_ENGN5_CONFIG2          0xA2
 #define ADC_REG_ENGN6_CONFIG2          0xC2
 #define ADC_REG_ENGN7_CONFIG2          0xE2
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
  #define ADC_CONFIG2_INHIBIT_EN         ( 1<<0 )
  #define ADC_CONFIG2_TRIGGER_MODE       ( 1<<1 )
  #define ADC_CONFIG2_MODE_MASK          ( 7<<2 )
  // ADC_CONFIG2_MODE_AVERAGE - ADC result register contains average of
  //     ADC readings taken over the span of the measurements
  #define ADC_CONFIG2_MODE_AVERAGE       ( 0<<2 )
  // ADC_CONFIG2_MODE_MIN - ADC result register contains minimum reading
  //     taken over the span of the measurements
  #define ADC_CONFIG2_MODE_MIN           ( 1<<2 )
  // ADC_CONFIG2_MODE_MAX - ADC result register contains maximum reading
  //     taken over the span of the measurements
  #define ADC_CONFIG2_MODE_MAX           ( 2<<2 )
  // ADC_CONFIG2_MODE_SINGLE - ADC result register contains the single
  //     reading taken
  #define ADC_CONFIG2_MODE_SINGLE        ( 3<<2 )
  // ADC_CONFIG2_MODE_RANGE - ADC result register contains the reading
  //     which 'tripped' the threshold. This mode continuously samples
  //     at the specified WAIT period until +/- threshold is exceeded
  #define ADC_CONFIG2_MODE_RANGE         ( 4<<2 )
  // ADC_CONFIG2_MODE_RANGE_AVG - This mode is identical to RANGE except
  //     uses a moving average reading instead of instantaneous reading.
  #define ADC_CONFIG2_MODE_RANGE_AVG     ( 5<<2 )


// ADC_REG_MUXCHANNEL(x)
// Bits 0-7 (rw) Internal analog mux channel selection (only used if
//               ADC_CONFIG0_CHANNEL_MUX is used)
#define ADC_REG_MUXCHANNEL(x)           ( 0x03 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_MUXCHANNEL       0x03
 #define ADC_REG_ENGN1_MUXCHANNEL       0x23
 #define ADC_REG_ENGN2_MUXCHANNEL       0x43
 #define ADC_REG_ENGN3_MUXCHANNEL       0x63
 #define ADC_REG_ENGN4_MUXCHANNEL       0x83
 #define ADC_REG_ENGN5_MUXCHANNEL       0xA3
 #define ADC_REG_ENGN6_MUXCHANNEL       0xC3
 #define ADC_REG_ENGN7_MUXCHANNEL       0xE3
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

// ADC_REG_STATUS0(x)
// Bit  0   (r) ADC sample complete interrupt flag
// Bit  1   (r) ADC minimum voltage threshold exceeded interrupt flag
// Bit  2   (r) ADC maximum voltage threshold exceeded interrupt flag
// Bit  3   (r) ADC minimum voltage threshold2 exceeded interrupt flag
// Bit  4   (r) ADC maximum voltage threshold2 exceeded interrupt flag
// Bits 5-6     Reserved
// Bit  7   (r) ADC Engine in use flag
#define ADC_REG_STATUS0(x)              ( 0x04 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_STATUS0          0x04
 #define ADC_REG_ENGN1_STATUS0          0x24
 #define ADC_REG_ENGN2_STATUS0          0x44
 #define ADC_REG_ENGN3_STATUS0          0x64
 #define ADC_REG_ENGN4_STATUS0          0x84
 #define ADC_REG_ENGN5_STATUS0          0xA4
 #define ADC_REG_ENGN6_STATUS0          0xC4
 #define ADC_REG_ENGN7_STATUS0          0xE4
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
 #define ADC_STATUS0_ADCIFG             ( 1<<0 )
 #define ADC_STATUS0_MINIFG             ( 1<<1 )
 #define ADC_STATUS0_MAXIFG             ( 1<<2 )
 #define ADC_STATUS0_BUSY               ( 1<<7 )

// ADC_REG_NUM_SAMPLES(x)
// Bits 0-7 (rw) Number of samples to take
//               SamplesTaken = (2^ADC_REG_NUM_SAMPLES)
#define ADC_REG_NUM_SAMPLES(x)          ( 0x05 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_NUM_SAMPLES      0x05
 #define ADC_REG_ENGN1_NUM_SAMPLES      0x25
 #define ADC_REG_ENGN2_NUM_SAMPLES      0x45
 #define ADC_REG_ENGN3_NUM_SAMPLES      0x65
 #define ADC_REG_ENGN4_NUM_SAMPLES      0x85
 #define ADC_REG_ENGN5_NUM_SAMPLES      0xA5
 #define ADC_REG_ENGN6_NUM_SAMPLES      0xC5
 #define ADC_REG_ENGN7_NUM_SAMPLES      0xE5
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

// ADC_REG_WAIT_TIME_xSB(x)
// Bits 0-7 (rw) Number of mS to wait between samples
#define ADC_REG_WAIT_TIME_MSB(x)        ( 0x06 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_WAIT_TIME_MSB    0x06
 #define ADC_REG_ENGN1_WAIT_TIME_MSB    0x26
 #define ADC_REG_ENGN2_WAIT_TIME_MSB    0x46
 #define ADC_REG_ENGN3_WAIT_TIME_MSB    0x66
 #define ADC_REG_ENGN4_WAIT_TIME_MSB    0x86
 #define ADC_REG_ENGN5_WAIT_TIME_MSB    0xA6
 #define ADC_REG_ENGN6_WAIT_TIME_MSB    0xC6
 #define ADC_REG_ENGN7_WAIT_TIME_MSB    0xE6
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
#define ADC_REG_WAIT_TIME_LSB(x)        ( 0x07 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_WAIT_TIME_LSB    0x07
 #define ADC_REG_ENGN1_WAIT_TIME_LSB    0x27
 #define ADC_REG_ENGN2_WAIT_TIME_LSB    0x47
 #define ADC_REG_ENGN3_WAIT_TIME_LSB    0x67
 #define ADC_REG_ENGN4_WAIT_TIME_LSB    0x87
 #define ADC_REG_ENGN5_WAIT_TIME_LSB    0xA7
 #define ADC_REG_ENGN6_WAIT_TIME_LSB    0xC7
 #define ADC_REG_ENGN7_WAIT_TIME_LSB    0xE7
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

// ADC_REG_DELAY_TIME_xSB(x)
// Bits 0-7 (rw) Number of mS to delay before first sample is taken
//               once engine is started
#define ADC_REG_DELAY_TIME_MSB(x)       ( 0x08 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_DELAY_TIME_MSB   0x08
 #define ADC_REG_ENGN1_DELAY_TIME_MSB   0x28
 #define ADC_REG_ENGN2_DELAY_TIME_MSB   0x48
 #define ADC_REG_ENGN3_DELAY_TIME_MSB   0x68
 #define ADC_REG_ENGN4_DELAY_TIME_MSB   0x88
 #define ADC_REG_ENGN5_DELAY_TIME_MSB   0xA8
 #define ADC_REG_ENGN6_DELAY_TIME_MSB   0xC8
 #define ADC_REG_ENGN7_DELAY_TIME_MSB   0xE8
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
#define ADC_REG_DELAY_TIME_LSB(x)       ( 0x09 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_DELAY_TIME_LSB   0x09
 #define ADC_REG_ENGN1_DELAY_TIME_LSB   0x29
 #define ADC_REG_ENGN2_DELAY_TIME_LSB   0x49
 #define ADC_REG_ENGN3_DELAY_TIME_LSB   0x69
 #define ADC_REG_ENGN4_DELAY_TIME_LSB   0x89
 #define ADC_REG_ENGN5_DELAY_TIME_LSB   0xA9
 #define ADC_REG_ENGN6_DELAY_TIME_LSB   0xC9
 #define ADC_REG_ENGN7_DELAY_TIME_LSB   0xE9
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

// ADC_REG_RESULT_xSB(x)
// Bits 0-7 (r)  ADC Conversion result
//               Reading MSB auto clears the any pending interrupt
//               for that engine
#define ADC_REG_RESULT_MSB(x)           ( 0x0A + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_RESULT_MSB       0x0A
 #define ADC_REG_ENGN1_RESULT_MSB       0x2A
 #define ADC_REG_ENGN2_RESULT_MSB       0x4A
 #define ADC_REG_ENGN3_RESULT_MSB       0x6A
 #define ADC_REG_ENGN4_RESULT_MSB       0x8A
 #define ADC_REG_ENGN5_RESULT_MSB       0xAA
 #define ADC_REG_ENGN6_RESULT_MSB       0xCA
 #define ADC_REG_ENGN7_RESULT_MSB       0xEA
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
#define ADC_REG_RESULT_LSB(x)           ( 0x0B + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_RESULT_LSB       0x0B
 #define ADC_REG_ENGN1_RESULT_LSB       0x2B
 #define ADC_REG_ENGN2_RESULT_LSB       0x4B
 #define ADC_REG_ENGN3_RESULT_LSB       0x6B
 #define ADC_REG_ENGN4_RESULT_LSB       0x8B
 #define ADC_REG_ENGN5_RESULT_LSB       0xAB
 #define ADC_REG_ENGN6_RESULT_LSB       0xCB
 #define ADC_REG_ENGN7_RESULT_LSB       0xEB
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

// ADC_REG_MAX_THRESH_xSB(x)
// Bits 0-7 (rw) ADC maximum threshold level
#define ADC_REG_MAX_THRESH_MSB(x)       ( 0x0C + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_MAX_THRESH_MSB   0x0C
 #define ADC_REG_ENGN1_MAX_THRESH_MSB   0x2C
 #define ADC_REG_ENGN2_MAX_THRESH_MSB   0x4C
 #define ADC_REG_ENGN3_MAX_THRESH_MSB   0x6C
 #define ADC_REG_ENGN4_MAX_THRESH_MSB   0x8C
 #define ADC_REG_ENGN5_MAX_THRESH_MSB   0xAC
 #define ADC_REG_ENGN6_MAX_THRESH_MSB   0xCC
 #define ADC_REG_ENGN7_MAX_THRESH_MSB   0xEC
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
#define ADC_REG_MAX_THRESH_LSB(x)       ( 0x0D + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_MAX_THRESH_LSB   0x0D
 #define ADC_REG_ENGN1_MAX_THRESH_LSB   0x2D
 #define ADC_REG_ENGN2_MAX_THRESH_LSB   0x4D
 #define ADC_REG_ENGN3_MAX_THRESH_LSB   0x6D
 #define ADC_REG_ENGN4_MAX_THRESH_LSB   0x8D
 #define ADC_REG_ENGN5_MAX_THRESH_LSB   0xAD
 #define ADC_REG_ENGN6_MAX_THRESH_LSB   0xCD
 #define ADC_REG_ENGN7_MAX_THRESH_LSB   0xED
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

// ADC_REG_MIN_THRESH_xSB(x)
// Bits 0-7 (rw) ADC minimum threshold level
#define ADC_REG_MIN_THRESH_MSB(x)       ( 0x0E + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_MIN_THRESH_MSB   0x0E
 #define ADC_REG_ENGN1_MIN_THRESH_MSB   0x2E
 #define ADC_REG_ENGN2_MIN_THRESH_MSB   0x4E
 #define ADC_REG_ENGN3_MIN_THRESH_MSB   0x6E
 #define ADC_REG_ENGN4_MIN_THRESH_MSB   0x8E
 #define ADC_REG_ENGN5_MIN_THRESH_MSB   0xAE
 #define ADC_REG_ENGN6_MIN_THRESH_MSB   0xCE
 #define ADC_REG_ENGN7_MIN_THRESH_MSB   0xEE
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
#define ADC_REG_MIN_THRESH_LSB(x)       ( 0x0F + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_MIN_THRESH_LSB   0x0F
 #define ADC_REG_ENGN1_MIN_THRESH_LSB   0x2F
 #define ADC_REG_ENGN2_MIN_THRESH_LSB   0x4F
 #define ADC_REG_ENGN3_MIN_THRESH_LSB   0x6F
 #define ADC_REG_ENGN4_MIN_THRESH_LSB   0x8F
 #define ADC_REG_ENGN5_MIN_THRESH_LSB   0xAF
 #define ADC_REG_ENGN6_MIN_THRESH_LSB   0xCF
 #define ADC_REG_ENGN7_MIN_THRESH_LSB   0xEF
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

// ADC_REG_LASTREADING_xSB(x)
// Bits 0-7 (r)  ADC Conversion result for last measurement taken
#define ADC_REG_LASTREADING_MSB(x)      ( 0x10 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_LASTREADING_MSB  0x10
 #define ADC_REG_ENGN1_LASTREADING_MSB  0x30
 #define ADC_REG_ENGN2_LASTREADING_MSB  0x50
 #define ADC_REG_ENGN3_LASTREADING_MSB  0x70
 #define ADC_REG_ENGN4_LASTREADING_MSB  0x90
 #define ADC_REG_ENGN5_LASTREADING_MSB  0xB0
 #define ADC_REG_ENGN6_LASTREADING_MSB  0xD0
 #define ADC_REG_ENGN7_LASTREADING_MSB  0xF0
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr
#define ADC_REG_LASTREADING_LSB(x)      ( 0x11 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_LASTREADING_LSB  0x11
 #define ADC_REG_ENGN1_LASTREADING_LSB  0x31
 #define ADC_REG_ENGN2_LASTREADING_LSB  0x51
 #define ADC_REG_ENGN3_LASTREADING_LSB  0x71
 #define ADC_REG_ENGN4_LASTREADING_LSB  0x91
 #define ADC_REG_ENGN5_LASTREADING_LSB  0xB1
 #define ADC_REG_ENGN6_LASTREADING_LSB  0xD1
 #define ADC_REG_ENGN7_LASTREADING_LSB  0xF1
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

// ADC_REG_STATE(x)
// Bits 0-7 (r)  ADC state machine value
#define ADC_REG_STATE(x)               ( 0x12 + ((x)<<5) )
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_REG_ENGN0_STATE            0x12
 #define ADC_REG_ENGN1_STATE            0x32
 #define ADC_REG_ENGN2_STATE            0x52
 #define ADC_REG_ENGN3_STATE            0x72
 #define ADC_REG_ENGN4_STATE            0x92
 #define ADC_REG_ENGN5_STATE            0xB2
 #define ADC_REG_ENGN6_STATE            0xD2
 #define ADC_REG_ENGN7_STATE            0xF2
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

// ADC_REG_DEBUGn
// Bits 0-7 (r)  ADC debug registers
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
#define ADC_REG_DEBUG1                  0x13
#define ADC_REG_DEBUG2                  0x14
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

/*****************************************************************************
 * DEVID_ADC_RAW
 *
 * Provides control over basic ADC HW for raw measurements (used by bootrom)
 *
 ****************************************************************************/


// ADC_RAW_REG_CONFIG0(x)
// Bit  0   (r)  Gets current state of ADC conversion ( BUSY = 1 )
//               result should not be read until this is cleared from
//               previous triggered sample
// Bit  1   (w)  Trigger the ADC engine to start (always reads 0)
// Bits 2-6 (rw) Specifies ADC channel being measured by engine
// Bit  7        Reserved
#define BUGBUILD_ENUM                  "RocketAdcRawRegAddr"
 #define ADC_RAW_REG_CONFIG0            0x00
#undef BUGBUILD_ENUM                 // RocketAdcRawRegAddr
  #define ADC_RAW_CONFIG0_BUSY              ( 1<<0 )
  #define ADC_RAW_CONFIG0_START             ( 1<<1 )
  #define ADC_RAW_CONFIG0_CHANNEL(x)        ( (x)<<2 )
  #define ADC_RAW_CONFIG0_CHANNEL_MASK      ( ADC_RAW_CONFIG0_CHANNEL(0x1F) )

// ADC_RAW_REG_MUXCHANNEL(x)
// Bits 0-7 (rw) Internal analog mux channel selection (only used if
//               ADC_CONFIG0_CHANNEL_MUX is used)
#define BUGBUILD_ENUM                  "RocketAdcRawRegAddr"
 #define ADC_RAW_REG_MUXCHANNEL         0x01
#undef BUGBUILD_ENUM                 // RocketAdcRawRegAddr

// ADC_RAW_REG_RESULT_xSB(x)
// Bits 0-7 (r)  ADC Conversion result
#define BUGBUILD_ENUM                  "RocketAdcRegAddr"
 #define ADC_RAW_REG_RESULT_MSB         0x02
 #define ADC_RAW_REG_RESULT_LSB         0x03
#undef BUGBUILD_ENUM                 // RocketAdcRegAddr

/*****************************************************************************
 * DEVID_UPDATER
 *
 * Provides interface for host to request informatin about current rocket
 * firmware, and update it
 ****************************************************************************/

// UPDATER_REG_KEY1
// Bits 0-7 (w)  Write access key1
#define BUGBUILD_ENUM           "RocketUpdaterRegAddr"
 #define UPDATER_REG_KEY1       0x00
#undef BUGBUILD_ENUM          // RocketUpdaterRegAddr
#define UPDATER_VALID_KEY1      0xAA

// UPDATER_REG_KEY2
// Bits 0-7 (w)  Write access key2
#define BUGBUILD_ENUM           "RocketUpdaterRegAddr"
 #define UPDATER_REG_KEY2       0x01
#undef BUGBUILD_ENUM          // RocketUpdaterRegAddr
#define UPDATER_VALID_KEY2      0x55

// UPDATER_REG_COMMAND
// Bits 0-7 (w)  Command register to write bootloader instructions/data to
#define BUGBUILD_ENUM           "RocketUpdaterRegAddr"
 #define UPDATER_REG_COMMAND    0x02
#undef BUGBUILD_ENUM          // RocketUpdaterRegAddr

// UPDATER_REG_RESPONSE
// Bits 0-7 (r)  Response from bootloader for last instruction run
#define BUGBUILD_ENUM           "RocketUpdaterRegAddr"
 #define UPDATER_REG_RESPONSE   0x03
#undef BUGBUILD_ENUM          // RocketUpdaterRegAddr

// UPDATER_REG_STATUS
// Bit  0   (r)  App code present
// Bits 1-7      Reserved
#define BUGBUILD_ENUM           "RocketUpdaterRegAddr"
 #define UPDATER_REG_STATUS     0x04
#undef BUGBUILD_ENUM          // RocketUpdaterRegAddr
#define UPDATER_APP_PRESENT     ( 1<<0 )

/*****************************************************************************
 * DEVID_I2C_RELAY
 *
 * Provides interface for host to request i2c transactions to be relayed
 * to devices connected to Rocket
 ****************************************************************************/


// I2CRLY_REG_STATE
// Bits 0-7 (r)  Indicates the current state of the relay device
#define BUGBUILD_ENUM           "RocketI2cRelayRegAddr"
 #define I2CRLY_REG_STATE       0x00
#undef BUGBUILD_ENUM          // RocketI2cRelayRegAddr
#define BUGBUILD_ENUM           "RocketI2cRelayState"
 #define I2CRLY_STATE_IDLE       0
 #define I2CRLY_STATE_BUSY       1
 #define I2CRLY_STATE_SUCCESS    2
 #define I2CRLY_STATE_FAILURE    3
 #define I2CRLY_STATE_CMDERR     4
#undef BUGBUILD_ENUM          // RocketI2cRelayState"

// I2CRLY_REG_WRLEN
// Bits 0-7 (w)  Number of bytes to write from the command buffer
#define BUGBUILD_ENUM           "RocketI2cRelayRegAddr"
 #define I2CRLY_REG_WRLEN       0x3D
#undef BUGBUILD_ENUM          // RocketI2cRelayRegAddr

// I2CRLY_REG_RDLEN
// Bits 0-7 (w)  Number of bytes to read into the command buffer
#define BUGBUILD_ENUM           "RocketI2cRelayRegAddr"
 #define I2CRLY_REG_RDLEN       0x3E
#undef BUGBUILD_ENUM          // RocketI2cRelayRegAddr

// I2CRLY_REG_SLAVEADDR
// Bits 0-7 (w)  Slave address of device being written/read to/from
#define BUGBUILD_ENUM           "RocketI2cRelayRegAddr"
 #define I2CRLY_REG_SLAVEADDR   0x3F
#undef BUGBUILD_ENUM          // RocketI2cRelayRegAddr

// I2CRLY_REG_CMDBUFFER
// Bits 0-7 (r)  Command/result buffer (64 Bytes)
#define BUGBUILD_ENUM           "RocketI2cRelayRegAddr"
 #define I2CRLY_REG_CMDBUFFER   0x40
#undef BUGBUILD_ENUM          // RocketI2cRelayRegAddr

/*****************************************************************************
 * DEVID_GPIO
 *
 * Provides direct access to rocket GPIO states
 *
 ****************************************************************************/

// GPIO_REG_LINE
// Bits 0-7  Specifies which GPIO on rocket is being modified/read
#define BUGBUILD_ENUM           "RocketGpioRegAddr"
 #define GPIO_REG_LINE        0x00
#undef BUGBUILD_ENUM          // RocketGpioRegAddr
// Utility macro to generate the specific BYTE to control a GPIO on rocket
// EX: for port 5.1  use   ROCKET_PAD_PORT(5,1)
// one gotcha: for ports J.X use   ROCKET_PAD_PORT(19,X)
#define ROCKET_GPIO_PORT( P, N )    ( ((P)-1)<<3 | (N) )

// GPIO_REG_CONTROL
// Bit  0   (r/w)    I/O Level State (1=High, 0=Low)
// Bit  1   (r/w)    I/O Direction State (1=Output, 0=Input)
#define BUGBUILD_ENUM           "RocketGpioRegAddr"
 #define GPIO_REG_CONTROL     0x01
#undef BUGBUILD_ENUM          // RocketGpioRegAddr
 #define GPIO_CONTROL_LEVEL     ( 1<<0 )
 #define GPIO_CONTROL_DIRECTION ( 1<<1 )

/*****************************************************************************
 * DEVID_PWRI2C
 *
 * Provides PWRI2C product point config values
 *
 ****************************************************************************/

// PWRI2C_REG_CONFIG
// Bit  0   (r/w)    Enable PP Encoding instead of voltage(bqhuge) encoding
// Bit  1-7 (r)      Describes which external switcher is present so the correct
//                   voltage encoding can be used in the SVCVLVL registers
#define BUGBUILD_ENUM           "RocketPwrI2c"
 #define PWRI2C_REG_CONFIG        0x00
#undef BUGBUILD_ENUM          // RocketGpioRegAddr
 #define PWRI2C_CONFIG_PP_ENCODING_EN  (1<<0)
 #define PWRI2C_CONFIG_EXTSW_MAX8649   (0<<1)     // MAX8649,  0x00 = 0.770V -> 0x3F = 1.400V .. 0.010V steps
 #define PWRI2C_CONFIG_EXTSW_TPS62361  (1<<1)     // TPS62361, 0x00 = 0.500V -> 0x7F = 1.770V .. 0.010V steps

// PWRI2C_REG_SVCVLVLXXX
// Bit  0-7   (r/w)    Product point voltage settings for VCC_MAIN
#define BUGBUILD_ENUM           "RocketPwrI2c"
 #define PWRI2C_REG_SVCVLVLD2     0x01
 #define PWRI2C_REG_SVCVLVL0      0x02
 #define PWRI2C_REG_SVCVLVL1      0x03
 #define PWRI2C_REG_SVCVLVL2      0x04
 #define PWRI2C_REG_SVCVLVL3      0x05
 #define PWRI2C_REG_SVCVLVL4      0x06
#undef BUGBUILD_ENUM          // RocketPwrI2c

// PWRI2C_REG_STATUS
// bit 0-7   (r)       Last voltage setting which was received over PWRI2C
#define BUGBUILD_ENUM           "RocketPwrI2c"
 #define PWRI2C_REG_STATUS        0x07
#undef BUGBUILD_ENUM          // RocketPwrI2c

// List of valid PWRI2C commands for host to send, requesting
// the specified VLVL
#define ROCKET_ALVLD2     (0)
#define ROCKET_ALVL0      (1)
#define ROCKET_ALVL1      (2)
#define ROCKET_ALVL2      (3)
#define ROCKET_ALVL3      (4)
#define ROCKET_ALVL4      (5)

/*****************************************************************************
 * DEVID_FT
 *
 * Provides Fault Tolerance implemenation
 *
 ****************************************************************************/

// FT_STATUS
// Bit  0   (r)      Set if the FT feature is enabled on Rocket
#define BUGBUILD_ENUM           "RocketFt"
 #define FT_REG_STATUS           0x00
#undef BUGBUILD_ENUM          // RocketFt
 #define FT_ENABLED               (1<<0)

// FT_CONFIG
// Bit  0  (w)      Kick the watchdog to prevent a reset (90sec timeout)
// Bit  1  (w)      Set this bit to trigger the Brick Bit shutdown
#define BUGBUILD_ENUM           "RocketFt"
 #define FT_REG_CONFIG           0x01
#undef BUGBUILD_ENUM          // RocketFt
 #define FT_KICK                (1<<0)
 #define FT_BRICK               (1<<1)

// FT_PASSWORD
// Bit 0-7 (w)      Writing password byte will disable the FT Watchdog; 
//                  writing any other value will enable it
#define BUGBUILD_ENUM           "RocketFt"
 #define FT_REG_PASSWORD         0x02
#undef BUGBUILD_ENUM          // RocketFt
 #define FT_PASSWORD            (0xBC)

/*****************************************************************************
 * DEVID_PASLUMP
 *
 * Provides PA slump detection implemenation
 *
 ****************************************************************************/

// PASLUMP_CONFIG
// Bit  0   (r/w)      Enable/disable the PA slump detect feature, reflects 
//                     if feature state also
// Bit  1   (r)        Set if PA slump was detected, clear on read
#define BUGBUILD_ENUM           "RocketPaSlump"
 #define PASLUMP_REG_CONFIG      0x00
#undef BUGBUILD_ENUM          // RocketPaSlump
 #define PASLUMP_ENABLE         (1<<0)
 #define PASLUMP_DETECTED       (1<<1)

// PASLUMP_VBAT_MSB
// PASLUMP_VBAT_LSB
// Bit 0-7   (r)       Raw ADC measurement of VBAT which tripped slump
#define BUGBUILD_ENUM           "RocketPaSlump"
 #define PASLUMP_REG_VBAT_MSB   0x01
 #define PASLUMP_REG_VBAT_LSB   0x02
#undef BUGBUILD_ENUM          // RocketPaSlump

// PASLUMP_TRESH_MSB
// PASLUMP_THRESH_LSB
// Bit 0-7   (r/w)     Raw ADC threshold which will trip PA Slump detection
#define BUGBUILD_ENUM           "RocketPaSlump"
 #define PASLUMP_REG_TRESH_MSB  0x03
 #define PASLUMP_REG_TRESH_LSB  0x04
#undef BUGBUILD_ENUM          // RocketPaSlump

/*****************************************************************************
 * DEVID_PMIC
 *
 * Provides direct access to PMIC registers
 * Bridging     Host <-I2C-> PMU430 <-SPI-> PowerDie
 *
 ****************************************************************************/

// PMIC_REG_x
// Bits 0-7 (rw) See rocket spec for bit descriptions
#define BUGBUILD_ENUM                   "RocketPmicRegAddr"
 #define PMIC_REG_EN_CMD0                0x00
 #define PMIC_REG_EN_CMD1                0x01
 #define PMIC_REG_EN_CMD2                0x02
 #define PMIC_REG_MBSTCTRL0              0x03
 #define PMIC_REG_MBSTCTRL1              0x04
 #define PMIC_REG_MBSTCTRL2              0x05
 #define PMIC_REG_ABSTCTRL0              0x06
 #define PMIC_REG_ABSTCTRL1              0x07
 #define PMIC_REG_PWM_BLU                0x08
 #define PMIC_REG_PWM_GRN                0x09
 #define PMIC_REG_PWM_RED                0x0A
 #define PMIC_REG_RGB_CTRL               0x0B
 #define PMIC_REG_PWMCOL3                0x0C
 #define PMIC_REG_PWMCOL2                0x0D
 #define PMIC_REG_PWMCOL1                0x0E
 #define PMIC_REG_PWMCOL0                0x0F
 #define PMIC_REG_MATRIXCTRL             0x10
 #define PMIC_REG_CTRL_PPCHG0            0x11
 #define PMIC_REG_CTRL_PPCHG1            0x12
 #define PMIC_REG_CTRL_PPCHG2            0x13
 #define PMIC_REG_CTRL_PPCHG3            0x14
 #define PMIC_REG_FLASHCTRL0             0x15
 #define PMIC_REG_FLASHCTRL1             0x16
 #define PMIC_REG_FLASHCTRL2             0x17
 #define PMIC_REG_FLASHCTRL3             0x18
 #define PMIC_REG_VSM3                   0x19
 #define PMIC_REG_VSM2                   0x1A
 #define PMIC_REG_TSM2                   0x1B
 #define PMIC_REG_VSM1                   0x1C
 #define PMIC_REG_VSM0                   0x1D
 #define PMIC_REG_TSM0                   0x1E
 #define PMIC_REG_BUCKCTRL               0x1F
 #define PMIC_REG_DIG_PWM                0x20
 #define PMIC_REG_VLDO0                  0x21
 #define PMIC_REG_VLDO1                  0x22
 #define PMIC_REG_VLDO2                  0x23
 #define PMIC_REG_VLDO3                  0x24
 #define PMIC_REG_VLDO4                  0x25
 #define PMIC_REG_VLDO5                  0x26
 #define PMIC_REG_VLDO6                  0x27
 #define PMIC_REG_VLDO7                  0x28
 #define PMIC_REG_VLDO8                  0x29
 #define PMIC_REG_VLDO9                  0x2A
 #define PMIC_REG_VLDO10                 0x2B
 #define PMIC_REG_VLDO11                 0x2C
 #define PMIC_REG_VLDO12                 0x2D
 #define PMIC_REG_VRTC                   0x2E
 #define PMIC_REG_ADCIN_SEL              0x2F
// #define PMIC_REG_                       0x30
// #define PMIC_REG_                       0x31
 #define PMIC_REG_LDO_MON                0x32
     #define PMIC_REG_STAT_PPCHG0                       0x33
     #define PMIC_REG_STAT_PPCHG1                       0x34
     #define PMIC_REG_STAT_PPCHG2                       0x35
     #define PMIC_REG_INTFLAGS0                         0x36
     #define PMIC_REG_INTFLAGS1                         0x37
     #define PMIC_REG_INTFLAGS2                         0x38
     #define PMIC_REG_STAT0                             0x39
     #define PMIC_REG_STAT1                             0x3A
     #define PMIC_REG_STAT2                             0x3B
     #define PMIC_REG_STAT3                             0x3C
     #define PMIC_REG_STAT4                             0x3D
     #define PMIC_REG_INTMASK0                          0x3E
     #define PMIC_REG_INTMASK1                          0x3F
     #define PMIC_REG_INTMASK2                          0x40
     #define PMIC_REG_INTMASK3                          0x41
     #define PMIC_REG_INTMASK4                          0x42
     #define PMIC_REG_INTMASK5                          0x43
// #define PMIC_REG_                       0x35
// #define PMIC_REG_                       0x36
// #define PMIC_REG_                       0x37
// #define PMIC_REG_                       0x38
// #define PMIC_REG_                       0x39
// #define PMIC_REG_                       0x3A   PMIC Intctrl registers, see DEVID_PMIC_INTCTRL
// #define PMIC_REG_                       0x3B
// #define PMIC_REG_                       0x3C
// #define PMIC_REG_                       0x3D
// #define PMIC_REG_                       0x3E
// #define PMIC_REG_                       0x3F
// #define PMIC_REG_                       0x40
// #define PMIC_REG_                       0x41
// #define PMIC_REG_                       0x42
// #define PMIC_REG_                       0x43
 #define PMIC_REG_MASKREV                0x44
 #define PMIC_REG_OPTIONS                0x45
 #define PMIC_REG_OPTIONS_UNLOCK         0x48
#undef BUGBUILD_ENUM                  // RocketPmicRegAddr

#define PMIC_EN_CMD0_EN_SM0              (1<<0)
#define PMIC_EN_CMD0_EN_SM1              (1<<1)
#define PMIC_EN_CMD0_EN_SM2              (1<<2)
#define PMIC_EN_CMD0_EN_SM3              (1<<3)
#define PMIC_EN_CMD0_PGDGL               (1<<7)
#define PMIC_EN_CMD0_EN_ECO              (1<<8)

#define PMIC_EN_CMD1_EN_LDO0             (1<<0)
#define PMIC_EN_CMD1_EN_LDO1             (1<<1)
#define PMIC_EN_CMD1_EN_LDO2             (1<<2)
#define PMIC_EN_CMD1_EN_LDO3             (1<<3)
#define PMIC_EN_CMD1_EN_LDO4             (1<<4)
#define PMIC_EN_CMD1_EN_LDO5             (1<<5)
#define PMIC_EN_CMD1_EN_LDO6             (1<<6)
#define PMIC_EN_CMD1_EN_LDO7             (1<<7)

#define PMIC_EN_CMD2_EN_LDO8             (1<<0)
#define PMIC_EN_CMD2_EN_LDO9             (1<<1)
#define PMIC_EN_CMD2_EN_LDO10            (1<<2)
#define PMIC_EN_CMD2_EN_LDO11            (1<<3)
#define PMIC_EN_CMD2_EN_LDO12            (1<<4)

#define PMIC_MBSTCTRL0_BKLPWM_MASK       (0xFF)

#define PMIC_MBSTCTRL1_BKLPWM_MASK       (0xF<<0)
#define PMIC_MBSTCTRL1_ENBKL1            (1<<4)
#define PMIC_MBSTCTRL1_ENBKL0            (1<<5)
#define PMIC_MBSTCTRL1_CNTPWM_EN         (1<<6)
#define PMIC_MBSTCTRL1_MBST_ILIM         (1<<7)

#define PMIC_MBSTCTRL2_PRESCALE_MASK     (7<<0)
#define PMIC_MBSTCTRL2_VOLTMBST          (1<<3)

#define PMIC_ABSTCTRL0_AUXPWM_MASK       (0xFF)

#define PMIC_ABSTCTRL1_AUXPWM_MASK       (0xF<<0)
#define PMIC_ABSTCTRL1_ABST_ILIM         (1<<6)
#define PMIC_ABSTCTRL1_VOLTABST          (1<<7)

#define PMIC_CTRL_PPCHG0_CHGTERM_EN      (1<<0)
#define PMIC_CTRL_PPCHG0_EN_CHGR         (1<<1)
#define PMIC_CTRL_PPCHG0_BATDCH          (1<<2)
#define PMIC_CTRL_PPCHG0_PPSEL_MASK      (3<<4)
#define PMIC_CTRL_PPCHG0_PPSEL_BAT       (0<<4)
#define PMIC_CTRL_PPCHG0_PPSEL_USBBAT    (1<<4)
#define PMIC_CTRL_PPCHG0_PPSEL_ACBAT     (2<<4)
#define PMIC_CTRL_PPCHG0_PPSEL_ACUSBBAT  (3<<4)
#define PMIC_CTRL_PPCHG0_ILIM_USB_MASK   (3<<6)
#define PMIC_CTRL_PPCHG0_ILIM_USB_100    (0<<6)
#define PMIC_CTRL_PPCHG0_ILIM_USB_500    (1<<6)
#define PMIC_CTRL_PPCHG0_ILIM_USB_900    (2<<6)
#define PMIC_CTRL_PPCHG0_ILIM_USB_2500   (3<<6)

#define PMIC_CTRL_PPCHG1_ICHG_MASK       (7<<0)
#define PMIC_CTRL_PPCHG1_CHG_TIMR_MASK   (7<<3)
#define PMIC_CTRL_PPCHG1_VPRECHG         (1<<6)
#define PMIC_CTRL_PPCHG1_ITERM           (1<<7)

#define PMIC_CTRL_PPCHG2_VCHG_MASK       (7<<0)
#define PMIC_CTRL_PPCHG2_VLOWSYS_MASK    (7<<3)
#define PMIC_CTRL_PPCHG2_LDO_MODE        (1<<6)
#define PMIC_CTRL_PPCHG2_FORCE_TSON      (1<<7)

#define PMIC_CTRL_PPCHG3_WD_RST          (1<<0)
#define PMIC_CTRL_PPCHG3_FPRECHG         (1<<7)

#define PMIC_FLASHCTRL0_ITORCH_MASK      (7<<0)
#define PMIC_FLASHCTRL0_FLSH_BLNK        (1<<3)
#define PMIC_FLASHCTRL0_PWM_DIM          (1<<4)
#define PMIC_FLASHCTRL0_CV_TORCH_EN      (1<<5)
#define PMIC_FLASHCTRL0_FLSHMODE_MASK    (3<<6)
#define PMIC_FLASHCTRL0_FLSHMODE_SHUTDOWN    (0<<6)
#define PMIC_FLASHCTRL0_FLSHMODE_TORCH       (1<<6)
#define PMIC_FLASHCTRL0_FLSHMODE_TORCHFLASH  (2<<6)
#define PMIC_FLASHCTRL0_FLSHMODE_CONSTVOLT   (3<<6)

#define PMIC_FLASHCTRL1_IFLASH_MASK      (0xF<<0)
#define PMIC_FLASHCTRL1_IFLSHREF_MASK    (7<<4)
#define PMIC_FLASHCTRL1_SFT              (1<<7)

#define PMIC_FLASHCTRL2_STIM_MASK        (0x1F<<0)
#define PMIC_FLASHCTRL2_STT              (1<<5)
#define PMIC_FLASHCTRL2_DCTIM_MASK       (3<<6)

#define PMIC_FLASHCTRL3_FLSH_TW_MASK     (3<<0)
#define PMIC_FLASHCTRL3_FLSH_TR_MASK     (3<<2)
#define PMIC_FLASHCTRL3_FLSH_VFLASH_MASK (7<<4)
#define PMIC_FLASHCTRL3_FLSH_FLSHREFEN   (1<<7)

#define PMIC_BUCKCTRL_SM0_ECO            (1<<0)
#define PMIC_BUCKCTRL_SM0_FPWM           (1<<1)
#define PMIC_BUCKCTRL_SM1_ECO            (1<<2)
#define PMIC_BUCKCTRL_SM1_FPWM           (1<<3)
#define PMIC_BUCKCTRL_SM2_ECO            (1<<4)
#define PMIC_BUCKCTRL_SM2_FPWM           (1<<5)
#define PMIC_BUCKCTRL_SM3_ECO            (1<<6)
#define PMIC_BUCKCTRL_SM3_FPWM           (1<<7)

#define PMIC_DIG_PWM_PWM_D_MASK          (0xF<<0)
#define PMIC_DIG_PWM_PWM_F_MASK          (0x7<<4)
#define PMIC_DIG_PWM_PWM_EN              (1<<7)

#define PMIC_VRTC_VRTC_MASK              (3<<0)
#define PMIC_VRTC_VRTC_2V2               (0<<0)
#define PMIC_VRTC_VRTC_2V5               (1<<0)
#define PMIC_VRTC_VRTC_2V8               (2<<0)
#define PMIC_VRTC_VRTC_3V1               (3<<0)
#define PMIC_VRTC_CL_SEL_MASK            (3<<2)
#define PMIC_VRTC_CL_SEL_1P5             (0<<2)
#define PMIC_VRTC_CL_SEL_6P5             (1<<2)
#define PMIC_VRTC_CL_SEL_7P5             (2<<2)
#define PMIC_VRTC_CL_SEL_12P5            (3<<2)
#define PMIC_VRTC_FGLDO_EN               (1<<7)

#define PMIC_LDO_MON_LDOSEL_MASK         (0xF<<0)

#define PMIC_STAT_PPCHG0_FLSH_TMR        (1<<0)
#define PMIC_STAT_PPCHG0_LED_FLT         (1<<1)
#define PMIC_STAT_PPCHG0_BAT_CON         (1<<2)
#define PMIC_STAT_PPCHG0_DPPM            (1<<3)
#define PMIC_STAT_PPCHG0_THERMAL         (1<<4)
#define PMIC_STAT_PPCHG0_TEMP_LOW        (1<<5)
#define PMIC_STAT_PPCHG0_TEMP_HIGH       (1<<6)
#define PMIC_STAT_PPCHG0_CHGR_ON         (1<<7)

#define PMIC_STAT_PPCHG1_USB_PRESENT     (1<<0)
#define PMIC_STAT_PPCHG1_USB_CON         (1<<1)
#define PMIC_STAT_PPCHG1_USB_OVP         (1<<2)
#define PMIC_STAT_PPCHG1_AC_PRESENT      (1<<3)
#define PMIC_STAT_PPCHG1_AC_CON          (1<<4)
#define PMIC_STAT_PPCHG1_AC_OVP          (1<<5)
#define PMIC_STAT_PPCHG1_BAT_PRESENT     (1<<6)
#define PMIC_STAT_PPCHG1_LOWSYS          (1<<7)

#define PMIC_STAT_PPCHG2_BAT_OVP         (1<<0)
#define PMIC_STAT_PPCHG2_WD_TMR          (1<<1)
#define PMIC_STAT_PPCHG2_CHG_TERM        (1<<2)
#define PMIC_STAT_PPCHG2_PRECHG_FLT      (1<<3)
#define PMIC_STAT_PPCHG2_CHG_FLT         (1<<4)
#define PMIC_STAT_PPCHG2_LIN_CHG         (1<<5)
#define PMIC_STAT_PPCHG2_PRE_CHG         (1<<6)
#define PMIC_STAT_PPCHG2_FAST_CHG        (1<<7)

#define PMIC_INTMASK0_M_BATDET           (1<<0)
#define PMIC_INTMASK0_M_USBDET           (1<<1)
#define PMIC_INTMASK0_M_ACDET            (1<<2)
#define PMIC_INTMASK0_M_PWRSTAT          (1<<3)
#define PMIC_INTMASK0_M_SYSCHG           (1<<4)
#define PMIC_INTMASK0_M_BATOV            (1<<5)
#define PMIC_INTMASK0_M_PWROV            (1<<6)
#define PMIC_INTMASK0_M_LOWSYS           (1<<7)

#define PMIC_INTMASK1_M_RDCECHG          (1<<0)
#define PMIC_INTMASK1_M_PCHGTMR          (1<<1)
#define PMIC_INTMASK1_M_CHGTMR           (1<<2)
#define PMIC_INTMASK1_M_CHGTERM          (1<<3)
#define PMIC_INTMASK1_M_WDTMR            (1<<4)
#define PMIC_INTMASK1_M_BATTEMP          (1<<5)
#define PMIC_INTMASK1_M_TSHUT            (1<<6)

#define PMIC_INTMASK2_M_FLEDTMR          (1<<0)
#define PMIC_INTMASK2_M_MBSTOV           (1<<1)
#define PMIC_INTMASK2_M_ABSTOV           (1<<2)
#define PMIC_INTMASK2_M_RGBACT           (1<<3)
#define PMIC_INTMASK2_M_FTMPWRN          (1<<4)
#define PMIC_INTMASK2_M_FTMPREG          (1<<5)
#define PMIC_INTMASK2_M_FLSHSTO          (1<<6)

#define PMIC_INTMASK3_M_SM0PG            (1<<0)
#define PMIC_INTMASK3_M_SM1PG            (1<<1)
#define PMIC_INTMASK3_M_SM2PG            (1<<2)
#define PMIC_INTMASK3_M_SM3PG            (1<<3)

#define PMIC_INTMASK4_M_LDO0PG           (1<<0)
#define PMIC_INTMASK4_M_LDO1PG           (1<<1)
#define PMIC_INTMASK4_M_LDO2PG           (1<<2)
#define PMIC_INTMASK4_M_LDO3PG           (1<<3)
#define PMIC_INTMASK4_M_LDO4PG           (1<<4)
#define PMIC_INTMASK4_M_LDO5PG           (1<<5)
#define PMIC_INTMASK4_M_LDO6PG           (1<<6)
#define PMIC_INTMASK4_M_LDO7PG           (1<<7)

#define PMIC_INTMASK5_M_LDO8PG           (1<<0)
#define PMIC_INTMASK5_M_LDO9PG           (1<<1)
#define PMIC_INTMASK5_M_LDO10PG          (1<<2)
#define PMIC_INTMASK5_M_LDO11PG          (1<<3)
#define PMIC_INTMASK5_M_LDO12PG          (1<<4)

#define PMIC_INTFLAGS0_BAT_DET           (1<<0)
#define PMIC_INTFLAGS0_USB_DET           (1<<1)
#define PMIC_INTFLAGS0_AC_DET            (1<<2)
#define PMIC_INTFLAGS0_PWR_CHANGE        (1<<3)
#define PMIC_INTFLAGS0_SYS_CHANGE        (1<<4)
#define PMIC_INTFLAGS0_BAT_OVP           (1<<5)
#define PMIC_INTFLAGS0_PWR_OVP           (1<<6)
#define PMIC_INTFLAGS0_LOWSYS            (1<<7)

#define PMIC_INTFLAGS1_REDUCE_CHG        (1<<0)
#define PMIC_INTFLAGS1_TPRECHG_FLT       (1<<1)
#define PMIC_INTFLAGS1_TCHG_FLG          (1<<2)
#define PMIC_INTFLAGS1_CHG_TERM          (1<<3)
#define PMIC_INTFLAGS1_CHG_WD            (1<<4)
#define PMIC_INTFLAGS1_BAT_TEMP          (1<<5)
#define PMIC_INTFLAGS1_OVR_TEMP          (1<<6)
#define PMIC_INTFLAGS1_PWRGD_FLT         (1<<7)

#define PMIC_INTFLAGS2_FLASH_FLT         (1<<0)
#define PMIC_INTFLAGS2_MBST_OV           (1<<1)
#define PMIC_INTFLAGS2_ABST_OV           (1<<2)
#define PMIC_INTFLAGS2_RGB_ACTIVE        (1<<3)
#define PMIC_INTFLAGS2_FLSH_TWARN        (1<<4)
#define PMIC_INTFLAGS2_FLSH_TREG         (1<<5)
#define PMIC_INTFLAGS2_FLSH_STO          (1<<6)

#define PMIC_STAT0_RGB_ACT                 (1 << 0)
#define PMIC_STAT0_ABST_OVSTAT             (1 << 1)
#define PMIC_STAT0_MBST_OVSTAT             (1 << 2)
#define PMIC_STAT0_BATOC                   (1 << 3)
#define PMIC_STAT0_BIT4                    (1 << 4)
#define PMIC_STAT0_FLSH_STO                (1 << 5)
#define PMIC_STAT0_FTEMPW                  (1 << 6)
#define PMIC_STAT0_FTEMPR                  (1 << 7)

#define PMIC_SMLDO_DCHG_EN             (1<<7)


#endif
