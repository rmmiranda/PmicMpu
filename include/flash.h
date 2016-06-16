/*****************************************************************************
 * Filename:    flash.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 21, 2011
 *
 * Description: Flash module definitions (renamed from msp430_flash.h which
 * 				had been authored by Chris Book and created on May 25, 2009).
 ****************************************************************************/

#ifndef __FLASH_H__
#define __FLASH_H__

#if defined( PROCESSOR_PMU430 )

 #define FLASH_BASE_ADDR         0x140
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

#elif defined( PROCESSOR_MQ31 )

 WORD flash_writeWord(WORD addr, WORD data);
 WORD flash_readWord(WORD addr);
 WORD flash_crc(WORD addr, WORD crc);
 WORD flash_erasePage(WORD addr);

#else

 #error Flash module not defined for this processor

#endif

#endif
