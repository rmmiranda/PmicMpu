/*****************************************************************************
 * Filename:    rocket_intvector.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     July 16, 2009
 *
 * Description: Rocket interrupt vector handling/dispatching
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "eventlog.h"
#include "intvector.h"
#include "rocket_sfr.h"

// locate the array of handlers where the MSP430 inturrupt controller is going to
// get the ISR jump address.
#pragma DATA_SECTION( InterruptHandler, ".ramvectors" )
FUNCTION InterruptHandler[NUM_INT_VECTORS];

// the actual ISRs that will be called from
FUNCTION InterruptHandlerActual[NUM_INT_VECTORS];

// these functions exist in msp430_inout.s
extern void IsrVect0( void );
extern void IsrVect1( void );
extern void IsrVect2( void );
extern void IsrVect3( void );
extern void IsrVect4( void );
extern const WORD Vect5Base;

/*
 * getIsr( n ):
 * For IsrVect0,1,2,4, the assembler generates 6 bytes by using the constant generator
 * For IsrVect3,5+, the assembler generates 8 bytes because the 16-bit constant is stored
 * So to get IsrVect5+, a word (Vect5Base) is declared at the same address as IsrVect5, and
 *   pointer arithmetic can be used to calculate the isr address.
 */
static FUNCTION getIsr( BYTE n )
{
    switch( n ) {
        case 0:
            return &IsrVect0;
        case 1:
            return &IsrVect1;
        case 2:
            return &IsrVect2;
        case 3:
            return &IsrVect3;
        case 4:
            return &IsrVect4;
        default:
            return (FUNCTION)(&Vect5Base + (n - 5) * 4);
    }
}

static __interrupt void unregisteredHandler( void )
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

    // enable the RAM interrupt vector mode so we jump to those vectors
    // and not the ones *not* in flash
    WriteRegWordOr( SYSCTL_REG16, SYSCTL_SYSRIVECT );
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

