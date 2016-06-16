/*****************************************************************************
 * Filename:    msp430_flash.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     May 25, 2009
 *
 * Description: Register defitions for the MSP430's Flash module
 ****************************************************************************/

#ifndef __MSP430_FLASH_H__
#define __MSP430_FLASH_H__

#if defined( PROCESSOR_PMU430 )
 #define FLASH_BASE_ADDR         0x140
#else
 #error Flash module not defined for this processor
#endif

#define FCTL1_REG16             (FLASH_BASE_ADDR + 0x00 )
#define FCTL3_REG16             (FLASH_BASE_ADDR + 0x04 )
#define FCTL4_REG16             (FLASH_BASE_ADDR + 0x06 )
#define IE1_REG16               (FLASH_BASE_ADDR + 0x0A )
#define IFG_REG16               (FLASH_BASE_ADDR + 0x0C )

#define FCTL3_BUSY              (1<<0)
#define FCTL3_LOCK              (1<<4)

#define FCTLKEY                 0xA500

typedef enum
{
    FLASH_WRITE_BYTE = 0,
    FLASH_WRITE_WORD = 1,
    FLASH_WRITE_DWORD = 2
} FlashWriteType;

void FlashEraseSegment( WORD address );
void FlashWrite( WORD address, DWORD data, FlashWriteType type );
void FlashWriteBuffer( WORD address, BYTE *data, WORD size );

#endif
