/*****************************************************************************
 * Filename:    lagavulin_crc.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 21, 2011
 *
 * Description: Lagavulin CRC module functions
 ****************************************************************************/
#include "basetype.h"
#include "registers.h"
#include "flash.h"

WORD GenerateCrc16(BYTE *start, WORD count )
{
	WORD _start = (WORD) start;
    WORD crc = 0;

    // do crc16 (ibm variant) for the entire length
    // note: start is assumed to be the word address
    //		 and count is assumed to be in word bytes
    while( count-- ) {
    	crc = flash_crc( _start++, crc );
    }

    // return the result
    return( crc );
}
