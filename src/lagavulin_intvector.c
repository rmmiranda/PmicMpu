/*****************************************************************************
 * Filename:    lagavulin_intvector.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 10, 2010
 *
 * Description: Lagavulin interrupt vector handling/dispatching
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "eventlog.h"
#include "intvector.h"

// locate the array of handlers where the MSP430 inturrupt controller is going to
// get the ISR jump address.
//#pragma DATA_SECTION( InterruptHandler, ".ramvectors" )
FUNCTION InterruptHandler[NUM_INT_VECTORS];

// the actual ISRs that will be called from
FUNCTION InterruptHandlerActual[NUM_INT_VECTORS];

/*
 * getIsr( n ):
 * The Interrupt Vector (IV) register is fixed at FG+0020h and acts as the vector
 * table base location. The interrupt vectors are preset at sixteen fixed memory
 * address offests from IV.
 */
static FUNCTION getIsr( BYTE n )
{
	// get the vector table base location
	WORD vector = ReadRegWord( IV_REG16 );

	// compute the module vection offset. each
	// vector is separated by 4 words (8 bytes)
	WORD offset = (n * 8);

	// return module vector address
	return (FUNCTION) (vector + offset);
}

static void unregisteredHandler( void )
{
    // Log it in eventlog and print vector if BOOT code exists (eventlog code exists).
    // Possibly reset?
    EventLogAdd( EVENTLOG_PMU430_UNHANDLED_INT );
}

void InterruptControllerInit( void )
{
    BYTE i;

    for( i=0 ; i<NUM_INT_VECTORS ; i++ ) {
        InterruptHandler[ i ] = unregisteredHandler;
    }
}

void RegisterIntHandler( IntId intId, FUNCTION handler )
{
    if( intId >= NUM_INT_VECTORS ) {
        return;
    }

    if( handler == NULL ) {
        return;
    }

    InterruptHandlerActual[ intId ] = handler;
    InterruptHandler[ intId ] = getIsr( intId );
}

void RegisterIntHandlerReal( IntId intId, FUNCTION handler )
{
    if( intId >= NUM_INT_VECTORS ) {
        return;
    }

    if( handler == NULL ) {
        return;
    }

    InterruptHandler[ intId ] = handler;
}
