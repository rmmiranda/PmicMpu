/****************************************************************************
 * Filename:    i2c.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Dec 23, 2008
 * 
 * Modified:    February 25, 2011 by AS (TI)
 *
 * Description: i2c definitions
 ****************************************************************************/
#ifndef __I2C_H__
#define __I2C_H__

#include <basetype.h>

#if defined( PROCESSOR_PMU430 )
#include "rocket_usci.h"
#define USCI_HOSTI2C_BUS   USCI0B_BASE
#define USCI_PWRI2C_BUS    USCI1B_BASE
#define USCI_FGI2C_BUS     USCI2B_BASE
#elif defined( PROCESSOR_MQ31 )
#include "maxq31_sfr.h"
#else
#error I2C modules not defined for this processor
#endif

#define LAGAVULIN_SLAVE_ADDR    ( 0x68 )
#if defined( HOSTPROCESSOR_OMAP4430 )
  #define ROCKET_SLAVE_ADDR       ( 0x94 )
#else
  #define ROCKET_SLAVE_ADDR       ( 0x64 )
#endif
#define BQHUGE_SLAVE_ADDR       ( 0x68 )
#define PWR_SLAVE_ADDR          ( 0x68 )
#define FG_SLAVE_ADDR           ( 0xAA )
#define MAX8649_SLAVE_ADDR      ( 0xC0 )

// Mapping of HW I2C module/pins to Bus identifier
typedef enum {
    HOSTI2C_MODULE     = 0,
    PWRI2C_MODULE      = 1,
    FGI2C_MODULE       = 2,
    I2C_NUM_MODULES    = 3
} I2cModuleId;



// maximum number of operations that can be chained together
// for one I2C transaction
#define MAX_OP_CHAIN_SIZE    5

// Defines for various ModInUse flags
#define MODULE_AVAILABLE    0
#define MODULE_INUSE        1
#define MODULE_REQ_STOP     2
#define MODULE_REQ_ERRSTOP  3
#define MODULE_COMPLETE     4
#define MODULE_ERROR        5

// modes of operation for the I2C Module
#define I2C_SLAVE           ( 0x0 )
#define I2C_MASTER          ( 0x1 )
#define I2C_SLAVE_DUMMY     ( 0x2 )
#define I2C_SLAVE_EXT       ( 0x3 )
#define I2C_SLAVE_ROM       ( 0x4 )


typedef WORD I2cAddress;

typedef enum {
    I2C_OP_WRITE,
    I2C_OP_READ,
    I2C_OP_START,
    I2C_OP_RESTART
} I2cOperation;

typedef struct {
    I2cOperation  op;
    BYTE          *data;
    WORD          len;
} I2cDescriptor;

typedef struct {
    volatile WORD            index;              // current operation index
    volatile WORD   inUse;              // module state
    volatile WORD   mode;               // master, slave, or relay mode
    I2cModuleId     relayId;            // slaveId/hostId for module to relay to in relay mode
    volatile WORD            bytesRead;          // number of byte available from last transaction
    I2cDescriptor   *desc;              // operation descriptor chain
    volatile BYTE            bufferIndex;        // current byte index within descriptor buffer
    volatile BYTE            descSize;           // number of descriptors
    FUNCTION        callback;           // callback function when transaction is compelte/error
} I2cModule;

#if defined( HOSTPROCESSOR_OMAP4430 )
void I2cRomInit( void );                //AS+ Configure I2C to emulate TWL6030 trible address mode for ROM execution
#endif // HOSTPROCESSOR_OMAP4430
void I2cInit( void );
void I2cUninit( void );

WORD I2cWriteRead( I2cModuleId i2cModule, I2cAddress address, BYTE *wBuffer, BYTE wSize, BYTE *rBuffer, BYTE rSize );

WORD I2cRead( I2cModuleId i2cModule, I2cAddress address, BYTE *buffer, WORD buffersize );

WORD I2cWriteWrite( I2cModuleId i2cModule, I2cAddress address, BYTE *wBuffer1, BYTE wSize1, BYTE *wBuffer2, BYTE wSize2 );

void I2cRegisterExtCb( I2cModuleId mId, void (*isr)( WORD moduleBase ) );

void I2cInitInternalBus( void );

#endif

