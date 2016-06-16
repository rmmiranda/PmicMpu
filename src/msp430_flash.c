/*****************************************************************************
 * Filename:    msp430_flash.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     May, 2009
 *
 * Description: MSP430 flash drivers
 ****************************************************************************/
#include "basetype.h"
#include "registers.h"
#include "msp430_flash.h"
#include "critical.h"

void FlashEraseSegment( WORD address )
{
    WORD wason = CoreIntDisable();

    // unlock flash
    WriteRegWord( FCTL3_REG16, FCTLKEY | ( ReadRegByte( FCTL3_REG16 ) & ~FCTL3_LOCK ) );

    // setup segment erase
    WriteRegWord( FCTL1_REG16, FCTLKEY | 0x02 );

    // dummy write starts the erase
    WriteRegWord( address, 0 );

    // wait for operation to complete
    while( ReadRegByte( FCTL3_REG16 ) & FCTL3_BUSY );

    // lock flash
    WriteRegWord( FCTL3_REG16, FCTLKEY | ReadRegByte( FCTL3_REG16 ) | FCTL3_LOCK );

    CoreIntEnable( wason );
}

void FlashWrite( WORD address, DWORD data, FlashWriteType type )
{
    WORD wason = CoreIntDisable();

    // unlock flash
    WriteRegWord( FCTL3_REG16, FCTLKEY | ( ReadRegByte( FCTL3_REG16 ) & ~FCTL3_LOCK ) );

    // setup write
    WriteRegWord( FCTL1_REG16, FCTLKEY | ( type == FLASH_WRITE_DWORD ? 0x80 : 0x40 ) );

    // perform the write
    if( type == FLASH_WRITE_DWORD ) {
        WriteRegDword( address, data );
    } else if( type == FLASH_WRITE_WORD ) {
        WriteRegWord( address, data );
    } else {
        WriteRegByte( address, data );
    }

    // write is done
    WriteRegWord( FCTL1_REG16, FCTLKEY );

    // lock flash
    WriteRegWord( FCTL3_REG16, FCTLKEY | ReadRegByte( FCTL3_REG16 ) | FCTL3_LOCK );

    CoreIntEnable( wason );
}


/**
 * Writes a chunk of data to the specified address. Assumes that range of
 * addresses being written to have already been erased and are ready to be written to.
 *
 * Attempts to write as much data in chunks of DWORD size bursts as it is the most efficient.
 */
void FlashWriteBuffer( WORD address, BYTE *data, WORD size )
{
    WORD wason;
    WORD endAddress = address + size;
    WORD d1, d2;
    DWORD wData;

    if( size == 0 ) {
        return;
    }

    wason = CoreIntDisable();

    // unlock flash
    WriteRegWord( FCTL3_REG16, FCTLKEY | ( ReadRegByte( FCTL3_REG16 ) & ~FCTL3_LOCK ) );

    // setup write to be BYTE aligned
    WriteRegWord( FCTL1_REG16, FCTLKEY | 0x40 );


    // Write 1 byte at a time until we are DWORD aligned
    while( address & 0x3 ) {
        if( address >= endAddress ) {
            break;
        }
        WriteRegByte( address++, *data );
        data++;
    }

    // setup write to be DWORD aligned
    WriteRegWord( FCTL1_REG16, FCTLKEY | 0x80 );

    while( address<(endAddress-4) ) {
        // if the data buffer isn't aligned anymore to a DWORD
        // we need to grab the data one byte at a time.
        if( (WORD)data & 0x3 ) {

            // account for endianess and pack it into a DWORD
            d1 = *data++;
            d1 |= (*data++<<8);
            d2 = *data++;
            d2 |= (*data++<<8);

            wData = ((DWORD)d2<<16) | d1;

        } else {
            wData = *(DWORD*)data;
            data += 4;
        }
        WriteRegDword( address, wData );
        address += 4;
    }

    // setup write to be BYTE aligned again to finish off any remaining data
    WriteRegWord( FCTL1_REG16, FCTLKEY | 0x40 );

    if( address != endAddress ) {
        while( address<endAddress ) {
            WriteRegByte( address++, *data );
            data++;
        }
    }

    // write is done
    WriteRegWord( FCTL1_REG16, FCTLKEY );

    // lock flash
    WriteRegWord( FCTL3_REG16, FCTLKEY | ReadRegByte( FCTL3_REG16 ) | FCTL3_LOCK );

    CoreIntEnable( wason );
}

