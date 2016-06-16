/*****************************************************************************
 * Filename:    lagavulin_i2c.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     January 05, 2011
 *
 * Description: Lagavulin I2C Routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "intvector.h"
#include "gpio.h"
#include "timer_mux.h"
#include "i2c.h"
#include "delay.h"
#include "api.h"
#include "msgque.h"
#include "lagavulin_registers.h"
#include "slowclock.h"

#define VERBOSE_LEVEL 2
#include "eventlog.h"

#define I2C_MASTER_DIV          0x0913  // 400KHz bus: Sysclk=12MHz, I2CCKH=0x09, I2CCKL=0x13

//#define I2C_MASTER_FG_BOOST_DELAY   5 // 5/32768 = 152us

// interrupt handlers
static void i2c0RxTxIsr( void );
static void i2c1RxTxIsr( void );
static void i2c2RxTxIsr( void );

// I2C States for slave mode
//   HOST refers to I2C Module connected to host side I2C Bus
//   DEV refers to I2C device peripheral within rocket (IE BqHuge or the FG)
typedef enum {
    HOST_RECEIVED_START,
    // Slave related states
    HOST_I2C_ERROR,
    HOST_RECEIVED_DEVID,
    HOST_RECEIVED_REGADDR,
    HOST_HAS_DEVID_REGADDR,
    HOST_RECEIVED_STOP
} I2C_State;

// Next state for relay on event
static WORD nextState[ I2C_NUM_MODULES ];

// keep track of how many bytes have been sent/received in a transaction
// NOT counting SlaveAddr, SubDevID, or RegAddr
static WORD accessCount[ I2C_NUM_MODULES ];

static I2cModule module[ I2C_NUM_MODULES ];

// TODO: only one slave can be defined right now, they all use these state variables.
// TODO: test if more than one slave can be created now and remove above comment
static BYTE devId[ I2C_NUM_MODULES ];   /*= 0xFF; */           // 0xFF represents no devId has been specified yet
static BYTE regAddr[ I2C_NUM_MODULES ]; /*= 0xFF; */           // 0xFF represents no regAddr has been specified yet

const BYTE adrSeqRom[3][20] = {
    // Address sequence for ROM commands in USB Debug mode (code download to eMMC when P1.0 is high)
    { 0x4A<<1, 0x4A<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x49<<1, 0x49<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x7F<<1, 0x00, 0x00, 0x00 },
    // Address sequence for ROM commands in MMC1 Debug mode (Boot from uSD card for debugging when P1.0 is high)
    { 0x4A<<1, 0x4A<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x49<<1, 0x49<<1, 0x48<<1, 0x48<<1, 0x7F<<1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    // Address sequence for ROM commands in MMC2 Application mode (normal boot from eMMC)
    { 0x4A<<1, 0x4A<<1, 0x7F<<1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

static BYTE adrSeqMode = 0;
static BYTE adrSeqCnt = 0;

static void hwInitI2c( I2cModuleId i2cModule, I2cAddress addr, WORD mode )
{
    I2cModule *m = &module[ i2cModule ];

    // set these flags to their default
    devId[ i2cModule ] = 0xFF;
    regAddr[ i2cModule ] = 0xFF;

    m->mode = mode;
    m->inUse = MODULE_AVAILABLE;

    switch( mode ) {
        case I2C_MASTER:
            // configure I2C2 as master for I2C relay purposes
            WriteRegWord( I2CCN2_REG16, I2CCN_I2CMST );
            // configure for 400Khz bus clock
            WriteRegWord( I2CCK2_REG16, I2C_MASTER_DIV );
            // disable interrupts
            WriteRegWord( I2CIE2_REG16, 0x0000 );
            // enable i2c module
            WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CEN );
            break;
        case I2C_SLAVE_DUMMY:
            // configure I2C1 as slave dummy for communication with power
            WriteRegWord( I2CCN1_REG16, 0x0000 );
            WriteRegWord( I2CSLA1_REG16, (WORD)(addr >> 1) );
            nextState[ PWRI2C_MODULE ] = HOST_RECEIVED_START;
            // enable interrupts
            WriteRegWord( I2CIE1_REG16, I2CIE_I2CRXIE | I2CIE_I2CAMIE | I2CIE_I2CSPIE );
            // enable i2c module
            WriteRegWordOr( I2CCN1_REG16, I2CCN_I2CEN );
            break;
        case I2C_SLAVE:
        case I2C_SLAVE_ROM:
            // configure I2C0 as slave for communication with host
            WriteRegWord( I2CCN0_REG16, 0x0000 );
            WriteRegWord( I2CSLA0_REG16, (WORD)(addr >> 1) );
            nextState[ HOSTI2C_MODULE ] = HOST_RECEIVED_START;
            // enable interrupts
            WriteRegWord( I2CIE0_REG16, I2CIE_I2CSRIE | I2CIE_I2CTXIE | I2CIE_I2CRXIE | I2CIE_I2CSTRIE | I2CIE_I2CAMIE | I2CIE_I2CSPIE );
            // enable i2c module
            WriteRegWordOr( I2CCN0_REG16, I2CCN_I2CEN | I2CCN_I2CSTREN );
            break;
        default:
            // uh oh
            break;
    }
}

#pragma inline=forced
static void i2cSlaveDummyIsr( I2cModuleId i2cModule )
{
    WORD i2cStatus;

    switch( nextState[ i2cModule ] ) {

        case HOST_RECEIVED_START:
            i2cStatus = ReadRegWord( I2CST1_REG16 );
            if( i2cStatus & I2CST_I2CAMI ) {
                // we got an address match so clear the interrupt
                 WriteRegWordAnd( I2CST1_REG16, ~(I2CST_I2CSRI | I2CST_I2CAMI) );
                if( ReadRegWord( I2CCN1_REG16 ) & I2CCN_I2CMODE ) {
                    // host should not be requesting anything from us at this point!
                    nextState[ i2cModule ] = HOST_I2C_ERROR;
                } else {
                    // prepare for data reception
                    //devId[ i2cModule ] = DEVID_PWRI2C;
                    nextState[ i2cModule ] = HOST_RECEIVED_REGADDR;
                }
            } else {
                // we are expecting a start byte first!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            }
            break;

        case HOST_RECEIVED_REGADDR:
            i2cStatus = ReadRegWord( I2CST1_REG16 );
            if( i2cStatus & I2CST_I2CRXI ) {
                // we received a byte so read and process it (to clear the interrupt)
                regAddr[ i2cModule ] =(BYTE) ReadRegWord( I2CBUF1_REG16 );
                nextState[ i2cModule ] = HOST_HAS_DEVID_REGADDR;
            } else {
                // we are expecting a register address data byte!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            }
            break;

        case HOST_HAS_DEVID_REGADDR:
            i2cStatus = ReadRegWord( I2CST1_REG16 );
            if( i2cStatus & I2CST_I2CRXI ) {
                // we received a byte so read it (to clear the interrupt)
                //ApiRegWrite( devId[ i2cModule ], regAddr[ i2cModule ], (BYTE) ReadRegWord( I2CBUF1_REG16 ) );
                ReadRegWord( I2CBUF1_REG16 );
                //nextState[ i2cModule ] = HOST_RECEIVED_STOP;
            } else if( i2cStatus & I2CST_I2CSPI ) {
                // received a stop byte so clear the interrupt
                WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSPI );
                //ApiStopReceived( devId[ i2cModule ] );
                nextState[ i2cModule ] = HOST_RECEIVED_START;
                SCSleepEnable( i2cModule );
            } else {
                // unexpected interrupt!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            }
            break;

        case HOST_RECEIVED_STOP:
            i2cStatus = ReadRegWord( I2CST1_REG16 );
            if( i2cStatus & I2CST_I2CSPI ) {
                // received a stop byte so clear the interrupt
                WriteRegWordAnd( I2CST1_REG16, ~I2CST_I2CSPI );
                //ApiStopReceived( devId[ i2cModule ] );
                nextState[ i2cModule ] = HOST_RECEIVED_START;
                SCSleepEnable( i2cModule );
            } else {
                // unexpected interrupt!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            }
            break;

        case HOST_I2C_ERROR:
        default:
            // clear all possible interrupts
            WriteRegWord( I2CST1_REG16, 0x0000 );
            nextState[ i2cModule ] = HOST_RECEIVED_START;
            break;
    }
}

#pragma inline=forced
static void i2cSlaveRomIsr( I2cModuleId i2cModule )
{
    WORD i2cStatus;

    while( i2cStatus = ReadRegWord( I2CST0_REG16 ) ) {

        // check for the start byte condition
        if( i2cStatus & I2CST_I2CSRI ) {
            // clear the interrupts
             WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSRI );
            SCSleepDisable( i2cModule );
        }

        // check for slave address match flag
        if( i2cStatus & I2CST_I2CAMI ) {

            // clear the interrupts
            WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CAMI );

            if( ReadRegWord( I2CCN0_REG16 ) & I2CCN_I2CMODE ) {

                // host is requesting data from us
                switch( ReadRegWord(I2CSLA0_REG16) ) {
                    case 0x48:
                        // send the expected answer for P2 to emulate TWL6030
                        WriteRegWord( I2CBUF0_REG16, 0x01 );
                        break;
                    case 0x49:
                        if( ApiRegRead( DEVID_PMIC, 0x02 ) & 0x20 ) {
                            adrSeqMode = 0;
                            // tell OMAP to boot from USB via P8 response = 0x84
                            WriteRegWord( I2CBUF0_REG16, 0x84 );
                        } else {
                            adrSeqMode = 1;
                            // tell OMAP to boot from MMC1 via P8 response = 0x80
                            WriteRegWord( I2CBUF0_REG16, 0x80 );
                        }
                        break;
                    case 0x4A:
    #if defined( OMAP_SYSBOOT_USB_UART_MMC1_MMC2 )
                        adrSeqMode = 0;
    #else
                        adrSeqMode = 2;
    #endif
                        // send the expected answer for P1 to emulate TWL6030
                        WriteRegWord( I2CBUF0_REG16, 0x00 );
                        break;
                    default:
                        //V1( EventLogAdd( EVENTLOG_I2C_ROM_ERROR ) );
                        WriteRegWord( I2CBUF0_REG16, 0x00 );
                        break;
                }
            }
        }

        // check for transmit complete flag
        if( i2cStatus & I2CST_I2CTXI ) {
            // clear the interrupt
             WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CTXI );
        }

        // check for nack flag
        if( i2cStatus & I2CST_I2CTXI ) {
            // clear the interrupt
            WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CNACKI );
        }

        // check for data receipt flag
        if( i2cStatus & I2CST_I2CRXI ) {
            // read the buffer to clear the interrupt
            (BYTE) ReadRegWord( I2CBUF0_REG16 );
        }

        // check for clock stretch interrupt
        if( i2cStatus & I2CST_I2CSTRI ) {
            // clear the interrupt
             WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSTRI );
        }

        // check for the stop condition
        if( i2cStatus & I2CST_I2CSPI ) {

            // clear the interrupt
             WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSPI );

            SCSleepEnable( i2cModule );

                  // switch to the appropriate I2C slave rom address
               hwInitI2c( HOSTI2C_MODULE, adrSeqRom[adrSeqMode][adrSeqCnt++], I2C_SLAVE_ROM );
               continue;

        }

        // determine if I2C slave rom execution is completed
        if( ReadRegWord(I2CSLA0_REG16) == 0x7F ) {
            //V1( EventLogAdd( EVENTLOG_I2C_ROM_FINISH ) );
            adrSeqMode = 0;
            adrSeqCnt = 0;
            I2cInit();
            return;
        }

    }
}

#pragma inline=forced
static void i2cSlaveIsr( I2cModuleId i2cModule )
{
    WORD i2cStatus;

    switch( nextState[ i2cModule ] ) {

        case HOST_RECEIVED_START:
            i2cStatus = ReadRegWord( I2CST0_REG16 );
            if( i2cStatus & I2CST_I2CSRI ) {
                // we got a start byte condition so clear the interrupt
                 WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSRI );
                SCSleepDisable( i2cModule );
            } else if( i2cStatus & I2CST_I2CAMI ) {
                // we got an address match so clear the interrupt
                 WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CAMI );
                if( ReadRegWord( I2CCN0_REG16 ) & I2CCN_I2CMODE ) {
                    // host should not be requesting anything from us at this point!
                    nextState[ i2cModule ] = HOST_I2C_ERROR;
                } else {
                    // prepare for data reception while clock is stretched
                    nextState[ i2cModule ] = HOST_RECEIVED_DEVID;
                }
            } else if( i2cStatus & I2CST_I2CSTRI ) {
                // we got a clock stretch so clear the interrupt
                 WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSTRI );
            } else {
                // we are expecting a start byte first!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            }
            break;

        case HOST_RECEIVED_DEVID:
            i2cStatus = ReadRegWord( I2CST0_REG16 );
            if( i2cStatus & I2CST_I2CRXI ) {
                // we received a byte so read it (to clear the interrupt)
                devId[ i2cModule ] = (BYTE) ReadRegWord( I2CBUF0_REG16 );
                // let the device know it just received a START
                ApiStartReceived( devId[ i2cModule ] );
                // process data while clock is stretched
                nextState[ i2cModule ] = HOST_RECEIVED_REGADDR;
            } else if( i2cStatus & I2CST_I2CSTRI ) {
                // we got a clock stretch so clear the interrupt
                 WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSTRI );
            } else {
                // we are expecting a device id data byte!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            }
            break;

        case HOST_RECEIVED_REGADDR:
            i2cStatus = ReadRegWord( I2CST0_REG16 );
            if( i2cStatus & I2CST_I2CRXI ) {
                // we received a byte so read it (to clear the interrupt)
                regAddr[ i2cModule ] =(BYTE) ReadRegWord( I2CBUF0_REG16 );
                // process data while clock is stretched
                nextState[ i2cModule ] = HOST_HAS_DEVID_REGADDR;
                accessCount[ i2cModule ] = 0;
            } else if( i2cStatus & I2CST_I2CSTRI ) {
                // we got a clock stretch so clear the interrupt
                 WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSTRI );
            } else {
                // we are expecting a register address data byte!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            }
            break;

        case HOST_HAS_DEVID_REGADDR:
            i2cStatus = ReadRegWord( I2CST0_REG16 );
            if( i2cStatus & I2CST_I2CSRI ) {
                // we got a start byte condition so clear the interrupt
                 WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSRI );
                SCSleepDisable( i2cModule );
            } else if( i2cStatus & I2CST_I2CAMI ) {
                // we have a repeated start condition so clear the interrupt
                 WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CAMI );
                if( (ReadRegWord( I2CCN0_REG16 ) & I2CCN_I2CMODE) && (accessCount[i2cModule] == 0) ) {
                    // host is reading from the device register
                    WriteRegWord( I2CBUF0_REG16, ApiRegRead( devId[ i2cModule ], regAddr[ i2cModule ] ) );
                    accessCount[ i2cModule ]++;
                } else {
                    // slave is suppose to transmit not receive byte!
                    nextState[ i2cModule ] = HOST_I2C_ERROR;
                }
            } else if( i2cStatus & I2CST_I2CRXI ) {
                // we received a byte so read it (to clear the interrupt)
                ApiRegWrite( devId[ i2cModule ], regAddr[ i2cModule ], (BYTE) ReadRegWord( I2CBUF0_REG16 ) );
                accessCount[ i2cModule ]++;
            } else if( i2cStatus & I2CST_I2CTXI ) {
                // byte transmitted so clear the interrupt
                WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CTXI );
                if( i2cStatus & I2CST_I2CNACKI ) {
                    // transaction completed so clear the nack flag
                    WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CNACKI );
                    nextState[ i2cModule ] = HOST_RECEIVED_STOP;
                } else {
                    // something wrong happened so re-transmit
                    WriteRegWord( I2CBUF0_REG16, ApiRegRead( devId[ i2cModule ], regAddr[ i2cModule ] ) );
                    accessCount[ i2cModule ]++;
                }
            } else if( i2cStatus & I2CST_I2CSPI ) {
                // received a stop byte so clear the interrupt
                WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSPI );
                ApiStopReceived( devId[ i2cModule ] );
                nextState[ i2cModule ] = HOST_RECEIVED_START;
                SCSleepEnable( i2cModule );
            } else if( i2cStatus & I2CST_I2CSTRI ) {
                // we got a clock stretch so clear the interrupt
                 WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSTRI );
            } else {
                // unexpected interrupt!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            }
            break;

        case HOST_RECEIVED_STOP:
            i2cStatus = ReadRegWord( I2CST0_REG16 );
            if( i2cStatus & I2CST_I2CSPI ) {
                // received a stop byte so clear the interrupt
                WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSPI );
                ApiStopReceived( devId[ i2cModule ] );
                nextState[ i2cModule ] = HOST_RECEIVED_START;
                SCSleepEnable( i2cModule );
            } else if( i2cStatus & I2CST_I2CSTRI ) {
                // we got a clock stretch so clear the interrupt
                 WriteRegWordAnd( I2CST0_REG16, ~I2CST_I2CSTRI );
            } else {
                // unexpected interrupt!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            }
            break;

        case HOST_I2C_ERROR:
        default:
            // clear all possible interrupts
            WriteRegWord( I2CST0_REG16, 0x0000 );
            nextState[ i2cModule ] = HOST_RECEIVED_START;
            break;
    }
}

#pragma inline=forced
static void i2cMasterIsr( I2cModuleId i2cModule )
{
    WORD i2cst;
    I2cModule *m = &module[ i2cModule ];

    // read highest pending interrupt source
    i2cst = ReadRegWord( I2CST2_REG16 );

    // ARBITRATION Error
    if( i2cst & I2CST_I2CALI ) {
        m->inUse = MODULE_REQ_ERRSTOP;
        WriteRegWordAnd( I2CST2_REG16, ~I2CST_I2CALI );
        EventLogAdd( EVENTLOG_I2C_ARBLOST );
    }
    // NACK Error
    if( i2cst & I2CST_I2CNACKI ) {
        m->inUse = MODULE_REQ_ERRSTOP;
        WriteRegWordAnd( I2CST2_REG16, ~I2CST_I2CNACKI );
        EventLogAdd( EVENTLOG_I2C_NACKED );
    }

    // RX buffer contains new data
    if( i2cst & I2CST_I2CRXI ) {
        WriteRegWordAnd( I2CST2_REG16, ~I2CST_I2CRXI );
        // if the byte we just about to read out was our last byte, we are done
        if( m->bufferIndex == m->desc[m->index].len-1 ) {
            m->inUse = MODULE_COMPLETE;
        }
        // then read RXd byte so we send the proper NACK/STOP to the slave
        m->desc[m->index].data[ m->bufferIndex ] = (BYTE) ReadRegByte( I2CBUF2_REG16 );
        m->bufferIndex++;
        m->bytesRead++;
        // next byte we get is our last, so send stop with it
        if( m->bufferIndex == (m->desc[m->index].len-1) ) {
            WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CSTOP );
            while( ReadRegWord( I2CCN2_REG16 ) & I2CCN_I2CSTOP );
            while( ReadRegWord( I2CST2_REG16 ) & I2CST_I2CBUSY );
        }

    }
    // TX buffer ready for data
    if( i2cst & I2CST_I2CTXI ) {
        WriteRegWordAnd( I2CST2_REG16, ~I2CST_I2CTXI );
        WriteRegWord( I2CBUF2_REG16, m->desc[m->index].data[ m->bufferIndex ] );
        m->bufferIndex++;

        // Any we out of bytes in this descriptor?
        if( m->bufferIndex == m->desc[m->index].len ) {
            m->index++;

            // we are out of descriptors to run; we are done, send STOP
            if(  m->index == m->descSize ) {
                // we have finished TXing, send stop
                m->inUse = MODULE_REQ_STOP;
            } else {
                // we expect a RESTART here... otherwise return an error
                if( m->desc[m->index].op != I2C_OP_RESTART ) {
                    m->inUse = MODULE_REQ_ERRSTOP;
                } else {
                    m->index++;
                    m->bufferIndex = 0;

                    // configure direction of transaction for NEXT operation being performed
                    if( m->desc[m->index].op == I2C_OP_READ ) {
                        // read mode
                        WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CMODE );
                        // disable TX interrupt since we arn't TX'ing
                    } else {
                        // write mode
                        WriteRegWordAnd( I2CCN2_REG16, ~I2CCN_I2CMODE );
                    }
                    // disable interrupts
                    WriteRegWord( I2CIE2_REG16, 0x0000 );
                    // send repeat start condition and wait until it is sent out
                    WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CSTART );
                    while( ReadRegWord( I2CCN2_REG16 ) & I2CCN_I2CSTART );
                    while( ReadRegWord( I2CST2_REG16 ) & I2CST_I2CBUSY );
                    // write address of device we will be talking to
                    WriteRegWord( I2CBUF2_REG16, (WORD)(LAGAVULIN_SLAVE_ADDR >> 1) );
                    // if this next byte transaction is going to be our last,
                    // we need to send a stop with it in the RX case
                    if( (m->desc[m->index].len <= 1) && m->desc[m->index].op == I2C_OP_READ ) {
                        while( !(ReadRegWord( I2CST2_REG16 ) & I2CST_I2CTXI) ) {
                            if( ReadRegWord( I2CST2_REG16 ) & I2CST_I2CNACKI ) {
                                // slave NACKed the repeat start address
                                m->inUse = MODULE_REQ_ERRSTOP;
                                WriteRegWordAnd( I2CST2_REG16, ~I2CST_I2CNACKI );
                                EventLogAdd( EVENTLOG_I2C_NACKED );
                                break;
                            }
                        }
                        // then trigger the stop to go with next read
                        WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CSTOP );
                        while( ReadRegWord( I2CCN2_REG16 ) & I2CCN_I2CSTOP );
                        while( ReadRegWord( I2CST2_REG16 ) & I2CST_I2CBUSY );
                    }
                }
            }
        }
    }

    if( !(i2cst & (I2CST_I2CTXI | I2CST_I2CRXI | I2CST_I2CALI | I2CST_I2CNACKI)) ) {
        m->inUse = MODULE_REQ_ERRSTOP;
    }

    if( (m->inUse == MODULE_REQ_STOP) || (m->inUse == MODULE_REQ_ERRSTOP) ) {
        WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CSTOP );
        while( ReadRegWord( I2CCN2_REG16 ) & I2CCN_I2CSTOP );
        while( ReadRegWord( I2CST2_REG16 ) & I2CST_I2CBUSY );
        if( m->inUse == MODULE_REQ_STOP ) {
            m->inUse = MODULE_COMPLETE;
        } else {
            m->inUse = MODULE_ERROR;
        }
    }

    // if we are done, or there was an error free up the module and do the callback
    // if required
    if( (m->inUse >= MODULE_COMPLETE) /*&& (m->callback != NULL)*/ ) {
        // disable the interrupt sources now that we are done
        WriteRegWord( I2CIE2_REG16, 0x0000 );
        // nothing for CB to read back, so make module available right away
        // incase CB wants to setup another I2C transaction.
        // If we arn't using a callback, don't flag it as available yet.
        if( m->bytesRead == 0 && (m->callback != NULL) ) {
            m->inUse = MODULE_AVAILABLE;
        }
        if( m->callback != NULL ) {
            (m->callback)();
            m->inUse = MODULE_AVAILABLE;
        }
    }
}

/**
 * Performs a START or REPEATED_START transaction on the I2C bus
 *
 * @param address 7-bit device address on the I2C bus.
 * @param rw A bit indicating whether this is a write (0) or read (1) transaction
 *
 * @return 1 on success, 0 on any failure
 */
static BOOL i2cDoStart( I2cModuleId i2cModule, I2cAddress address, BOOL tx )
{
    I2cModule *m = &module[ i2cModule ];

    // incrment to next operation
    m->index++;
    m->bufferIndex = 0;

    if( i2cModule == FGI2C_MODULE ) {
        // disable i2c module
        WriteRegWordAnd( I2CCN2_REG16, ~I2CCN_I2CEN );
        // configure direction of transaction for NEXT operation being performed
        WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CMODE );
        if( tx ) {
            WriteRegWordAnd( I2CCN2_REG16, ~I2CCN_I2CMODE );
        }
        // enable i2c in master mode and wait for bus to clear
        WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CEN | I2CCN_I2CMST );
        while( ReadRegWord( I2CST2_REG16 ) & I2CST_I2CBUSY );
        // send start condition and wait until it is sent out and the busy is clear
        WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CSTART );
        while( ReadRegWord( I2CCN2_REG16 ) & I2CCN_I2CSTART );
        while( ReadRegWord( I2CST2_REG16 ) & I2CST_I2CBUSY );
        // write address of slave device
        WriteRegWord( I2CBUF2_REG16, (WORD)(address >> 1) );
        // Enable module interrupts
        WriteRegWord( I2CIE2_REG16, I2CIE_I2CTXIE | I2CIE_I2CRXIE | I2CIE_I2CALIE | I2CIE_I2CNACKIE );
    } else {

    }

    return( TRUE );
}

/**
 * Performs an I2C write write operation and blocks until it completes
 * This cannot be called with interrupts disabled
 *
 * Returns 0 on success, none zero on an error
 */
WORD I2cWriteWrite( I2cModuleId i2cModule, I2cAddress address, BYTE *wBuffer1, BYTE wSize1, BYTE *wBuffer2, BYTE wSize2 )

{
    I2cModule *m = &module[ i2cModule ];
    I2cDescriptor d[3];
    WORD wason = CoreIntDisable();
    // interrupts were already off, can't run
    if( !wason ) {
        return( 1 );
    } else if( m->inUse != MODULE_AVAILABLE) {
        CoreIntEnable( wason );
        // module already in use
        return( 1 );
    } else {
        m->inUse = MODULE_INUSE;
    }
    CoreIntEnable( wason );

    // make sure we don't try to callback anything
    m->callback = NULL;

    m->index = 0;
    m->bufferIndex = 0;
    m->bytesRead = 0;

    m->desc = d;

    m->desc[0].op   = I2C_OP_START;
    m->desc[1].op   = I2C_OP_WRITE;
    m->desc[1].data = wBuffer1;
    m->desc[1].len  = wSize1;
    m->desc[2].op   = I2C_OP_WRITE;
    m->desc[2].data = wBuffer2;
    m->desc[2].len  = wSize2;
    m->descSize = 3;

    i2cDoStart( i2cModule, address, TRUE );

    // wait for COMPLETE or ERROR flags
    while( m->inUse < MODULE_COMPLETE );

    m->inUse = MODULE_AVAILABLE;

    return ( (m->inUse == MODULE_ERROR) ? 1 : 0 );
}

/**
 * Performs an I2C write write operation and blocks until it completes
 * This cannot be called with interrupts disabled
 *
 * Returns 0 on success, none zero on an error
 */
WORD I2cWrite( I2cModuleId i2cModule, I2cAddress address, BYTE *wBuffer1, BYTE wSize1 )
{
    I2cModule *m = &module[ i2cModule ];
    I2cDescriptor d[2];
    WORD wason = CoreIntDisable();
    // interrupts were already off, can't run
    if( !wason ) {
        return( 1 );
    } else if( m->inUse != MODULE_AVAILABLE) {
        CoreIntEnable( wason );
        // module already in use
        return( 1 );
    } else {
        m->inUse = MODULE_INUSE;
    }
    CoreIntEnable( wason );

    // make sure we don't try to callback anything
    m->callback = NULL;

    m->index = 0;
    m->bufferIndex = 0;
    m->bytesRead = 0;

    m->desc = d;

    m->desc[0].op   = I2C_OP_START;
    m->desc[1].op   = I2C_OP_WRITE;
    m->desc[1].data = wBuffer1;
    m->desc[1].len  = wSize1;
    m->descSize = 2;

    i2cDoStart( i2cModule, address, TRUE );

    // wait for COMPLETE or ERROR flags
    while( m->inUse < MODULE_COMPLETE );

    m->inUse = MODULE_AVAILABLE;

    return ( (m->inUse == MODULE_ERROR) ? 1 : 0 );
}

/**
 * Performs an I2C write read operation and blocks until it completes
 * This cannot be called with interrupts disabled
 *
 * Returns 0 on success, none zero on an error
 */
WORD I2cWriteRead( I2cModuleId i2cModule, I2cAddress address, BYTE *wBuffer, BYTE wSize, BYTE *rBuffer, BYTE rSize )
{
    I2cModule *m = &module[ i2cModule ];
    I2cDescriptor d[4];

    WORD wason = CoreIntDisable();

    // interrupts were already off, can't run
    if( !wason ) {
        return( 1 );
    } else if( m->inUse != MODULE_AVAILABLE) {
        CoreIntEnable( wason );
        // module already in use
        return( 1 );
    } else {
        m->inUse = MODULE_INUSE;
    }
    CoreIntEnable( wason );

    // make sure we don't try to callback anything
    m->callback = NULL;

    m->index = 0;
    m->bufferIndex = 0;
    m->bytesRead = 0;

    m->desc = d;

    m->desc[0].op   = I2C_OP_START;
    m->desc[1].op   = I2C_OP_WRITE;
    m->desc[1].data = wBuffer;
    m->desc[1].len  = wSize;
    m->desc[2].op   = I2C_OP_RESTART;
    m->desc[3].op   = I2C_OP_READ;
    m->desc[3].data = rBuffer;
    m->desc[3].len  = rSize;
    m->descSize = 4;

    i2cDoStart( i2cModule, address, TRUE );

    // wait for COMPLETE or ERROR flags
    while( m->inUse < MODULE_COMPLETE );

    m->inUse = MODULE_AVAILABLE;

    return ( (m->inUse == MODULE_ERROR) ? 1 : 0 );
}

void I2cInitInternalBus( void )
{

    RegisterIntHandler( SPI0_I2C0_VECTOR, i2c0RxTxIsr );
    RegisterIntHandler( SPI1_I2C1_VECTOR, i2c1RxTxIsr );
    RegisterIntHandler( SPMI_I2C2_VECTOR, i2c2RxTxIsr );

    hwInitI2c( FGI2C_MODULE, 0x00 , I2C_MASTER );
}

void I2cRomInit( void )
{
    WORD wason = CoreIntDisable();
    V1( EventLogAdd( EVENTLOG_I2C_ROM_INIT ) );

    hwInitI2c( HOSTI2C_MODULE, adrSeqRom[adrSeqMode][adrSeqCnt++], I2C_SLAVE_ROM );

    CoreIntEnable( wason );

}

void I2cInit( void )
{
    WORD wason = CoreIntDisable();
    V1( EventLogAdd( EVENTLOG_I2C_INIT ) );

    hwInitI2c( HOSTI2C_MODULE, LAGAVULIN_SLAVE_ADDR, I2C_SLAVE );
    hwInitI2c( PWRI2C_MODULE, PWR_SLAVE_ADDR, I2C_SLAVE_DUMMY );
//    RegisterTimerExpiry( I2C_FG_SPEED_TIMER, i2cMasterSpeedBoost, i2cBusMapping[ FGI2C_MODULE ] );

    CoreIntEnable( wason );
}

void I2cUninit( void )
{
    // reset i2c module
    WriteRegWordOr( I2CCN0_REG16, I2CCN_I2CRST );
    WriteRegWordOr( I2CCN1_REG16, I2CCN_I2CRST );
    WriteRegWordOr( I2CCN2_REG16, I2CCN_I2CRST );

    // disable i2c module
    WriteRegWordAnd( I2CCN0_REG16, ~I2CCN_I2CEN );
    WriteRegWordAnd( I2CCN1_REG16, ~I2CCN_I2CEN );
    WriteRegWordAnd( I2CCN2_REG16, ~I2CCN_I2CEN );
}

static void i2cIsrHandler( I2cModuleId mId )
{
    switch( module[ mId ].mode ) {
        case I2C_MASTER:
            i2cMasterIsr( mId );
            break;
        case I2C_SLAVE:
            i2cSlaveIsr( mId );
            break;
        case I2C_SLAVE_DUMMY:
            i2cSlaveDummyIsr( mId );
            break;
        case I2C_SLAVE_ROM:
            i2cSlaveRomIsr( mId );
            break;
        default:
            break;
    }
}

static void i2c0RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIB0_INT );
    i2cIsrHandler( HOSTI2C_MODULE );
}

static void i2c1RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIB1_INT );
    i2cIsrHandler( PWRI2C_MODULE );
}

static void i2c2RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIB1_INT );
    i2cIsrHandler( FGI2C_MODULE );
}
