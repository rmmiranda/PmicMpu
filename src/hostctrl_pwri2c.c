/*****************************************************************************
 * Filename:    hostctrl_pwri2c.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Feb, 2010
 *
 * Description: Rocket power i2c request routines
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "timer_mux.h"
#include "rocket_pmic.h"
#include "config_pwrseq.h"
#include "spi.h"
#include "eventlog.h"
#include "hostctrl.h"
#include "max8649.h"
#if defined(RIM_SM0_MAIN_VCC)
#include "sm0.h"
#endif
static BYTE newVolt;
static BYTE lastVolt;

BYTE NumExtSwRequestsPending;
BYTE ExtSwLastWrittenValue;

// Identify which switcher is present for this HW
#if defined( RIM_TPS62361 )
 #define PWRI2C_CONFIG_EXTSW_PRESENT   PWRI2C_CONFIG_EXTSW_TPS62361
#else
 #define PWRI2C_CONFIG_EXTSW_PRESENT   PWRI2C_CONFIG_EXTSW_MAX8649
#endif

#if defined( FORCE_PP_ENCODING )
static BOOL UsePPEncoding = TRUE;
#else
static BOOL UsePPEncoding = FALSE;
#endif

#define NUM_SVCVLVLS        6
BYTE SvcVLvls[ NUM_SVCVLVLS ];

extern BYTE SysFlags0;

#if (defined( RIM_MAX8649 ) || defined( RIM_TPS62361 )) && (defined( HOSTPROCESSOR_TAVOR_MG ) || defined( HOSTPROCESSOR_ESHEL ))
/******************************************************************************
 * Max8649SetHugeVoltage( hugeVal )
 *
 * takes a BqHuge voltage setting and translates it to MAX8649
 *  - BqHuge, 0x00 = 0.725V -> 0x1F = 1.500V .. 0.025V steps
 *  - Mx8649, 0x00 = 0.770V -> 0x3F = 1.400V .. 0.010V steps
 *  - TPS62361, 0x00 = 0.500V -> 0x7F = 1.770V .. 0.010V steps
 * sets a voltage equal or 5mV higher than requested voltage (10mV resolution)
 *****************************************************************************/
void Max8649SetHugeVoltage( WORD hugeVal )
{
    BYTE maxVal = 0;
    WORD wason;

    if( UsePPEncoding ) {
        if( hugeVal >= NUM_SVCVLVLS ) {
            hugeVal = NUM_SVCVLVLS-1;
        }
        maxVal = SvcVLvls[ hugeVal ];
#if defined( RIM_TPS62361 )
    } else {
        // maxVal = (hugeVal * 2.5) + 23 (adding the 0.730-0.500 offset; rounding to neareset 10mv)
        maxVal = ((hugeVal << 1) + (hugeVal >> 1) ) + 23;

        // saturate at 1.770V (maximum supported by tps62361)
        if( maxVal & 0x80 ) {
            maxVal = 0x7F;
        }
    }
#else
    } else if( hugeVal > 1 ) { // 0 and 1 are below the max8649's minimum
        // maxVal = hugeVal * 2.5 - 4 (rounds up to the nearest 10mV)
        maxVal = (hugeVal << 1) + (hugeVal >> 1) - 4;
        // saturate at 1.40V (maximum supported by max8649)
        if( maxVal & 0xC0 ) {
            maxVal = 0x3F;
        }
    } else {
        maxVal = 0;
    }
#endif

    Max8649Write( MAX8649_MODE2, maxVal );

    // keep these all atomic
    wason = CoreIntDisable();
    // record the last value written to the switcher
    ExtSwLastWrittenValue = maxVal;
    // update the last poweri2c index request sent to the switcher
    lastVolt = hugeVal;
    // reduce the pending operations count
    NumExtSwRequestsPending--;
    // remove the force 988PP during the transition if we are allowed to
    Max8649UseTransition( FALSE );
    CoreIntEnable( wason );
}

/******************************************************************************
 * Max8649UpdTransitionVoltage( hugeVal )
 *
 * raise VID0 GPIO if voltage will be increasing
 *****************************************************************************/
void Max8649UpdTransitionVoltage( BYTE hugeVal )
{
    newVolt = hugeVal;
}

/******************************************************************************
 * Max8649UseTransition( hugeVal )
 *
 * lower VID0 GPIO
 *****************************************************************************/
void Max8649UseTransition( BOOL yes )
{
    if( yes && (newVolt > lastVolt) ) {
    #if defined(CORE_SW_VID0)
        PadConfig( CORE_SW_VID0, PAD_OUTPUT_HIGH );
    // Only allow the GPIO to transition back out of the temp 988PP voltage if there
    // aren't any pending transactions to go to the switcher
    } else if( NumExtSwRequestsPending == 0 ) {
        PadConfig( CORE_SW_VID0, PAD_OUTPUT_LOW );
    #endif
    }
}
#endif  // RIM_MAX8649

#if defined( RIM_SM0_MAIN_VCC ) && defined( HOSTPROCESSOR_TAVOR_MG )
/******************************************************************************
 * SetSM0VoltageFromSvcVLvls( index )
 *
 * Write the appropriate index from the SvcVLvls to SM0
 *****************************************************************************/

void SetSM0VoltageFromSvcVLvls(BYTE index)
{
    if( UsePPEncoding ) {
        if (index >= NUM_SVCVLVLS) {
            index = NUM_SVCVLVLS - 1;
        }
        PmicWrite( PMIC_VSM0, SvcVLvls[ index ] );
    }
    else {
        // we should never get here when using SM0 as the main power source
        // maybe we should catfail instead of setting a fixed voltage?
        PmicWrite( PMIC_VSM0, 0x34 ); //set 1.275 Volts
        UsePPEncoding = TRUE;
    }
}
#endif
/******************************************************************************
 * PwrI2cRegRead( )
 *
 * Returns PwrI2c Register value
 *****************************************************************************/
BYTE PwrI2cRegRead( BYTE reg )
{
    switch( reg ) {
        case PWRI2C_REG_CONFIG:
            return( PWRI2C_CONFIG_EXTSW_PRESENT | UsePPEncoding );
        case PWRI2C_REG_SVCVLVLD2:
        case PWRI2C_REG_SVCVLVL0:
        case PWRI2C_REG_SVCVLVL1:
        case PWRI2C_REG_SVCVLVL2:
        case PWRI2C_REG_SVCVLVL3:
        case PWRI2C_REG_SVCVLVL4:
            return( SvcVLvls[ reg-1 ] );
        case PWRI2C_REG_STATUS:
            return( newVolt );
        default:
            return( 0 );
    }
}

/******************************************************************************
 * PwrI2cRegWrite( )
 *
 * Sets PwrI2c Register value
 *****************************************************************************/
void PwrI2cRegWrite( BYTE reg, BYTE value )
{
    switch( reg ) {
        case PWRI2C_REG_CONFIG:
            UsePPEncoding = value & PWRI2C_CONFIG_PP_ENCODING_EN;
            lastVolt = 0;   // force a transition on the next voltage request
            break;
        case PWRI2C_REG_SVCVLVLD2:
        case PWRI2C_REG_SVCVLVL0:
        case PWRI2C_REG_SVCVLVL1:
        case PWRI2C_REG_SVCVLVL2:
        case PWRI2C_REG_SVCVLVL3:
        case PWRI2C_REG_SVCVLVL4:
        #if defined(RIM_SM0_MAIN_VCC)
            SvcVLvls[ reg-1 ] = value & SM0_MAX_INDEX; //cap it at the max SM0 index value to be safe
        #else
            SvcVLvls[ reg-1 ] = value;
        #endif
            break;
        default:
            return;
    }
    return;
}


/******************************************************************************
 * HostCtrlPwrI2cInit( )
 *
 * Init all Power I2c control functionality (called from APP init)
 *****************************************************************************/
void HostCtrlPwrI2cInit( void )
{
    lastVolt = 0;
    NumExtSwRequestsPending = 0;
#if defined(RIM_SM0_MAIN_VCC)
    // initialize the array to some valid values for SM0
    SvcVLvls[0] = 0xA; //0.725 V
    SvcVLvls[1] = 0x25;//1.0625 V
    SvcVLvls[2] = 0x27;//1.0875 V
    SvcVLvls[3] = 0x2A;//1.125 V
    SvcVLvls[4] = 0x33;//1.275 V
    SvcVLvls[5] = 0x38;//1.40 V
#endif
/*
#if defined( RIM_MAX8649 ) && defined( FORCE_PP_ENCODING )
    // initialize the array to some valid values for RIM_MAX8649 - need to revise this one
    SvcVLvls[0] =  place valid value for MAX8649 here
    SvcVLvls[1] =
    SvcVLvls[2] =
    SvcVLvls[3] =
    SvcVLvls[4] =
    SvcVLvls[5] =
#endif
*/
#if defined( RIM_TPS62361 ) && defined( FORCE_PP_ENCODING )
    // initialize the array to some valid values for RIM_TPS62361
    SvcVLvls[0] = 0x17;//0.730 V
    SvcVLvls[1] = 0x39;//1.0700 V
    SvcVLvls[2] = 0x3B;//1.0900 V
    SvcVLvls[3] = 0x3F;//1.1300 V
    SvcVLvls[4] = 0x4E;//1.2800 V
    SvcVLvls[5] = 0x5A;//1.40 V
#endif
    ApiRegisterHandlers( DEVID_PWRI2C, PwrI2cRegRead, PwrI2cRegWrite, NULL, NULL );
}

/******************************************************************************
 * HostCtrlPwrUninit( )
 *
 * Uninit all Power control funcationality (called from APP uninit)
 *****************************************************************************/
void HostCtrlPwrI2cUninit( void )
{
    ApiUnregisterHandlers( DEVID_PWRI2C );
}
