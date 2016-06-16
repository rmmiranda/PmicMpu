/*****************************************************************************
 * Filename:    rocket_pmic_intctrl.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     June 25, 2009
 *
 * Description: Rocket PMIC interrupt controller interface
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "pmic_intctrl.h"
#include "rocket_pmic.h"
#include "api.h"
#include "spi.h"
#include "rocket_registers.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

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
    { 2, (1<<3), 2, (1<<3) },
    { 2, (1<<2), 2, (1<<2) },
    { 2, (1<<1), 2, (1<<1) },
    { 2, (1<<0), 2, (1<<0) },
    { 1, (1<<7), 4, (1<<0) },
    { 1, (1<<7), 4, (1<<1) },
    { 1, (1<<7), 4, (1<<2) },
    { 1, (1<<7), 4, (1<<3) },
    { 1, (1<<7), 4, (1<<4) },
    { 1, (1<<7), 4, (1<<5) },
    { 1, (1<<7), 4, (1<<6) },
    { 1, (1<<7), 4, (1<<7) },
    { 1, (1<<7), 5, (1<<0) },
    { 1, (1<<7), 5, (1<<1) },
    { 1, (1<<7), 5, (1<<2) },
    { 1, (1<<7), 5, (1<<3) },
    { 1, (1<<7), 5, (1<<4) },
    { 1, (1<<7), 3, (1<<0) },
    { 1, (1<<7), 3, (1<<1) },
    { 1, (1<<7), 3, (1<<2) },
    { 1, (1<<7), 3, (1<<3) },
    { 1, (1<<6), 1, (1<<6) },
    { 1, (1<<5), 1, (1<<5) },
    { 1, (1<<3), 1, (1<<3) },
    { 1, (1<<2), 1, (1<<2) },
    { 1, (1<<1), 1, (1<<2) },
    { 0, (1<<7), 0, (1<<7) },
    { 1, (1<<0), 1, (1<<0) },
    { 0, (1<<6), 0, (1<<6) },
    { 0, (1<<5), 0, (1<<5) },
    { 0, (1<<4), 0, (1<<3) },
    { 0, (1<<3), 0, (1<<3) },
    { 0, (1<<2), 0, (1<<2) },
    { 0, (1<<0), 0, (1<<0) },
    { 0, (1<<1), 0, (1<<1) },
    { 1, (1<<4), 1, (1<<4) },
    { 2, (1<<5), 2, (1<<5) },
    { 2, (1<<4), 2, (1<<4) },
    { 2, (1<<6), 2, (1<<6) }
};


/**
 * Table of MSP430 PMIC interrupt handlers, one for each IntFlag
 */
static FUNCTION Msp430PmicIntHandler[ 3 ][ 8 ];     // 3=Flag Register 8=FlagBit

/**
 * Copy of pmic interrupt mask registers seen by the MSP430 code
 */
static BYTE Msp430PmicIntMaskRegisters[ 6 ];

/**
 * Copy of pmic interrupt mask registers seen by host
 */
static BYTE HostPmicIntMaskRegisters[ 6 ];

/**
 * Copy of pmic interrupt flag registers seen by host
 */
static BYTE HostPmicIntFlagRegisters[ 3 ];

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
 * Takes the OR result of the mask bits in the MSP430 register and the host
 * register and pushes that to the actual mask bits on the PMIC
 */
static void PmicUpdateIntMaskRegisterReal( BYTE intMaskReg )
{
    BYTE val;

    val = Msp430PmicIntMaskRegisters[ intMaskReg ] & HostPmicIntMaskRegisters[ intMaskReg ];

    PmicWrite( intMaskReg + PMIC_INTMASK0, val );
}

void PmicUpdateIntMaskRegisters( void )
{
    BYTE i;

    IntctrlUpdateStat &= ~INTCTRL_UPDATE_QUEUED;
    for( i=0 ; i<6 ; i++ ) {
        PmicUpdateIntMaskRegisterReal( i );
    }
}

/**
 * Using the passed in shadow register array, returns true if the interrupt
 * is enabled based on which intMask bits are important to the interrupt flag
 * in question
 *
 * returns TRUE if interrupt is enabled, otherwise FALSE
 */
static BOOL IntIsEnabled( BYTE *intMaskRegisters, BYTE reg, BYTE bit )
{
    BOOL ret = FALSE;


    // check the "exception" cases where the reg/bitmask arn't orthogonal
    // PWRGD_FLT intflag1[7]
    if( reg == 1 && bit == 7 ) {
        // are any of the masks unmasked?
        if( (~intMaskRegisters[3] & 0x0F) ||
            (~intMaskRegisters[4] & 0xFF) ||
            (~intMaskRegisters[5] & 0x1F) ) {
            ret = TRUE;
        }
    // TPRECHG_FLT intflag1[0]  intmask1[2]
    } else if( reg == 1 && bit == 1) {
        if( ~intMaskRegisters[1] & PMIC_INTMASK1_CHGTMR ) {
            ret = TRUE;
        }
    // SYS_CHANGE intflags0[4]  intmask0[3]
    } else if( reg == 0 && bit == 4 ) {
        if( ~intMaskRegisters[0] & PMIC_INTMASK0_PWRSTAT ) {
            ret = TRUE;
        }
    } else {
        if( ~intMaskRegisters[ reg ] & (1<<bit) ) {
            ret = TRUE;
        }
    }

    return( ret );
}

/**
 * Handle the actual interrupt from the PMIC and dispatch the MSP430 handler
 * and/or the Host interrupt if they are enabled.
 */
static void PmicInterruptHandlerReal( void )
{
    BYTE reg,bit;
    BYTE mask;
    BYTE intflag[3];
    BOOL hostRequiresInterrupt = FALSE;

    // since we don't have level interrupts, clear it now so we catch any more edges.
    // If we happent to fire off a 2nd interrupt it shouldn't hurt anything.
    PadConfig( PMIC_NPWRINT, PAD_INTERRUPT_CLEAR );
    PadConfig( PMIC_NPWRINT, PAD_INTERRUPT_FALLING );

    do {
        // read flags and mask off reserved bits
        intflag[ 0 ] = PmicRead( PMIC_INTFLAGS0 ) & PMIC_INTFLAGS0_MASK;
        intflag[ 1 ] = PmicRead( PMIC_INTFLAGS1 ) & PMIC_INTFLAGS1_MASK;
        intflag[ 2 ] = PmicRead( PMIC_INTFLAGS2 ) & PMIC_INTFLAGS2_MASK;

        V1( EventLogAddN( EVENTLOG_PMIC_INTCTRL_INT, intflag[0] ) );
        V1( EventLogAddN( EVENTLOG_PMIC_INTCTRL_INT, intflag[1] ) );
        V1( EventLogAddN( EVENTLOG_PMIC_INTCTRL_INT, intflag[2] ) );

        // if we are done servicing everything, get out
        if( (intflag[0] | intflag[1] | intflag[2]) == 0 ) {
            break;
        }

        for( reg=0; reg<3 ; reg++ ) {
            mask = 0;

            // if there isn't anything set in this reg. skip it
            // and move to the next
            if( intflag[reg] == 0 ) {
                continue;
            }

            // step through each flag bit
            for( bit=1 ; bit!=0 ; bit<<=1 ) {
                // is this flag set?
                if( bit & intflag[reg] ) {
                    // fire off MSP430 handler if its enabled
                    if( IntIsEnabled( Msp430PmicIntMaskRegisters, reg, mask ) ) {
                        if( Msp430PmicIntHandler[ reg ][ mask ] != NULL ) {
                            V1( EventLogAdd( EVENTLOG_PMIC_INTCTRL_MSP430HANDLER ) );
                            Msp430PmicIntHandler[ reg ][ mask ]();
                        }
                    }

                    // set flag bit for host to read
                    HostPmicIntFlagRegisters[reg] |= bit;

                    // flag host interrupt if its enabled; actual interrupt will be
                    // triggered after MSP430 has handled any/all interrupts
                    if( IntIsEnabled( HostPmicIntMaskRegisters, reg, mask ) ) {
                        hostRequiresInterrupt = TRUE;
                    }
                }
                mask ++;
            }
        }
    } while( !PadRead( PMIC_NPWRINT ) );

    // MSP430 has handled everything it needs/wants to; notify host so it can
    // respond to what it needs to as well.
    if( hostRequiresInterrupt ) {
        ApiInterruptHost( ROCKET_INT_PMIC );
    }
}

void PmicIntCtrlInit( void )
{
    BYTE i, y;
    static volatile BYTE reg;

    // init int mask shadow registers from actual PMIC register values
    for( i=0 ; i<6 ; i++ ) {
        // reset the PMIC mask registers to match shadow
        PmicWrite( i + PMIC_INTMASK0, 0xFF );
        HostPmicIntMaskRegisters[ i ] = 0xFF;
        Msp430PmicIntMaskRegisters[ i ] = 0xFF;
    }

    HostPmicIntFlagRegisters[0] = PmicRead( PMIC_INTFLAGS0 ) & PMIC_INTFLAGS0_MASK;
    HostPmicIntFlagRegisters[1] = PmicRead( PMIC_INTFLAGS1 ) & PMIC_INTFLAGS1_MASK;
    HostPmicIntFlagRegisters[2] = PmicRead( PMIC_INTFLAGS2 ) & PMIC_INTFLAGS2_MASK;


    for( i=0 ; i<3 ; i++ ) {
        for( y=0 ; y<8 ; y++ ) {
            Msp430PmicIntHandler[i][y] = NULL;
        }
    }

    // configure interrupt signal from PMIC to MSP430
    PadRegisterIntHandler( PMIC_NPWRINT, PmicInterruptHandlerReal );
    PadConfig( PMIC_NPWRINT, PAD_INPUT_ENABLE );
    PadConfig( PMIC_NPWRINT, PAD_INTERRUPT_FALLING );

    // if we are already being interrupted, handle it
    if( PadRead( PMIC_NPWRINT ) == 0 ) {
        PmicInterruptHandlerReal();
    }
}

void PmicIntCtrlHostUninit( void )
{
    BYTE i;
    WORD wason = CoreIntDisable();

    // init int mask shadow registers for the host
    for( i=0 ; i<6 ; i++ ) {
        HostPmicIntMaskRegisters[ i ] = 0xFF;
    }

    // propogate the changed mask values to the PMIC die
    PmicUpdateIntMaskRegisters();

    CoreIntEnable( wason );
}

void PmicInterruptEnable( PmicInterrupt sig )
{
    WORD wason = CoreIntDisable();

    // force a read of the INTFLAG registers to clear any stale interrupt (incase we are about to enable it)
    PmicInterruptHandlerReal();

    // clear the mask to enable the interrupt
    Msp430PmicIntMaskRegisters[ PmicInterruptMaskMap[sig].maskReg ] &= ~PmicInterruptMaskMap[sig].maskBit;

    // update the real PMIC interrupt register
    PmicUpdateIntMaskRegisterReal( PmicInterruptMaskMap[sig].maskReg );

    CoreIntEnable( wason );

}

void PmicInterruptDisable( PmicInterrupt sig )
{
    WORD wason = CoreIntDisable();

    // set the mask to disable the interrupt
    Msp430PmicIntMaskRegisters[ PmicInterruptMaskMap[sig].maskReg ] |= PmicInterruptMaskMap[sig].maskBit;

    // update the real PMIC interrupt register
    PmicUpdateIntMaskRegisterReal( PmicInterruptMaskMap[sig].maskReg );

    CoreIntEnable( wason );
}

void PmicRegisterIntHandler( PmicInterrupt sig, FUNCTION function )
{
    BYTE x;

    if( function != NULL ) {
        x = GetBitPos( PmicInterruptMaskMap[sig].flagBit );
        Msp430PmicIntHandler[ PmicInterruptMaskMap[sig].flagReg ][ x ] = function;
    }
}


BYTE PmicIntctrlRegRead( BYTE reg )
{
    BYTE ret;

    // return shadow register contents for the intmask registers
    if( (reg >= PMIC_REG_INTMASK0) && (reg <= PMIC_REG_INTMASK5) ) {
        ret = HostPmicIntMaskRegisters[ reg-PMIC_REG_INTMASK0 ];
        return( ret );

    // return shadow register contents for intflags registers
    } else if( (reg >= PMIC_REG_INTFLAGS0) && (reg <= PMIC_REG_INTFLAGS2) ) {
        ret = HostPmicIntFlagRegisters[ reg-PMIC_REG_INTFLAGS0 ];
        // self clearing register... clear bits now
        HostPmicIntFlagRegisters[ reg-PMIC_REG_INTFLAGS0 ] = 0;

        if( (HostPmicIntFlagRegisters[0] | HostPmicIntFlagRegisters[1] | HostPmicIntFlagRegisters[2]) == 0 ) {
            // if there is nothing else interrrupting the host, clear it for the PMIC
            ApiClearInterruptHost( ROCKET_INT_PMIC );
        }

        return( ret );
    }

    // return the actual current status register values for everything else
    switch( reg ) {
        case PMIC_REG_STAT_PPCHG0:
        case PMIC_REG_STAT_PPCHG1:
        case PMIC_REG_STAT_PPCHG2:
        case PMIC_REG_STAT0:
        case PMIC_REG_STAT1:
        case PMIC_REG_STAT2:
        case PMIC_REG_STAT3:
        case PMIC_REG_STAT4:
            return( PmicRead( reg ) );
        default:
            return( 0 );
    }

}

void PmicIntctrlRegWrite( BYTE reg, BYTE value )
{
    // write to the shadow register intmask registers
    if( (reg >= PMIC_REG_INTMASK0) && (reg <= PMIC_REG_INTMASK5) ) {
        HostPmicIntMaskRegisters[ reg-PMIC_REG_INTMASK0 ] = value;

        // flag that we will need to propogate the shadow register change to the power die
        // real registers
        IntctrlUpdateStat |= INTCTRL_UPDATE_REQ;
    }

}

