/*****************************************************************************
 * Filename:    max8649.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Jesse Schwartzentruber
 *
 * Created:     April, 2010
 *
 * Description: Registers and API for Maxims's MAX8649 converter
 ****************************************************************************/
#ifndef __MAX8649_H__
#define __MAX8649_H__

#include <basetype.h>

// MAX8649 internal register addresses
#define MAX8649_MODE0           0x00
#define MAX8649_MODE1           0x01
#define MAX8649_MODE2           0x02
#define MAX8649_MODE3           0x03
  #define MAX8649_MODE_PWM              0x80
  #define MAX8649_MODE_SYNC             0x40
  #define MAX8649_MODE_VOLTAGE          0x3F
    #define MAX8649_MIN_MV                (770)
    #define MAX8649_MAX_MV                (1400)
    #define MAX8649_MV_STEP               (10)
    #define MAX8649_FROM_MV( mv )         ((((mv)-MAX8649_MIN_MV+(MAX8649_MV_STEP-1))/MAX8649_MV_STEP)&MAX8649_MODE_VOLTAGE)
    #define MAX8649_TO_MV( reg )          ((((reg)&MAX8649_MODE_VOLTAGE)*MAX8649_MV_STEP)+MAX8649_MIN_MV)
  #define TPS62361_MODE_PWM             0x80
  #define TPS62361_MODE_VOLTAGE         0x7F
    #define TPS62361_MIN_MV               (500)
    #define TPS62361_MAX_MV               (1770)
    #define TPS62361_MV_STEP              (10)
    #define TPS62361_FROM_MV( mv )        ((((mv)-TPS62361_MIN_MV+(TPS62361_MV_STEP-1))/TPS62361_MV_STEP)&TPS62361_MODE_VOLTAGE)
    #define TPS62361_TO_MV( reg )         ((((reg)&TPS62361_MODE_VOLTAGE)*TPS62361_MV_STEP)+TPS62361_MIN_MV)
#define MAX8649_CONTROL         0x04
  #define MAX8649_CONTROL_EN_PULLDOWN   0x80
  #define MAX8649_CONTROL_VID0_PULLDOWN 0x80
  #define MAX8649_CONTROL_VID1_PULLDOWN 0x80
#define MAX8649_SYNC            0x05
  #define MAX8649_SYNC_MASK             0xC0
    #define MAX8649_SYNC_26MHZ            0x00
    #define MAX8649_SYNC_13MHZ            0x40
    #define MAX8649_SYNC_19_2MHZ          0x80
    //#define MAX8649_SYNC_19_2MHZ          0xC0
#define MAX8649_RAMP            0x06
  #define MAX8649_RAMP_MASK             0xE0
    #define MAX8649_RAMP_32MV_US          0x00
    #define MAX8649_RAMP_16MV_US          0x20
    #define MAX8649_RAMP_8MV_US           0x40
    #define MAX8649_RAMP_4MV_US           0x60
    #define MAX8649_RAMP_2MV_US           0x80
    #define MAX8649_RAMP_1MV_US           0xA0
    #define MAX8649_RAMP_0_5MV_US         0xC0
    #define MAX8649_RAMP_0_25MV_US        0xE0
  #define MAX8649_RAMP_FORCE_HYS        0x10
  #define MAX8649_RAMP_FORCE_OSC        0x08
  #define MAX8649_RAMP_ACTIVE_RAMP_DOWN 0x02
  #define MAX8649_RAMP_SOFT_SWITCH      0x01
#define MAX8649_CHIP_ID1        0x08 // BCD 2-digit DIE_TYPE
#define MAX8649_CHIP_ID2        0x09 // BCD DASH (0xF0) and MASK_REV (0x0F)

#define CHIP_ID2_VENDORID       0xF0
  #define CHIP_ID2_VENDORID_TI      0x80
#define CHIP_ID2_CHIPID         0x0C
  #define CHIP_ID2_CHIPID_TPS62360  (0x00<<2)
  #define CHIP_ID2_CHIPID_TPS62361  (0x01<<2)
#define CHIP_ID2_REVID          0x03

void Max8649SetHugeVoltage( WORD hugeVal ); // arg is WORD only to fit FUNCTION1 defn
void Max8649UpdTransitionVoltage( BYTE hugeVal );
void Max8649UseTransition( BOOL yes );
BYTE Max8649Read( BYTE reg );
void Max8649Write( BYTE reg, BYTE value );

#define Max8649Set(r,v)         Max8649Write((r),Max8649Read(r)|(v))
#define Max8649ClearSet(r,v,m)  Max8649Write((r),(Max8649Read(r)&~(m))|((v)&(m)))
#define Max8649Clear(r,v)       Max8649Write((r),Max8649Read(r)&~(v))

void Max8649Init( void );


// if using the TPS62361 the volage range is different, use a different
// calculation to determine the mode register value from mV.
#if defined( RIM_TPS62361 )
  #define MODE_FROM_MV(x)   TPS62361_FROM_MV(x)
  #define MODE_VOLTAGE_MASK TPS62361_MODE_VOLTAGE
#else
  #define MODE_FROM_MV(x)   MAX8649_FROM_MV(x)
  #define MODE_VOLTAGE_MASK MAX8649_MODE_VOLTAGE
#endif

#endif // __MAX8649_H__

