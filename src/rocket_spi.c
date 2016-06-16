/*****************************************************************************
 * Filename:    rocket_spi.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     April 29, 2009
 *
 * Description: Rocket SPI Routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "intvector.h"
#include "gpio.h"
#include "rocket_usci.h"
#include "spi.h"
#include "delay.h"
#include "eventlog.h"

// interrupt handlers
void UsciA0RxTxIsr( void );
void UsciA1RxTxIsr( void );
void UsciA2RxTxIsr( void );

// PMIC is located on USCI0A SPI Module/bus
#define USCI_PMICSPI_BUS    USCI0A_BASE

// index into this array must match with the SpiModuleId value
const WORD spiBusMapping[] = {
    USCI_PMICSPI_BUS,
};

static void UsciInitSpi( SpiModuleId spiModule, WORD mode )
{
    WORD moduleBase = spiBusMapping[ spiModule ];

    // Put module in reset while configuring it
    WriteRegByteOr( UCCTL1_REG8( moduleBase ), UCSCTL1_UCSWRST );

    switch( mode ) {
        case SPI_MASTER:
            // configure clock source and division for module
            // use SMCLK(4Mhz)... and keep module in reset
            WriteRegByte( UCCTL1_REG8( moduleBase ), UCSCTL1_UCSSEL_SMCLK | UCSCTL1_UCSWRST );
            // divide by 1 for a CLK of 4Mhz
            WriteRegByte( UCBR0_REG8( moduleBase ), 1 );
            WriteRegByte( UCBR1_REG8( moduleBase ), 0 );

            // configure modules for MSB first, 8-bit length master mode, 3-pin SPI Mode
            WriteRegByte( UCCTL0_REG8( moduleBase ), UCSCTL0_UCMSB | UCSCTL0_UCMST | UCSCTL0_UCSYNC );
            break;
        default:
            // other modes not supported right now
            break;
    }

    // Take module out of reset now that we are done
    WriteRegByteAnd( UCCTL1_REG8( moduleBase ), ~UCSCTL1_UCSWRST );

}

// takes approx. 12uS to complete @ a 4Mhz CLK
void PmicWrite( BYTE addr, BYTE data )
{
    WORD moduleBase = spiBusMapping[ PMICSPI_MODULE ];
    WORD wason = CoreIntDisable();

    // slave select pin -> ACTIVE
    PadConfig( PMIC_SPI_CS, PAD_OUTPUT_HIGH );

    WriteRegByte( UCTXBUF_REG8( moduleBase ), (addr<<1) );
    while( ReadRegByte( UCSTAT_REG8( moduleBase ) ) & UCSSTAT_UCBUSY );
    WriteRegByte( UCTXBUF_REG8( moduleBase ), data );
    while( ReadRegByte( UCSTAT_REG8( moduleBase ) ) & UCSSTAT_UCBUSY );

    // slave select pin -> INACTIVE
    PadConfig( PMIC_SPI_CS, PAD_OUTPUT_LOW );

    CoreIntEnable( wason );
}

// takes approx. 12uS to complete @ a 4Mhz CLK
BYTE PmicRead( BYTE addr  )
{
    BYTE data;
    WORD moduleBase = spiBusMapping[ PMICSPI_MODULE ];
    WORD wason = CoreIntDisable();

    // slave select pin -> ACTIVE
    PadConfig( PMIC_SPI_CS, PAD_OUTPUT_HIGH );

    WriteRegByte( UCTXBUF_REG8( moduleBase ), (addr<<1) | 0x01 );
    while( ReadRegByte( UCSTAT_REG8( moduleBase ) ) & UCSSTAT_UCBUSY );

    HwDelay( 1000 );

    WriteRegByte( UCTXBUF_REG8( moduleBase ), 0x00 );   // dummy write so we clock in data
    while( ReadRegByte( UCSTAT_REG8( moduleBase ) ) & UCSSTAT_UCBUSY );
//    incase UCBUSY clears before rx shifted data is moved to RXBUF
//    while( (ReadRegByte( UCIFG_REG8( moduleBase ) ) & UCSIFG_UCRXIFG ) == 0 );

    data = ReadRegByte( UCRXBUF_REG8( moduleBase ) );

    // slave select pin -> INACTIVE
    PadConfig( PMIC_SPI_CS, PAD_OUTPUT_LOW );

    CoreIntEnable( wason );
    return( data );
}

void SpiInit( void )
{
    WORD wason = CoreIntDisable();

    RegisterIntHandler( USCIA0_RXTX_VECTOR, UsciA0RxTxIsr );
    RegisterIntHandler( USCIA1_RXTX_VECTOR, UsciA1RxTxIsr );
    RegisterIntHandler( USCIA2_RXTX_VECTOR, UsciA2RxTxIsr );

    // PMIC SPI in master mode
    UsciInitSpi( PMICSPI_MODULE, SPI_MASTER );

    CoreIntEnable( wason );
}

void UsciA0RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIA0_INT );
}

void UsciA1RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIA1_INT );
/*    if( module[ PMICSPI_MODULE ].mode == SPI_MASTER ) {
        SpiMasterIsr( PMICSPI_MODULE );
    }
*/
}

void UsciA2RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIA2_INT );
}

void UsciA3RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIA3_INT );
}


/**
 * Unused Interrupt driven SPI code
 *
 */

// SPI.H

/*#define MAX_OP_CHAIN_SIZE   5

// Defines for various ModInUse flags
#define MODULE_AVAILABLE    0
#define MODULE_INUSE        1
#define MODULE_COMPLETE     4
#define MODULE_ERROR        5

// Mapping of HW USCI module/pins to Bus identifier
#define USCI_PMICSPI_BUS    USCI1A_BASE

// modes of operation for the SPI Module
#define SPI_SLAVE           ( 0x0 )
#define SPI_MASTER          ( 0x1 )
#define SPI_MASTER_RELAY    ( 0x2 )

typedef enum {
    SPI_OP_WRITE,
    SPI_OP_READ
//    SPI_OP_WRITEREAD
} SpiOperation;

typedef struct {
    SpiOperation  op[ MAX_OP_CHAIN_SIZE ];
    BYTE          data[ MAX_OP_CHAIN_SIZE ];
    WORD          len;
} SpiDescriptor;

typedef enum {
    PMICSPI_MODULE  = 0,
    SPI_NUM_MODULES = 2
} SpiModuleId;

typedef struct {
    WORD            index;              // current operation
    WORD            inUse;              // module state
    WORD            mode;               // master or slave mode
    BYTE            bytesRead;
    SpiModuleId     relayId;            // slaveId/hostId for module to relay to in relay mode
    SpiDescriptor   desc;
    FUNCTION        callback;           // callback function when transaction is compelte/error
} SpiModule;
WORD SpiWriteReadWithCallback( SpiModuleId spiModule, BYTE wData1, FUNCTION cbFunc );
WORD SpiWriteRead( SpiModuleId spiModule, BYTE wData1, BYTE *rData1 );
WORD SpiReadData( SpiModuleId spiModule, BYTE *data );

WORD SpiWriteWriteWithCallback( SpiModuleId spiModule, BYTE wData1, BYTE wData2, FUNCTION cbFunc );
WORD SpiWriteWrite( SpiModuleId spiModule, BYTE wData1, BYTE wData2 );
*/

// SPI.C

/*static SpiModule module[ SPI_NUM_MODULES ];
static void UsciInitSpi( SpiModuleId spiModule, WORD mode )
{
    WORD moduleBase = spiBusMapping[ spiModule ];
    SpiModule *m = &module[ spiModule ];

    // Put module in reset while configuring it
    WriteRegByteOr( UCCTL1_REG8( moduleBase ), UCSCTL1_UCSWRST );

    m->mode = mode;
    m->inUse = MODULE_AVAILABLE;

    switch( mode ) {
        case SPI_MASTER:
            // configure clock source and division for module
            // use SMCLK(4Mhz)... and keep module in reset
            WriteRegByte( UCCTL1_REG8( moduleBase ), UCSCTL1_UCSSEL_SMCLK | UCSCTL1_UCSWRST );
            // configure us to divide down to 1Mhz
            WriteRegByte( UCBR0_REG8( moduleBase ), 4 );
            WriteRegByte( UCBR1_REG8( moduleBase ), 0 );

            // configure modules for MSB first, 8-bit length master mode, 3-pin SPI Mode
            WriteRegByte( UCCTL0_REG8( moduleBase ), UCSCTL0_UCMSB | UCSCTL0_UCMST | UCSCTL0_UCSYNC );
            break;
        default:
            // other modes not supported right now
            break;
    }

    // Take module out of reset now that we are done
    WriteRegByteAnd( UCCTL1_REG8( moduleBase ), ~UCSCTL1_UCSWRST );

} */
// starts SPI transfer on specified module... assumes there is only one slave on the
// bus for now
/*static void SpiStart( SpiModuleId spiModule )
{
    WORD moduleBase = spiBusMapping[ spiModule ];
    SpiModule *m = &module[ spiModule ];

    if( spiModule == PMICSPI_MODULE ) {
        // slave select pin -> ACTIVE
        PadSet( A1_SPI_STE, PAD_OUTPUT );
    }

    WriteRegByte( UCTXBUF_REG8( moduleBase ), m->desc.data[0] );

    // trigger for next byte to be sent is when this one is done sending (ie RX IFG)
    WriteRegByte( UCIE_REG8( moduleBase ), UCSIE_UCRXIE );

}

static void SpiMasterIsr( SpiModuleId spiModule )
{
    SpiModule *m = &module[ spiModule ];
    WORD moduleBase = spiBusMapping[ spiModule ];       // Base address of I2C module being used
    BYTE data;

//    BYTE ifg = ReadRegByte( UCIFG_REG8( moduleBase ) );



    data = ReadRegByte( UCRXBUF_REG8( moduleBase ) );

    if( m->desc.op[ m->index ] == SPI_OP_READ ) {
        m->desc.data[ m->index ] = data;
    }

    m->index++;

    // if we are done with the descriptor chain, do callback and make
    // module available again.
    if( m->index == m->desc.len ) {

        // flag it as complete for blocking API call to
        // know we are done. Callback API calls will happen
        // below.
        m->inUse = MODULE_COMPLETE;

        if( spiModule == PMICSPI_MODULE ) {
            // slave select pin -> INACTIVE
            PadClear( A1_SPI_STE, PAD_OUTPUT );
        }
        // nothing for CB to read back, so make module available right away
        // incase CB wants to setup another I2C transaction.
        // If we arn't using a callback, don't flag it as available yet.
        if( (m->bytesRead == 0) && (m->callback != NULL) ) {
            m->inUse = MODULE_AVAILABLE;
        }
        if( m->callback != NULL ) {
            (m->callback)();
            m->inUse = MODULE_AVAILABLE;
        }
    // else still have more to do
    } else {
        // send another byte (or dummy byte to allow a read)
        WriteRegByte( UCTXBUF_REG8( moduleBase ), m->desc.data[ m->index ] );
    }
}*/

/**
 * Performs an SPI write write operation and blocks until it completes
 * This cannot be called with interrupts disabled
 *
 * Returns 0 on success, none zero on an error
 */
/*WORD SpiWriteWrite( SpiModuleId spiModule, BYTE wData1, BYTE wData2 )
{
    SpiModule *m = &module[ spiModule ];
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

    m->desc.op[0]   = SPI_OP_WRITE;
    // shift 7-bit address by 1 so we are in write mode
    m->desc.data[0] = wData1;
    m->desc.op[1]   = SPI_OP_WRITE;
    m->desc.data[1] = wData2;
    m->desc.len = 2;

    SpiStart( spiModule );

    // wait for COMPLETE or ERROR flags
    while( m->inUse < MODULE_COMPLETE ) {};

    if( m->inUse == MODULE_ERROR ) {
        m->inUse = MODULE_AVAILABLE;
        return( 1 );
    }



    m->inUse = MODULE_AVAILABLE;
    return( 0 );
}*/

/**
 * Performs an SPI write read operation and blocks until it completes
 * This cannot be called with interrupts disabled
 *
 * Returns 0 on success, none zero on an error
 */
/*WORD SpiWriteRead( SpiModuleId spiModule, BYTE wData1, BYTE *rData1 )
{
    SpiModule *m = &module[ spiModule ];
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

    m->desc.op[0]   = SPI_OP_WRITE;
    // shift 7-bit address by 1 so we are in write mode
    m->desc.data[0] = wData1;
    m->desc.op[1]   = SPI_OP_READ;
    m->desc.len = 2;

    SpiStart( spiModule );

    // wait for COMPLETE or ERROR flags
    while( m->inUse < MODULE_COMPLETE ) {};

    if( m->inUse == MODULE_ERROR ) {
        m->inUse = MODULE_AVAILABLE;
        return( 1 );
    }

    *rData1 = m->desc.data[1];

        PadSet( PAD_PORT_1_0, PAD_OUTPUT_ENABLE );
    PadSet( PAD_PORT_1_0, PAD_OUTPUT);

    m->inUse = MODULE_AVAILABLE;

    return( 0 );
}*/

