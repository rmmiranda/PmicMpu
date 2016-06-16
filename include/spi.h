/****************************************************************************
 * Filename:    spi.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     April 29, 2008
 *
 * Description: SPI definitions
 ****************************************************************************/
#ifndef __SPI_H__
#define __SPI_H__

#include <basetype.h>

#if defined( PROCESSOR_PMU430 )
#include "rocket_usci.h"
#elif defined( PROCESSOR_MQ31 )
#include "maxq31_sfr.h"
#else
#error SPI module not defined for this processor
#endif

// modes of operation for the SPI Module
#define SPI_SLAVE           ( 0x0 )
#define SPI_MASTER          ( 0x1 )

typedef enum {
    PMICSPI_MODULE  = 0,
    SPI_NUM_MODULES = 1
} SpiModuleId;

/**
 * Init SPI modules
 */
void SpiInit( void );

#endif


