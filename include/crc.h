/*****************************************************************************
 * Filename:    crc.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:    March 21, 2011
 *
 * Description: CRC module defines (renamed from rocket_crc.h which has been
 * 				authored by Bryon Hummel and created on August 10, 2009).
 ****************************************************************************/
#ifndef __CRC_H__
#define __CRC_H__

#if defined( PROCESSOR_PMU430 )
 #define CRC_BASE        (0x150)
 #define CRCDI_REG16     ( CRC_BASE + 0x00 )
 #define CRCDIRB_REG16   ( CRC_BASE + 0x02 )
 #define CRCINIRES_REG16 ( CRC_BASE + 0x04 )
 #define CRCRESR_REG16   ( CRC_BASE + 0x06 )
#elif defined( PROCESSOR_MQ31 )
 // no necessary definitions are needed
#else
 #error CRC Module not defined for this processor
#endif


WORD GenerateCrc16( BYTE *start, WORD count );

#endif /* __CRC_H__ */
