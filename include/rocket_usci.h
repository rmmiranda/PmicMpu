/*****************************************************************************
 * Filename:    rocket_usci.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Dec 11, 2008
 *
 * Description: Definitions of USCI module registers
 ****************************************************************************/
#ifndef __ROCKET_USCI_H__
#define __ROCKET_USCI_H__

#if defined( PROCESSOR_PMU430 )

// Each module has a base address
#define USCI0_BASE              ( 0x5C0 )
#define USCI1_BASE              ( 0x600 )   // FIXME: This needs to be verified with rocket spec update from Todd
#define USCI2_BASE              ( 0x640 )   // FIXME: This needs to be verified with rocket spec update from Todd

// each module has an A and a B sub-module
// A Modes: SPI, UART, IrDA
// B Modes: SPI, I2C
#define USCI0A_BASE             ( USCI0_BASE )
#define USCI0B_BASE             ( USCI0_BASE + 0x20 )
#define USCI1A_BASE             ( USCI1_BASE )
#define USCI1B_BASE             ( USCI1_BASE + 0x20 )
#define USCI2A_BASE             ( USCI2_BASE )
#define USCI2B_BASE             ( USCI2_BASE + 0x20 )

#else
 #error Unknown USCI register definitions
#endif

// USCI A Registers
#define UCACTL0_REG8(x)         ( (x) + 0x01 )
#define UCACTL1_REG8(x)         ( (x) + 0x00 )
#define UCABR0_REG8(x)          ( (x) + 0x06 )
#define UCABR1_REG8(x)          ( (x) + 0x07 )
#define UCAMCTL_REG8(x)         ( (x) + 0x08 )
#define UCASTAT_REG8(x)         ( (x) + 0x0A )
#define UCARXBUF_REG8(x)        ( (x) + 0x0C )
#define UCATXBUF_REG8(x)        ( (x) + 0x0E )
#define UCAABCTL_REG8(x)        ( (x) + 0x10 )
#define UCAIRTCTL_REG8(x)       ( (x) + 0x12 )
#define UCAIRRCTL_REG8(x)       ( (x) + 0x13 )
#define UCAIE_REG8(x)           ( (x) + 0x1C )
#define UCAIFG_REG8(x)          ( (x) + 0x1D )
#define UCAIV_REG16(x)          ( (x) + 0x1E )


// USCI B Registers
#define UCBCTL0_REG8(x)         ( (x) + 0x01 )          // TODO: this could be backwords for A and B modules?
#define UCBCTL1_REG8(x)         ( (x) + 0x00 )          // TODO: this could be backwords for A and B modules?
#define UCBBR_REG16(x)          ( (x) + 0x06 )
#define UCBBR0_REG8(x)          ( (x) + 0x06 )
#define UCBBR1_REG8(x)          ( (x) + 0x07 )
#define UCBSTAT_REG8(x)         ( (x) + 0x0A )
#define UCBRXBUF_REG8(x)        ( (x) + 0x0C )
#define UCBTXBUF_REG8(x)        ( (x) + 0x0E )
#define UCBIE_REG8(x)           ( (x) + 0x1C )
#define UCBIFG_REG8(x)          ( (x) + 0x1D )
#define UCBIV_REG16(x)          ( (x) + 0x1E )
#define UCBI2COA_REG16(x)       ( (x) + 0x10 )
#define UCBI2CSA_REG16(x)       ( (x) + 0x12 )

// USCI SPI Registers
#define UCCTL0_REG8(x)          ( (x) + 0x01 )
#define UCCTL1_REG8(x)          ( (x) + 0x00 )
#define UCBR0_REG8(x)           ( (x) + 0x06 )
#define UCBR1_REG8(x)           ( (x) + 0x07 )
#define UCMCTL_REG8(x)          ( (x) + 0x08 )
#define UCSTAT_REG8(x)          ( (x) + 0x0A )
#define UCRXBUF_REG8(x)         ( (x) + 0x0C )
#define UCTXBUF_REG8(x)         ( (x) + 0x0E )
#define UCABCTL_REG8(x)         ( (x) + 0x10 )
#define UCIRTCTL_REG8(x)        ( (x) + 0x12 )
#define UCIRRCTL_REG8(x)        ( (x) + 0x13 )
#define UCIE_REG8(x)            ( (x) + 0x1C )
#define UCIFG_REG8(x)           ( (x) + 0x1D )
#define UCIV_REG16(x)           ( (x) + 0x1E )

// USCI I2C Registers
// TODO: convert i2c driver to use these registers instead of only B module ones
#define UCCTL0_REG8(x)          ( (x) + 0x01 )          // TODO: this could be backwords for A and B modules?
#define UCCTL1_REG8(x)          ( (x) + 0x00 )          // TODO: this could be backwords for A and B modules?
#define UCBR0_REG8(x)           ( (x) + 0x06 )
#define UCBR1_REG8(x)           ( (x) + 0x07 )
#define UCSTAT_REG8(x)          ( (x) + 0x0A )
#define UCRXBUF_REG8(x)         ( (x) + 0x0C )
#define UCTXBUF_REG8(x)         ( (x) + 0x0E )
#define UCIE_REG8(x)            ( (x) + 0x1C )
#define UCIFG_REG8(x)           ( (x) + 0x1D )
#define UCIV_REG16(x)           ( (x) + 0x1E )
#define UCI2COA_REG16(x)        ( (x) + 0x10 )
#define UCI2CSA_REG16(x)        ( (x) + 0x12 )


// USCI SPI Bit Defines
#define UCSCTL0_UCCKPH          ( 1<<7 )
#define UCSCTL0_UCCKPL          ( 1<<6 )
#define UCSCTL0_UCMSB           ( 1<<5 )
#define UCSCTL0_7BIT            ( 1<<4 )
#define UCSCTL0_UCMST           ( 1<<3 )
#define UCCSTL0_UCMODE_MASK     ( 3<<1 )
#define UCCSTL0_UCMODE_3PSPI    ( 0<<1 )
#define UCSCTL0_UCMODE_4PSPI_AH ( 1<<1 )
#define UCSCTL0_UCMODE_4PSPI_AL ( 2<<1 )
#define UCSCTL0_UCMODE_I2C      ( 3<<1 )
#define UCSCTL0_UCSYNC          ( 1<<0 )

#define UCSCTL1_UCSSEL_MASK     ( 3<<6 )
#define UCSCTL1_UCSSEL_NA       ( 0<<6 )
#define UCSCTL1_UCSSEL_ACLK     ( 1<<6 )
#define UCSCTL1_UCSSEL_SMCLK    ( 2<<6 )
//#define UCSCTL1_UCSSEL_SMCLK    ( 3<<6 )
#define UCSCTL1_UCSWRST         ( 1<<0 )

#define UCSSTAT_UCLISTEN        ( 1<<7 )
#define UCSSTAT_UCFE            ( 1<<6 )
#define UCSSTAT_UCOE            ( 1<<5 )
#define UCSSTAT_UCBUSY          ( 1<<0 )

#define UCSIE_UCTXIE            ( 1<<1 )
#define UCSIE_UCRXIE            ( 1<<0 )

#define UCSIFG_UCTXIFG          ( 1<<1 )
#define UCSIFG_UCRXIFG          ( 1<<0 )

#define UCSIV_UCRXIFG           ( 0x02 )
#define UCSIV_UCTXIFG           ( 0x04 )


// USCI B I2C Bit Defines
#define UCBCTL0_UCA10           ( 1<<7 )
#define UCBCTL0_UCSLA10         ( 1<<6 )
#define UCBCTL0_UCMM            ( 1<<5 )
#define UCBCTL0_UCMST           ( 1<<3 )
#define UCBCTL0_UCMODE_MASK     ( 3<<1 )
#define UCBCTL0_UCMODE_I2C      ( 3<<1 )
#define UCBCTL0_UCSYNC          ( 1<<0 )

#define UCBCTL1_UCSSEL_UCLKI    ( 0<<6 )
#define UCBCTL1_UCSSEL_ACLK     ( 1<<6 )
#define UCBCTL1_UCSSEL_SMCLK    ( 2<<6 )
#define UCBCTL1_UCSSEL_SMCLK2   ( 3<<6 )
#define UCBCTL1_UCTR            ( 1<<4 )
#define UCBCTL1_UCTXNACK        ( 1<<3 )
#define UCBCTL1_UCTXSTP         ( 1<<2 )
#define UCBCTL1_UCTXSTT         ( 1<<1 )
#define UCBCTL1_UCSWRST         ( 1<<0 )

#define UCBSTAT_UCSCLLOW        ( 1<<6 )
#define UCBSTAT_UCGC            ( 1<<5 )
#define UCBSTAT_UCBBUSY         ( 1<<4 )

#define UCBI2COA_UCGCEN         ( 1<<15 )

#define UCBIE_UCNACKIE          ( 1<<5 )
#define UCBIE_UCALIE            ( 1<<4 )
#define UCBIE_UCSTPIE           ( 1<<3 )
#define UCBIE_UCSTTIE           ( 1<<2 )
#define UCBIE_UCTXIE            ( 1<<1 )
#define UCBIE_UCRXIE            ( 1<<0 )

#define UCBIFG_UCNACKIFG        ( 1<<5 )
#define UCBIFG_UCALIFG          ( 1<<4 )
#define UCBIFG_UCSTPIFG         ( 1<<3 )
#define UCBIFG_UCSTTIFG         ( 1<<2 )
#define UCBIFG_UCTXIFG          ( 1<<1 )
#define UCBIFG_UCRXIFG          ( 1<<0 )

#define UCBIV_NONE              ( 0x00 )
#define UCBIV_UCALIFG           ( 0x02 )
#define UCBIV_UCNACKIFG         ( 0x04 )
#define UCBIV_UCSTTIFG          ( 0x06 )
#define UCBIV_UCSTPIFG          ( 0x08 )
#define UCBIV_UCRXIFG           ( 0x0A )
#define UCBIV_UCTXIFG           ( 0x0C )



#endif /* __ROCKET_USCI_H__ */

