/*****************************************************************************
 * Filename:    rocket_crc.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     August 10, 2009
 *
 * Description: MSP430 CRC module functions
 ****************************************************************************/
#include "basetype.h"
#include "registers.h"
#include "crc.h"

WORD GenerateCrc16( BYTE *start, WORD count )
{
    WORD crc = 0;
    WORD i;

    // init CRC module
    WriteRegWord( CRCINIRES_REG16, 0xFFFF );

    // feed data into CRC module
    for( i=0 ; i<count ; i++ ) {
        WriteRegByte( CRCDI_REG16, *start++ );
    }

    // get CRC result
    crc = ReadRegWord( CRCINIRES_REG16 );

    return( crc );
}

// *******************************************************
// C METHOD FOR CALCULATING THE SAME CRC AS THE MSP430 HW
// *******************************************************

/*unsigned short crc16MakeBitwiseBr(unsigned short crc, unsigned short poly,
                        unsigned char *pmsg, unsigned int msg_size);

unsigned long bitReflect(unsigned long data, unsigned int width)
{
    unsigned long result = 0;
    unsigned int i;

    for (i = 1; i < (width + 1); i++)
    {
        if(data & 1) result |= 0x1L << (width - i);
        data >>= 1;
    }

    return result;
}


// this is a C-optimized implementation
unsigned short crc16MakeBitwise2(unsigned short crc, unsigned short poly,
                        unsigned char *pmsg, unsigned int msg_size)
{
    unsigned int i, j;
    unsigned short msg;
    BYTE num;

    for(i = 0 ; i < msg_size ; i ++)
    {
        num = bitReflect( *pmsg++, 8);
        msg = num<<8;

        for(j = 0 ; j < 8 ; j++)
        {
            if((msg ^ crc) >> 15) crc = (crc << 1) ^ poly;
            else crc <<= 1;
            msg <<= 1;
        }
    }

    return(crc ^ 0x00);
}
*/

