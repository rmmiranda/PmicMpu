/*****************************************************************************
 * Filename:    lagavulin_gpio.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 07, 2010
 *
 * Description: Lagavulin gpio configuration routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "intvector.h"
#include "gpio.h"
#include "lagavulin_pmic.h"
#include "pmic_intctrl.h"
#include "spi.h"
#include "lagavulin_registers.h"
#include "eventlog.h"


// individual pad interrupt handlers
static FUNCTION PadHandler[PAD_PORT_TOTAL][PAD_INDEX_TOTAL];

// top level gpio interrupt handler
static void GpioHandler( void )
{
	WORD index;

	// This is an interrupt handler for selected pads:
	// 1. P0 ( MQ31 external interrupts)
	// 2. P3 and P4 ( PR66 pad interrupts )

    // reads and clears pending interrupt
	BYTE pad0IntVec = ReadRegByte( EIF0_REG8 );
	BYTE pad3IntVec = PmicRead( PMIC_EIF3 );
	BYTE pad4IntVec = PmicRead( PMIC_EIF4 );

    //EventLogAddN( EVENTLOG_PMU430_PORT1GPIO_INT, intVec );

    // make sure we have something to service
    if( (pad0IntVec == 0) && (pad3IntVec == 0) && (pad4IntVec == 0) ) {
        return;
    }

    // run the appropriate pad 0 handler
    if( pad0IntVec ) {
        for( index = 0; index < 8; index++ ) {
        	if( pad0IntVec & (1u << index) ) {
            	(PadHandler[0][index])();
        	}
        }
    }

    // run the appropriate pad 3 handler
    if( pad3IntVec ) {
        for( index = 0; index < 8; index++ ) {
        	if( pad3IntVec & (1u << index) ) {
            	(PadHandler[3][index])();
        	}
        }
    }

    // run the appropriate pad 4 handler
    if( pad4IntVec ) {
        for( index = 0; index < 8; index++ ) {
        	if( pad4IntVec & (1u << index) ) {
            	(PadHandler[4][index])();
        	}
        }
    }
}

// default pad interrupt handler
static void GpioInterruptFailure( void )
{
    // TODO: some error handling
}

void PadSupercapMode( void )
{
	return;
}

void PadConfig( Line line, PadFunction pf )
{
    BYTE padMask;
    BYTE padPort;
    BYTE padOutReg;
    BYTE padInReg;
    BYTE padDirReg;
    BYTE padEnReg;
    BYTE padIntEnReg;
    BYTE padIntESelReg;
    BYTE padIntFlagReg;
    BYTE padIntAltReg;
    WORD wason = CoreIntDisable();

    padMask = 1 << GET_INDEX_NUMBER( line );
    padPort = GET_PORT_NUMBER( line );

    switch( padPort ) {
		case 0:
			padOutReg = ReadRegByte( PO0_REG8 );
			padInReg = ReadRegByte( PI0_REG8 );
			padDirReg = ReadRegByte( PD0_REG8 );
			padEnReg = ReadRegByte( PE0_REG8 );
			padIntEnReg = ReadRegByte( EIE0_REG8 );
			padIntESelReg = ReadRegByte( EIES0_REG8 );
			padIntFlagReg = ReadRegByte( EIF0_REG8 );
			break;
		case 1:
			padOutReg = ReadRegByte( PO1_REG8 );
			padInReg = ReadRegByte( PI1_REG8 );
			padDirReg = ReadRegByte( PD1_REG8 );
			padEnReg = ReadRegByte( PE1_REG8 );
			break;
		case 3:
			padOutReg = PmicRead( PMIC_PO3 );
			padInReg = PmicRead( PMIC_PI3 );
			padDirReg = PmicRead( PMIC_PD3 );
			padEnReg = PmicRead( PMIC_PE3 );
			padIntEnReg = PmicRead( PMIC_EIE3 );
			padIntESelReg = PmicRead( PMIC_EIES3 );
			padIntFlagReg = PmicRead( PMIC_EIF3 );
			padIntAltReg = PmicRead( PMIC_AE3 );
			break;
		case 4:
			padOutReg = PmicRead( PMIC_PO4 );
			padInReg = PmicRead( PMIC_PI4 );
			padDirReg = PmicRead( PMIC_PD4 );
			padEnReg = PmicRead( PMIC_PE4 );
			padIntEnReg = PmicRead( PMIC_EIE4 );
			padIntESelReg = PmicRead( PMIC_EIES4 );
			padIntFlagReg = PmicRead( PMIC_EIF4 );
			padIntAltReg = PmicRead( PMIC_AE4 );
			break;
		default:
			break;
    }

    switch( pf ) {
        case PAD_OUTPUT_HIGH:
            // level HIGH, direction OUT, pull resistor DISABLED
            padOutReg |= padMask;
            padDirReg |= padMask;
            padEnReg &= ~padMask;
            break;
        case PAD_OUTPUT_LOW:
            // level LOW, direction OUT, pull resistor DISABLED
            padOutReg &= ~padMask;
            padDirReg |= padMask;
            padEnReg &= ~padMask;
            break;
        case PAD_INPUT_ENABLE:
            // direction IN, pull resistor DISABLED
            padDirReg &= ~padMask;
            padEnReg &= ~padMask;
            break;
        case PAD_INPUT_PULLUP:
            // level LOW, direction IN, pull resistor ENABLED
            padOutReg &= ~padMask;
            padDirReg &= ~padMask;
            padEnReg |= padMask;
            break;
        case PAD_INPUT_PULLDOWN:
            // level HIGH, direction IN, pull resistor ENABLED
            padOutReg |= padMask;
            padDirReg &= ~padMask;
            padEnReg |= padMask;
            break;
        case PAD_INTERRUPT_RISING:
        	// clear pending interrupt, edge select RISING, interrupt ENABLED
        	padIntFlagReg &= ~padMask;
        	padIntESelReg &= ~padMask;
        	padIntEnReg |= padMask;
        	break;
        case PAD_INTERRUPT_FALLING:
        	// clear pending interrupt, edge select FALLING, interrupt ENABLED
        	padIntFlagReg &= ~padMask;
        	padIntESelReg |= padMask;
        	padIntEnReg |= padMask;
        	break;
        case PAD_INTERRUPT_DISABLE:
        	// interrupt DISABLED
        	padIntEnReg &= ~padMask;
        	break;
        case PAD_INTERRUPT_CLEAR:
        	// clear pending interrupt
        	padIntFlagReg &= ~padMask;
        	break;
        case PAD_ALT_MODE:
        	// alternate mode
            padIntAltReg |= padMask;
            break;
        case PAD_STRENGTH_REDUCED:
        case PAD_STRENGTH_FULL:
        	// do nothing drive strengths are not supported
        default:
            break;

    }

    // if we arn't using ALT mode make sure we are in GPIO mode
    if( pf != PAD_ALT_MODE ) {
        padIntAltReg &= ~padMask;
    }

    switch( padPort ) {
		case 0:
	    	WriteRegByte( PO0_REG8, padOutReg );
	    	WriteRegByte( PI0_REG8, padInReg );
	    	WriteRegByte( PD0_REG8, padDirReg );
	    	WriteRegByte( PE0_REG8, padEnReg );
	    	WriteRegByte( EIE0_REG8, padIntEnReg );
	    	WriteRegByte( EIES0_REG8, padIntESelReg );
	    	WriteRegByte( EIF0_REG8, padIntFlagReg );
	    	break;
		case 1:
	    	WriteRegByte( PO1_REG8, padOutReg );
	    	WriteRegByte( PI1_REG8, padInReg );
	    	WriteRegByte( PD1_REG8, padDirReg );
	    	WriteRegByte( PE1_REG8, padEnReg );
	    	break;
		case 3:
			PmicWrite( PMIC_PO3, padOutReg );
			PmicWrite( PMIC_PI3, padInReg );
			PmicWrite( PMIC_PD3, padDirReg );
			PmicWrite( PMIC_PE3, padEnReg );
			PmicWrite( PMIC_EIE3, padIntEnReg );
			PmicWrite( PMIC_EIES3, padIntESelReg );
			PmicWrite( PMIC_EIF3, padIntFlagReg );
			PmicWrite( PMIC_AE3, padIntAltReg );
	    	break;
		case 4:
			PmicWrite( PMIC_PO4, padOutReg );
			PmicWrite( PMIC_PI4, padInReg );
			PmicWrite( PMIC_PD4, padDirReg );
			PmicWrite( PMIC_PE4, padEnReg );
			PmicWrite( PMIC_EIE4, padIntEnReg );
			PmicWrite( PMIC_EIES4, padIntESelReg );
			PmicWrite( PMIC_EIF4, padIntFlagReg );
			PmicWrite( PMIC_AE4, padIntAltReg );
	    	break;
		default:
			break;
    }

    CoreIntEnable( wason );
}

BOOL PadRead( Line line )
{
    BYTE reg;
    BYTE padMask;
    BYTE padPort;
    WORD wason = CoreIntDisable();

    padMask = 1 << GET_INDEX_NUMBER( line );
    padPort = GET_PORT_NUMBER( line );

    switch( padPort ) {
		case 0:
			reg = ReadRegByte( PI0_REG8 ) & padMask;
			break;
		case 1:
			reg = ReadRegByte( PI1_REG8 ) & padMask;
			break;
		case 3:
			reg = PmicRead( PMIC_PI3 ) & padMask;
			break;
		case 4:
			reg = PmicRead( PMIC_PI4 ) & padMask;
			break;
		default:
			break;
    }

    CoreIntEnable( wason );

    return( (reg != 0 ) );
}

BYTE PadGetState( Line line )
{
    BYTE reg = 0;
    BYTE padMask;
    BYTE padPort;
    WORD wason = CoreIntDisable();

    padMask = 1 << GET_INDEX_NUMBER( line );
    padPort = GET_PORT_NUMBER( line );

    switch( padPort ) {
		case 0:
			reg |= ((ReadRegByte( PD0_REG8 ) & padMask) ? GPIO_CONTROL_DIRECTION : 0);
			reg |= ((ReadRegByte( PI0_REG8 ) & padMask) ? GPIO_CONTROL_LEVEL : 0);
			break;
		case 1:
			reg |= ((ReadRegByte( PD1_REG8 ) & padMask) ? GPIO_CONTROL_DIRECTION : 0);
			reg |= ((ReadRegByte( PI1_REG8 ) & padMask) ? GPIO_CONTROL_LEVEL : 0);
			break;
		case 3:
			reg |= ((PmicRead( PMIC_PD3 ) & padMask) ? GPIO_CONTROL_DIRECTION : 0);
			reg |= ((PmicRead( PMIC_PI3 ) & padMask) ? GPIO_CONTROL_LEVEL : 0);
			break;
		case 4:
			reg |= ((PmicRead( PMIC_PD4 ) & padMask) ? GPIO_CONTROL_DIRECTION : 0);
			reg |= ((PmicRead( PMIC_PI4 ) & padMask) ? GPIO_CONTROL_LEVEL : 0);
			break;
		default:
			break;
    }

    CoreIntEnable( wason );

    return( reg );
}

void PadRegisterIntHandler( Line line, FUNCTION function )
{
    BYTE padIndex;
    BYTE padPort;
    WORD wason = CoreIntDisable();

    padIndex = GET_INDEX_NUMBER( line );
    padPort = GET_PORT_NUMBER( line );

    // store handler function in array for appropriate port/index
    PadHandler[ padPort ][ padIndex ] = function;

    // determine if pad interrupt will be coming from PR66
    if( padPort > 1 ) {
        // register interrupt handlers for PR66 pad interrupts
        PmicRegisterIntHandler( INT_GPIO_IRQ, GpioHandler );
    }

    CoreIntEnable( wason );
}

void GpioInit( void )
{
    BYTE padPort;
    BYTE padIndex;

    // register interrupt handlers for MQ31 external interrupts
    RegisterIntHandler( MQ31EXT_VECTOR, GpioHandler );

    // initialize any handlers that haven't been touched yet
    for( padPort = 0; padPort < PAD_PORT_TOTAL; padPort++ ) {
    	for( padIndex = 0; padIndex < PAD_INDEX_TOTAL; padIndex++ ) {
    		PadHandler[padPort][padIndex] = GpioInterruptFailure;
    	}
    }

    return;
}

