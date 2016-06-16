/*****************************************************************************
 * Filename:    pmic_max8649.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Jesse Schwartzentruber
 *
 * Created:     April, 2010
 *
 * Description: Functions to communicate with the MAX8649 converter
 ****************************************************************************/
#include "basetype.h"
#include "max8649.h"
#include "gpio.h"
#include "i2c.h"
#include "config_pwrseq.h"
#include "eventlog.h"

#define VLVL_ERR(mv)  (((mv)*103+99)/100) // add 3% accuracy error, rounding up

extern void ConfigureI2cPortMapping( BOOL external );

void Max8649Write( BYTE reg, BYTE value )
{
    BYTE buf[2];
    buf[0] = reg;
    buf[1] = value;
    ConfigureI2cPortMapping( TRUE ); // map the i2c output to external pins
    if( I2cWrite( FGI2C_MODULE, MAX8649_SLAVE_ADDR, &buf, 2 ) != 0 ) {
        EventLogAdd( EVENTLOG_I2C_WRITE_ERROR );
        RimInitiateReset( RESET_COOKIE_INITIATED_ERROR );
    }
}

BYTE Max8649Read( BYTE reg )
{
    BYTE result;

    ConfigureI2cPortMapping( TRUE ); // map the i2c output to external pins
    if( I2cWriteRead( FGI2C_MODULE, MAX8649_SLAVE_ADDR, &reg, 1, &result, 1 ) != 0 ) {
        EventLogAdd( EVENTLOG_I2C_READ_ERROR );
        RimInitiateReset( RESET_COOKIE_INITIATED_ERROR );
    }

    return result;
}

void Max8649Init( void )
{
    Max8649Write( MAX8649_RAMP, MAX8649_RAMP_32MV_US ); // ramp s.b. >= 25mV/us for Tavor
    // is config_pwrseq.h is overriding the default?
#if defined( EXT_SUPPLY_MODE0 )
    Max8649Write( MAX8649_MODE0, MODE_FROM_MV( VLVL_ERR( EXT_SUPPLY_MODE0 ) ) ); // sleep
    Max8649Write( MAX8649_MODE1, MODE_FROM_MV( VLVL_ERR( EXT_SUPPLY_MODE1 ) ) ); // sleep (s.b. unreachable)
    Max8649Write( MAX8649_MODE2, MODE_FROM_MV( VLVL_ERR( EXT_SUPPLY_MODE2 ) ) ); // default
    Max8649Write( MAX8649_MODE3, MODE_FROM_MV( VLVL_ERR( EXT_SUPPLY_MODE3 ) ) ); // transition
#else
    Max8649Write( MAX8649_MODE0, MODE_FROM_MV( VLVL_ERR( 750 ) ) ); // sleep
    Max8649Write( MAX8649_MODE1, MODE_FROM_MV( VLVL_ERR( 750 ) ) ); // sleep (s.b. unreachable)
    Max8649Write( MAX8649_MODE2, MODE_FROM_MV( VLVL_ERR( 1125 ) ) ); // default
    Max8649Write( MAX8649_MODE3, MODE_FROM_MV( VLVL_ERR( 1325 ) ) ); // transition
#endif
}

