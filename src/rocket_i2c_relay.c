/*****************************************************************************
 * Filename:    rocket_i2c_relay.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     May 31, 2010
 *
 * Description: Generic mechanism to relay i2c commands to different
 *              peripherals attached/within rocket (FG)
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "rocket_registers.h"
#include "api.h"
#include "rocket_pmic.h"
#include "msgque.h"
#include "i2c.h"
#include "i2c_relay.h"
#if defined( RIM_MAX8649 ) || defined( RIM_TPS62361 )
#include "max8649.h"
#endif
#include "gpio.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

static CommandState cmdState;
static BYTE         slaveAddr;
static BOOL         startCmd;
static BYTE         rdLen;
static BYTE         wrLen;
static BYTE         cmdBuffer[CMD_BUFFER_MAX_SIZE];

extern BYTE NumExtSwRequestsPending;

// utility function used as part of workaround for corrupted IFG bits
// when reading RXBuffer. forces a clock stretch to occur
void UsciForceClockStretch( WORD moduleBase, BOOL RestartPossible, Line SclGpio, BOOL highSpeed );

/******************************************************************************
 * i2cCmdProcess()
 *
 * Decode and process the current command (in the CmdBuffer).
 * Must be called from the main loop with interrutps enabled
 *****************************************************************************/
static void i2cCmdProcess( WORD unused )
{
    if( !wrLen || rdLen > CMD_BUFFER_MAX_SIZE || wrLen > CMD_BUFFER_MAX_SIZE ) {
        cmdState = STATE_CMDERR;
        return;
    }
    ConfigureI2cPortMapping( (slaveAddr != 0xAA) && (slaveAddr != 0x16) );
    if( rdLen ) {
        if( I2cWriteRead( FGI2C_MODULE, slaveAddr, cmdBuffer, wrLen, cmdBuffer, rdLen ) != 0 ) {
            cmdState = STATE_FAILURE;
        } else {
            cmdState = STATE_SUCCESS;
        }
    } else {
        if( I2cWrite( FGI2C_MODULE, slaveAddr, cmdBuffer, wrLen ) != 0 ) {
            cmdState = STATE_FAILURE;
        } else {
            cmdState = STATE_SUCCESS;
        }
    }
}

/******************************************************************************
 * i2cRlyReceivedStop()
 *
 * Trigger the just received command to be processed in the main loop
 *****************************************************************************/
static void i2cRlyReceivedStop( void )
{
    if( startCmd ) {
        cmdState = STATE_BUSY;
        QueueCall( i2cCmdProcess, 0 );
        startCmd = FALSE;
    }
}

/******************************************************************************
 * i2cRlyWriteByte()
 *
 * Handle received byte
 *****************************************************************************/
static void i2cRlyRegWrite( BYTE reg, BYTE data )
{
    switch( reg ) {
        case I2CRLY_REG_WRLEN:
            wrLen = data;
            break;
        case I2CRLY_REG_RDLEN:
            rdLen = data;
            break;
        case I2CRLY_REG_SLAVEADDR:
            slaveAddr = data;
            startCmd = TRUE;
            break;
        default:
            if( reg >= I2CRLY_REG_CMDBUFFER && reg < (I2CRLY_REG_CMDBUFFER+CMD_BUFFER_MAX_SIZE) ) {
                cmdBuffer[ reg-I2CRLY_REG_CMDBUFFER ] = data;
            } else {
                // ignore out-of-range
            }
            break;
    }
}

/******************************************************************************
 * i2cRlyReadByte()
 *
 * Provide requested byte
 *****************************************************************************/
static BYTE i2cRlyRegRead( BYTE reg )
{
    switch( reg ) {
        case I2CRLY_REG_STATE: { // clear to idle on read, unless busy or idle
            BYTE result = cmdState;
            if( cmdState > STATE_BUSY ) {
                cmdState = STATE_IDLE;
            }
            return result;
        }
        case I2CRLY_REG_WRLEN:
            return wrLen;
        case I2CRLY_REG_RDLEN:
            return rdLen;
        case I2CRLY_REG_SLAVEADDR:
            return slaveAddr;
        default:
            if( reg >= I2CRLY_REG_CMDBUFFER && (reg < (I2CRLY_REG_CMDBUFFER+CMD_BUFFER_MAX_SIZE)) ) {
                return cmdBuffer[ reg-I2CRLY_REG_CMDBUFFER ];
            }
            break;
    }
    return( 0 );
}

#if (defined( RIM_MAX8649 ) || defined( RIM_TPS62361 ) || defined( RIM_SM0_MAIN_VCC )) && defined( HOSTPROCESSOR_TAVOR_MG )
// Power I2C Protocol:
// When the host issues a voltage change request; the following packet is sent over PWRI2C
// START  SlaveAddr(0x68)  VoltageRegAddr(0x24)  VoltageSetting(0x??)  VCC1RegAddr(0x20)  SlectVoltageSet(0x03) STOP
// the only part of this transaction rocket needs to care about is the VoltageSetting(0x??) value. All of the other Bytes sent
// to rocket are BqHuge specific (IE, we ignore VoltageRegAddr since we know the external swither is the buck we need to control).
// We also ignore the VCC1RegAddr and the SelectVoltageSet data because these were just signals to bqhuge to apply the new voltage
// that was previously set.

static void i2cSlavePwrIsr( WORD moduleBase )
{
    BYTE data, flags;
    WORD recvd = 0;
    flags = ReadRegByte( UCBIFG_REG8( moduleBase ) );

    // Clear the START IFG
    WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~UCBIFG_UCSTTIFG );

    EventLogAdd( EVENTLOG_I2CRLY_PWRISR );
    while( 1 ) {
        if( flags & UCBIFG_UCRXIFG ) {
            if( recvd == 0 ) {
                // force a clock stretch to occur on first byte we receive
                // so we can guaruntee we don't read the RXbuffer on bit7 of the
                // next byte coming in (workaround for RXBuffer read bug).
                // don't need to do this for all bytes because we will be polling
                // the emainder of the ISR and grabbing the bytes quickly
                UsciForceClockStretch( moduleBase, FALSE, PWR_I2C_SCL, TRUE );
            }
            // read the received value

            data = ReadRegByte( UCBRXBUF_REG8( moduleBase ) );
            recvd++;
            if( recvd == 2 ) {
				// in Davis, SM0 is supplying VCC_Main and the external switcher has
				// been removed. Nessus sends and index into the SvcVLvls tables which
				// has been pre-programmed in PMICInit with valid values for SM0
				#if defined(RIM_SM0_MAIN_VCC)
				    SetSM0VoltageFromSvcVLvls(data);
				#else
                 NumExtSwRequestsPending++;
                //QueueCallPrio( Max8649SetHugeVoltage, data );
                //remove priority queue... lets not mess with the order, keep things FIFO
                QueueCall( Max8649SetHugeVoltage, data );
                Max8649UpdTransitionVoltage( data );
                Max8649UseTransition( TRUE );
                #endif // RIM_SM0_MAIN_VCC
                EventLogAddN( EVENTLOG_I2CRLY_PWRISR, data );
            } else if( recvd == 4 ) {
                return;
            }
        }
        flags = ReadRegByte( UCBIFG_REG8( moduleBase ) );
    }
}
#endif

/******************************************************************************
 * I2cRelayInit()
 *
 *****************************************************************************/
void I2cRlyInit( void )
{
    WORD wason = CoreIntDisable();

    cmdState = STATE_IDLE;

    ApiRegisterHandlers( DEVID_I2C_RELAY, i2cRlyRegRead, i2cRlyRegWrite, NULL, i2cRlyReceivedStop );

    // PWRI2C/Max8649 relay
#if (defined( RIM_MAX8649 ) || defined( RIM_TPS62361 ) || defined(RIM_SM0_MAIN_VCC)) && defined( HOSTPROCESSOR_TAVOR_MG )
    I2cRegisterExtCb( PWRI2C_MODULE, i2cSlavePwrIsr );
#endif
    CoreIntEnable( wason );
}

void I2cRlyUninit( void )
{
#if (defined( RIM_MAX8649 ) || defined( RIM_TPS62361 ) || defined(RIM_SM0_MAIN_VCC)) && defined( HOSTPROCESSOR_TAVOR_MG )
    I2cRegisterExtCb( PWRI2C_MODULE, NULL );
#endif
    ApiUnregisterHandlers( DEVID_I2C_RELAY );
}

