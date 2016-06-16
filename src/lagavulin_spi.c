/*****************************************************************************
 * Filename:    lagavulin_spi.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 15, 2010
 *
 * Description: Lagavulin SPI Routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "intvector.h"
#include "gpio.h"
#include "spi.h"
#include "delay.h"
#include "eventlog.h"

// takes approx. 12uS to complete @ a 4Mhz CLK
void PmicWrite( BYTE addr, BYTE data )
{
    WORD wason = CoreIntDisable();
    WORD header = (1 << 15) | (addr << 5);

    // reset fifos
    WriteRegWordOr( SPICN2_REG16, SPICN_SPIRXRST | SPICN_SPITXRST );

    // slave select pin -> ACTIVE
    PadConfig( PMIC_SPI_CS, PAD_OUTPUT_HIGH );

    // initiate write transfer
    WriteRegWord( SPIB2_REG16, header >> 8 );
    WriteRegWord( SPIB2_REG16, header & 0x00FF );
    WriteRegWord( SPIB2_REG16, data );

    // wait until spi transmit is complete
    while( (ReadRegWord( SPIST2_REG16 ) & SPIST_SPITXI) == 0 );

    // slave select pin -> INACTIVE
    PadConfig( PMIC_SPI_CS, PAD_OUTPUT_LOW );

    CoreIntEnable( wason );
}

// takes approx. 12uS to complete @ a 4Mhz CLK
BYTE PmicRead( BYTE addr  )
{
    BYTE data;
    WORD wason = CoreIntDisable();
    WORD header = addr << 5;

    // reset fifos
    WriteRegWordOr( SPICN2_REG16, SPICN_SPIRXRST | SPICN_SPITXRST );

    // slave select pin -> ACTIVE
    PadConfig( PMIC_SPI_CS, PAD_OUTPUT_HIGH );

    // initiate read transfer
    WriteRegWord( SPIB2_REG16, header >> 8 );
    WriteRegWord( SPIB2_REG16, header & 0x00FF );
    WriteRegWord( SPIB2_REG16, 0x00FF );

    // wait until spi transmit is complete
    while( (ReadRegWord( SPIST2_REG16 ) & SPIST_SPITXI) == 0 );

    // slave select pin -> INACTIVE
    PadConfig( PMIC_SPI_CS, PAD_OUTPUT_LOW );

    // transmit is complete so clear the flag
    WriteRegWordAnd( SPIST2_REG16, ~SPIST_SPITXI );

    // retrieve response data
    while( ReadRegWord( SPIST2_REG16 ) & SPIST_SPIRXI ) {
        data = ReadRegByte( SPIB2_REG16 );
    }

    CoreIntEnable( wason );
    return( data );
}

void SpiInit( void )
{
    WORD wason = CoreIntDisable();

//    RegisterIntHandler( SPI2_VECTOR, Spi2RxTxIsr );

    // initialize SPI module registers to default values
	WriteRegWord( SPIB2_REG16, 0x0000 );
	WriteRegWord( SPICN2_REG16, 0x0000 );
	WriteRegWord( SPICF2_REG16, 0x0000 );
	WriteRegByte( SPICK2_REG8, 0x00 );

    // set character length to 8 bits, slave select pin
	// to active high and sample data on the inactive edge
	WriteRegWordOr( SPICF2_REG16, SPICF_CKPHA | SPICF_SAS );

    // derive 6MHz from sysclk (12MHz)
//	WriteRegByteOr( SPICK2_REG8, 0x01 );

    // set to master mode, reset fifos and enable module
	WriteRegWordOr( SPICN2_REG16, SPICN_SPIEN | SPICN_MSTM | SPICN_SPIRXRST | SPICN_SPITXRST );

    CoreIntEnable( wason );
}
