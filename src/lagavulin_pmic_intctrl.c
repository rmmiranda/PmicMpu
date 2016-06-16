/*****************************************************************************
 * Filename:    lagavulin_pmic_intctrl.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     January 25, 2011
 *
 * Description: Lagavulin PMIC interrupt controller interface
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "intvector.h"
#include "pmic_intctrl.h"
#include "lagavulin_pmic.h"
#include "lagavulin_registers.h"
#include "api.h"
#include "spi.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

#define INTFLAG_IRQ			0
#define INTFLAG_FIQ			1
#define INTFLAG_MAX			2

#define INTMASK_IRQ			0
#define INTMASK_FIQ			1
#define INTMASK_MAX			2

typedef struct {
    BYTE flagReg;       // INTFLAG register to read
    BYTE flagBit;       // bit in register which flags the interrupt
    BYTE maskReg;       // INTMASK register to read
    BYTE maskBit;       // bit in register which masks the interrupt
} PmicFlagMaskMap;

// Maps the mask bit to the flag bit it affects, used when we receive an interrupt
// to see if we need to respond to it, and/or pass it on to the host if its not masked
// Note: must map to match the PmicInterrupt enum order
static const PmicFlagMaskMap PmicInterruptMaskMap[] = {

    { INTFLAG_IRQ, (1<<0), INTMASK_IRQ, (1<<0) }, // INT_GPIO_IRQ
    { INTFLAG_IRQ, (1<<1), INTMASK_IRQ, (1<<1) }, // INT_LIGHT_IRQ
    { INTFLAG_IRQ, (1<<2), INTMASK_IRQ, (1<<2) }, // INT_LDO_A_IRQ
    { INTFLAG_IRQ, (1<<3), INTMASK_IRQ, (1<<3) }, // INT_LDO_B_IRQ
    { INTFLAG_IRQ, (1<<4), INTMASK_IRQ, (1<<4) }, // INT_LDO_C_IRQ
    { INTFLAG_IRQ, (1<<5), INTMASK_IRQ, (1<<5) }, // INT_BUCK_IRQ
    { INTFLAG_IRQ, (1<<6), INTMASK_IRQ, (1<<6) }, // INT_CHARGER_IRQ
    { INTFLAG_IRQ, (1<<7), INTMASK_IRQ, (1<<7) }, // INT_SYSMON_IRQ

    { INTFLAG_FIQ, (1<<0), INTMASK_FIQ, (1<<0) }, // INT_GPIO_FIQ
    { INTFLAG_FIQ, (1<<1), INTMASK_FIQ, (1<<1) }, // INT_RSVD_FIQ
    { INTFLAG_FIQ, (1<<2), INTMASK_FIQ, (1<<2) }, // INT_LDO_A_FIQ
    { INTFLAG_FIQ, (1<<3), INTMASK_FIQ, (1<<3) }, // INT_LDO_B_FIQ
    { INTFLAG_FIQ, (1<<4), INTMASK_FIQ, (1<<4) }, // INT_LDO_C_FIQ
    { INTFLAG_FIQ, (1<<5), INTMASK_FIQ, (1<<5) }, // INT_BUCK_FIQ
    { INTFLAG_FIQ, (1<<6), INTMASK_FIQ, (1<<6) }, // INT_CHARGER_FIQ
    { INTFLAG_FIQ, (1<<7), INTMASK_FIQ, (1<<7) }, // INT_SYSMON_FIQ

};


/**
 * Table of core PMIC interrupt handlers, one for each IntFlag
 */
static FUNCTION CorePmicIntHandler[ INTFLAG_MAX ][ 8 ];

/**
 * Copy of pmic interrupt mask registers seen by the pmic core code
 */
static BYTE CorePmicIntMaskRegisters[ INTMASK_MAX ];

/**
 * Copy of pmic interrupt mask registers seen by host
 */
static BYTE HostPmicIntMaskRegisters[ INTMASK_MAX ];

/**
 * Copy of pmic interrupt flag registers seen by host
 */
static BYTE HostPmicIntFlagRegisters[ INTFLAG_MAX ];

BYTE IntctrlUpdateStat = 0;

static BYTE GetBitPos( BYTE bitfield )
{
    BYTE i = 0;

    while( bitfield != 0 ) {
        bitfield >>= 1;
        i++;
    }

    return (i-1);
}

/**
 * Takes the OR result of the mask bits in the core register and the host
 * register and pushes that to the actual mask bits on the PMIC
 */
static void PmicUpdateIntMaskRegisterReal( BYTE intmask )
{
    BYTE val;

    val = CorePmicIntMaskRegisters[ intmask ] & HostPmicIntMaskRegisters[ intmask ];

	switch( intmask ) {
		case INTMASK_IRQ:
	        PmicWrite( PMIC_MIRQ, val );
	        break;
		case INTMASK_FIQ:
	        PmicWrite( PMIC_MFIQ, val );
	        break;
		default:
			break;
	}
}

void PmicUpdateIntMaskRegisters( WORD data )
{
    IntctrlUpdateStat &= ~INTCTRL_UPDATE_QUEUED;

    PmicUpdateIntMaskRegisterReal( INTMASK_IRQ );
    PmicUpdateIntMaskRegisterReal( INTMASK_FIQ );
}

/**
 * Handle the actual interrupt from the PMIC and dispatch the core handler
 * and/or the Host interrupt if they are enabled.
 */
static void PmicInterruptHandlerReal( BYTE reg, BYTE intflag, BYTE intmask )
{
    BYTE bit;
    BYTE flag;
    BOOL hostRequiresInterrupt = FALSE;

    // read actual interrupt flags from PMIC
    flag = PmicRead( reg );
    if( flag == 0 ) {
    	// no interrupts found
    	return;
    }

    V1( EventLogAddN( EVENTLOG_PMIC_INTCTRL_INT, flag ) );

    if( flag & INT_MASK_CHARGER ) {
        // Clear the charger interrupt first
        PmicRead( PMIC_CHG_INT );
    }

    // step through each flag bit
    for( bit = 0; bit < 8; bit++ ) {
    	// determine if the flag bit is set
    	if( flag & (1u << bit) ) {
    		// run interrupt handler if enabled
    		if( ~CorePmicIntMaskRegisters[intmask] & (1u << bit) ) {
                if( CorePmicIntHandler[ intflag ][ bit ] != NULL ) {
                    V1( EventLogAdd( EVENTLOG_PMIC_INTCTRL_MSP430HANDLER ) );
                    CorePmicIntHandler[ intflag ][ bit ]();
                }
    		}
            // set flag bit for host to read
            HostPmicIntFlagRegisters[ intflag ] |= (1u << bit);
            // flag host interrupt if its enabled; actual interrupt will be
            // triggered after core has handled any/all interrupts
            if( ~HostPmicIntMaskRegisters[intmask] & (1u << bit) ) {
                hostRequiresInterrupt = TRUE;
            }
    	}
    }

    // core PMIC has handled everything it needs/wants to; notify host so it can
    // respond to what it needs to as well.
    if( hostRequiresInterrupt ) {
        ApiInterruptHost( ROCKET_INT_PMIC );
    }
}

/**
 * Fast interrupt handler
 */
static void PmicFiqHandler( void )
{
	BYTE intcn;

	// clear the MQ31 interrupt
	intcn = ReadRegByte( INTCN_REG8 );

	// clear the PR66 fast interrupt
	if( intcn & INTCN_FIQ ) {
		// handle all fast interrupts
	    PmicInterruptHandlerReal( PMIC_FFIQ, INTFLAG_FIQ, INTMASK_FIQ );
	}
}

/**
 * Normal interrupt handler
 */
static void PmicIrqHandler( void )
{
	BYTE intcn;

	// clear the MQ31 interrupt
	intcn = ReadRegByte( INTCN_REG8 );

	// clear the PR66 interrupt
	if( intcn & INTCN_IRQ ) {
		// handle all normal interrupts
	    PmicInterruptHandlerReal( PMIC_FIRQ, INTFLAG_IRQ, INTMASK_IRQ );
	}
}

void PmicIntCtrlInit( void )
{
    BYTE inthandler;

    // mask OFF all host PMIC shadow interrupt mask registers
    HostPmicIntMaskRegisters[ INTMASK_IRQ ] = 0xFF;
    HostPmicIntMaskRegisters[ INTMASK_FIQ ] = 0xFD;

    // mask OFF all core PMIC shadow interrupt mask registers
    CorePmicIntMaskRegisters[ INTMASK_IRQ ] = 0xFF;
    CorePmicIntMaskRegisters[ INTMASK_FIQ ] = 0xFD;

    // mask OFF all core PMIC interrupt registers
    PmicWrite( PMIC_MIRQ, 0xFF );
    PmicWrite( PMIC_MFIQ, 0xFD );

    // Set LDO23_HPD interrupt priority to IRQ
    PmicWrite( PMIC_L17_23_FIQ_IRQ, 0x0 );

    /* Just add a note here. The LDO_X interrupt at top-level will
     * be unmasked internally during HostCtrlStateInit() by setting
     * the internal mask CorePmicIntMaskRegisters[]. However, the
     * host won't see the internal mask but instead sees the external
     * mask of HostPmicIntMaskRegisters[]. So from the host point of
     * view even if LDO_X seems masked at top-level, it's actually
     * unmasked in the physical LDO_X mask register. But the host
     * still needs to unmask the external mask in order to receive
     * the PMIC interrupt.
     *
     * To unmask LDO23-HPD:
     *  PmicWrite( PMIC_L17_23_MASK, (0xFF & ~(1 << 7)) );
     *  PmicClear( PMIC_MIRQ, INT_MASK_LDO_C );
     */

	// clear all host PMIC shadow interrupt flag registers
    HostPmicIntFlagRegisters[ INTMASK_IRQ ] = 0;
    HostPmicIntFlagRegisters[ INTMASK_FIQ ] = 0;

	// clear all core PMIC block level interrupt flag registers
	PmicRead( PMIC_CHG_INT );
	PmicRead( PMIC_IRQ_FLASH1 );
	PmicRead( PMIC_IRQ_FLASH2 );
	PmicRead( PMIC_EIF3 );
	PmicRead( PMIC_EIF4 );
	PmicRead( PMIC_EFIF3 );
	PmicRead( PMIC_EFIF4 );
	PmicRead( PMIC_INTFLAGSYSIRQ );
	PmicRead( PMIC_INTFLAGSYSFIQ );
	PmicRead( PMIC_BUCK_IRQ );
	PmicRead( PMIC_L01_08_LVL2_FIQ );
	PmicRead( PMIC_L09_16_LVL2_FIQ );
	PmicRead( PMIC_L17_23_LVL2_FIQ );
	PmicRead( PMIC_L01_08_LVL2_IRQ );
	PmicRead( PMIC_L09_16_LVL2_IRQ );
	PmicRead( PMIC_L17_23_LVL2_IRQ );
	PmicRead( PMIC_LEDBST_IRQ );
	PmicRead( PMIC_RGBLED_IRQ_ST );

	// initialize individual interrupt handlers to NULL
    for( inthandler = 0 ; inthandler < 8 ; inthandler++ ) {
    	CorePmicIntHandler[ INTFLAG_IRQ ][ inthandler ] = NULL;
    	CorePmicIntHandler[ INTFLAG_FIQ ][ inthandler ] = NULL;
    }

    // register for interrupt handling
    RegisterIntHandler( PR66FIQ_VECTOR, PmicFiqHandler );
    RegisterIntHandler( PR66IRQ_VECTOR, PmicIrqHandler );

    // enable the interrupts
    WriteRegByte( INTCN_REG8, INTCN_FIQIE | INTCN_IRQIE );
}

void PmicIntCtrlHostUninit( void )
{
    WORD wason = CoreIntDisable();

    // re-initialize shadow interrupt mask registers for the host
    HostPmicIntMaskRegisters[ INTMASK_IRQ ] = 0xFF;
    HostPmicIntMaskRegisters[ INTMASK_FIQ ] = 0xFD;

    // propagate the changed mask values to the PMIC
    PmicUpdateIntMaskRegisters( 0 );

    CoreIntEnable( wason );
}

void PmicInterruptEnable( PmicInterrupt sig )
{
    WORD wason = CoreIntDisable();

    // clear the mask to enable the interrupt
    CorePmicIntMaskRegisters[ PmicInterruptMaskMap[sig].maskReg ] &= ~PmicInterruptMaskMap[sig].maskBit;

    // update the real PMIC interrupt register
    PmicUpdateIntMaskRegisterReal( PmicInterruptMaskMap[sig].maskReg );

    CoreIntEnable( wason );

}

void PmicInterruptDisable( PmicInterrupt sig )
{
    WORD wason = CoreIntDisable();

    // set the mask to disable the interrupt
    CorePmicIntMaskRegisters[ PmicInterruptMaskMap[sig].maskReg ] |= PmicInterruptMaskMap[sig].maskBit;

    // update the real PMIC interrupt register
    PmicUpdateIntMaskRegisterReal( PmicInterruptMaskMap[sig].maskReg );

    CoreIntEnable( wason );
}

void PmicRegisterIntHandler( PmicInterrupt sig, FUNCTION function )
{
    BYTE x;

    if( function != NULL ) {
        x = GetBitPos( PmicInterruptMaskMap[sig].flagBit );
        CorePmicIntHandler[ PmicInterruptMaskMap[sig].flagReg ][ x ] = function;
    }
}


BYTE PmicIntctrlRegRead( BYTE reg )
{
    BYTE ret;

    switch( reg ) {

    	// return shadow register contents for the int mask registers
		case PMIC_MIRQ:
			ret = HostPmicIntMaskRegisters[ INTMASK_IRQ ];
			break;
		case PMIC_MFIQ:
			ret = HostPmicIntMaskRegisters[ INTMASK_FIQ ];
			break;

	    // return shadow register contents for int flags registers
		case PMIC_FIRQ:
			ret = HostPmicIntFlagRegisters[ INTFLAG_IRQ ];
			HostPmicIntFlagRegisters[ INTFLAG_IRQ ] = 0;
            ApiClearInterruptHost( ROCKET_INT_PMIC );
			break;
		case PMIC_FFIQ:
			ret = HostPmicIntFlagRegisters[ INTFLAG_FIQ ];
			HostPmicIntFlagRegisters[ INTFLAG_FIQ ] = 0;
            ApiClearInterruptHost( ROCKET_INT_PMIC );
			break;

		// return the actual current status register values for everything else
		default:
			return( PmicRead( reg ) );
    }

    return( ret );
}

void PmicIntctrlRegWrite( BYTE reg, BYTE value )
{
    // write to the shadow register int mask registers
	switch( reg ) {
		case PMIC_MIRQ:
			HostPmicIntMaskRegisters[ INTMASK_IRQ ] = value;
			break;
		case PMIC_MFIQ:
			HostPmicIntMaskRegisters[ INTMASK_FIQ ] = value;
			break;
		default:
			return;
	}

    // flag that we will need to propagate the shadow register change to the power die
    // real registers
    IntctrlUpdateStat |= INTCTRL_UPDATE_REQ;
}

