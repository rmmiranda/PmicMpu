/*****************************************************************************
 * Filename:    rocket_ucs.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Dec 8, 2008
 *
 * Description: Unified Clock System module register definitions
 ****************************************************************************/
#ifndef __ROCKET_UCS_H__
#define __ROCKET_UCS_H__

#if defined( PROCESSOR_PMU430 )
 #define USCCTL_BASE             ( 0x160 )
#else
 #error UCS module not defined for this processor
#endif

#define UCSCTL0_REG16           ( USCCTL_BASE + 0x00 )
#define UCSCTL1_REG16           ( USCCTL_BASE + 0x02 )
#define UCSCTL2_REG16           ( USCCTL_BASE + 0x04 )
#define UCSCTL3_REG16           ( USCCTL_BASE + 0x06 )
#define UCSCTL4_REG16           ( USCCTL_BASE + 0x08 )
#define UCSCTL5_REG16           ( USCCTL_BASE + 0x0A )
#define UCSCTL6_REG16           ( USCCTL_BASE + 0x0C )
#define UCSCTL7_REG16           ( USCCTL_BASE + 0x0E )
#define UCSCTL8_REG16           ( USCCTL_BASE + 0x10 )


/* UCSCTL3 - FLL source and divider control */
#define UCSCTL3_SELREF_XT1CLK   ( 0<<4 )
#define UCSCTL3_SELREF_REFOCLK  ( 2<<4 )
#define UCSCTL3_SELREF_XT2CLK   ( 5<<4 )
#define UCSCTL3_FLLREFDIV_MASK  ( 7<<0 )


/* UCSCTL4 - Clock source control ACLK, SCLK, MCLK */
#define UCSCTL4_SELA_MASK       ( 7<<8 )
#define UCSCTL4_SELA_XT1CLK     ( 0<<8 )
#define UCSCTL4_SELA_VLOCLK     ( 1<<8 )
#define UCSCTL4_SELA_REFOCLK    ( 2<<8 )
#define UCSCTL4_SELA_DCOCLK     ( 3<<8 )
#define UCSCTL4_SELA_DCOCLKDIV  ( 4<<8 )
#define UCSCTL4_SELA_XT2CLK     ( 5<<8 )

#define UCSCTL4_SELS_MASK       ( 7<<4 )
#define UCSCTL4_SELS_XT1CLK     ( 0<<4 )
#define UCSCTL4_SELS_VLOCLK     ( 1<<4 )
#define UCSCTL4_SELS_REFOCLK    ( 2<<4 )
#define UCSCTL4_SELS_DCOCLK     ( 3<<4 )
#define UCSCTL4_SELS_DCOCLKDIV  ( 4<<4 )
#define UCSCTL4_SELS_XT2CLK     ( 5<<4 )

#define UCSCTL4_SELM_MASK       ( 7<<0 )
#define UCSCTL4_SELM_XT1CLK     ( 0<<0 )
#define UCSCTL4_SELM_VLOCLK     ( 1<<0 )
#define UCSCTL4_SELM_REFOCLK    ( 2<<0 )
#define UCSCTL4_SELM_DCOCLK     ( 3<<0 )
#define UCSCTL4_SELM_DCOCLKDIV  ( 4<<0 )
#define UCSCTL4_SELM_XT2CLK     ( 5<<0 )

/* UCSCTL5 - Clock source divider control ACLK_External, ACLK, SCLK, MCLK */
#define UCSCTL5_DIVPA_MASK      ( 7<<12 )
#define UCSCTL5_DIVA_MASK       ( 7<<8 )
#define UCSCTL5_DIVS_MASK       ( 7<<4 )
#define UCSCTL5_DIVM_MASK       ( 7<<0 )

/* UCSCTL6 - Drive Str, BypassEn, On/Off, Capacitor Selection for XT1 & XT2 */

#define UCSCTL6_XT2DRIVE_MIN    ( 0<<14 )
#define UCSCTL6_XT2DRIVE_LOW    ( 1<<14 )
#define UCSCTL6_XT2DRIVE_HI     ( 2<<14 )
#define UCSCTL6_XT2DRIVE_MAX    ( 3<<14 )
#define UCSCTL6_XT2BYPASS       ( 1<<12 )
#define UCSCTL6_XT2OFF          ( 1<<8 )
#define UCSCTL6_XT1DRIVE_MIN    ( 0<<6 )
#define UCSCTL6_XT1DRIVE_LOW    ( 1<<6 )
#define UCSCTL6_XT1DRIVE_HI     ( 2<<6 )
#define UCSCTL6_XT1DRIVE_MAX    ( 3<<6 )
#define UCSCTL6_XT1XTS          ( 1<<5 )
#define UCSCTL6_XT1BYPASS       ( 1<<4 )
#define UCSCTL6_XT1CAP_2PF      ( 0<<2 )
#define UCSCTL6_XT1CAP_6PF      ( 1<<2 )
#define UCSCTL6_XT1CAP_9PF      ( 2<<2 )
#define UCSCTL6_XT1CAP_12PF     ( 3<<2 )
#define UCSCTL6_SMCLKOFF        ( 1<<1 )        // User guide logic seems wrong ... BitSet=SMLCLK off i think
#define UCSCTL6_XT1OFF          ( 1<<0 )

#define UCSCTL7_XT2OFFG         ( 1<<3 )
#define UCSCTL7_XT1HFOFFG       ( 1<<2 )
#define UCSCTL7_XT1LFOFFG       ( 1<<1 )
#define UCSCTL7_DCOFFG          ( 1<<0 )

/* UCSCTL8 - Condition ReqEN for MCLK, SMCLK, ACLK, MODOSC */
#define UCSCTL8_MODOSC_REQEN    ( 1<<3 )
#define UCSCTL8_SMCLK_REQEN     ( 1<<2 )
#define UCSCTL8_MCLK_REQEN      ( 1<<1 )
#define UCSCTL8_ACLK_REQEN      ( 1<<0 )

#if defined( PROCESSOR_PMU430 )
 #define PMM_BASE                ( 0x120 )
#else
 #error PMM module not defined for this processor
#endif

#define PM5CTL0_REG16           ( PMM_BASE + 0x10 )
#define PMMIE_REG16             ( PMM_BASE + 0x0E )
#define PMMIFG_REG16            ( PMM_BASE + 0x0C )
#define SVSMIO_REG16            ( PMM_BASE + 0x08 )
#define SVSMLCTL_REG16          ( PMM_BASE + 0x06 )
#define SVSMHCTL_REG16          ( PMM_BASE + 0x04 )
#define PMMCTL1_REG16           ( PMM_BASE + 0x02 )
#define PMMCTL0_REG16           ( PMM_BASE + 0x00 )
#define PMMCTL0_H_REG8          ( PMM_BASE + 0x01 )
#define PMMCTL0_L_REG8          ( PMM_BASE + 0x00 )

#define PMMCTL0_PMMKEY          ( 0xA5u<<8 )
#define PMMCTL0_H_PMMKEY        ( 0xA5 )
#define PMMCTL0_PMMREGOFF       ( 1<<4 )
#define PMMCTL0_PMMSWPOR        ( 1<<3 )
#define PMMCTL0_PMMSWBOR        ( 1<<2 )
#define PMMCTL0_PMMCOREV_MASK   ( 3<<0 )
#define PMMCTL0_PMMCOREV_LVL0   ( 0<<0 )        // See data sheet for actual voltage level (may change on Rocket?)
#define PMMCTL0_PMMCOREV_LVL1   ( 1<<0 )
#define PMMCTL0_PMMCOREV_LVL2   ( 2<<0 )
#define PMMCTL0_PMMCOREV_LVL3   ( 3<<0 )

#define PMMCTL1_PMMCMD_MASK     ( 3<<4 )
#define PMMCTL1_PMMCMD_AUTO     ( 0<<4 )
#define PMMCTL1_PMMCMD_NORM     ( 2<<4 )
#define PMMCTL1_PMMCMD_FULL     ( 3<<4 )
#define PMMCTL1_PMMREFMD        ( 1<<0 )

#define PMMIFG_PMMRSTLPM5IFG    ( 1<<15 )
#define PMMIFG_SVSLIFG          ( 1<<13 )
#define PMMIFG_SVSHIFG          ( 1<<12 )
#define PMMIFG_PMMPORIFG        ( 1<<10 )
#define PMMIFG_PMMRSTIFG        ( 1<<9 )
#define PMMIFG_PMMBORIFG        ( 1<<8 )
#define PMMIFG_SVMHVLRIFG       ( 1<<6 )
#define PMMIFG_SVMHIFG          ( 1<<5 )
#define PMMIFG_SVSMHDLYIFG      ( 1<<4 )
#define PMMIFG_SVMLVLRIFG       ( 1<<2 )
#define PMMIFG_SVMLIFG          ( 1<<1 )
#define PMMIFG_SVSMLDLYIFG      ( 1<<0 )

#define PMMRIE_SVMHVLRPE        ( 1<<13 )
#define PMMRIE_SVSHPE           ( 1<<12 )
#define PMMRIE_SVMLVLRPE        ( 1<<9 )
#define PMMRIE_SVSLPE           ( 1<<8 )
#define PMMRIE_SVMHVLRIE        ( 1<<6 )
#define PMMRIE_SVMHIE           ( 1<<5 )
#define PMMRIE_SVSMHDLYIE       ( 1<<4 )
#define PMMRIE_SVMLVLRIE        ( 1<<2 )
#define PMMRIE_SVMLIE           ( 1<<1 )
#define PMMRIE_SVSMLDLYIE       ( 1<<0 )

#define SVSMHCTL_SVMHFP         ( 1<<15 )
#define SVSMHCTL_SVMHE          ( 1<<14 )
#define SVSMHCTL_SVMHOVPE       ( 1<<12 )
#define SVSMHCTL_SVSHFP         ( 1<<11 )
#define SVSMHCTL_SVSHE          ( 1<<10 )
#define SVSMHCTL_SVSHRVL_MASK   ( 3<<8 )        // TODO: see data sheet for voltage levels
#define SVSMHCTL_SVSHRVL(n)     ( (n)<<8 )
#define SVSMHCTL_SVSHRVL_LVL0   ( 0<<8 )
#define SVSMHCTL_SVSHRVL_LVL1   ( 1<<8 )
#define SVSMHCTL_SVSHRVL_LVL2   ( 2<<8 )
#define SVSMHCTL_SVSHRVL_LVL3   ( 3<<8 )

#define SVSMHCTL_SVSMHACE       ( 1<<7 )
#define SVSMHCTL_SVSMHEVM       ( 1<<6 )
#define SVSMHCTL_SVSHMD         ( 1<<4 )
#define SVSMHCTL_SVSMHDLYST     ( 1<<3 )
#define SVSMHCTL_SVSMHRRL_MASK  ( 7<<0 )        // TODO: see data sheet for voltage levels
#define SVSMHCTL_SVSMHRRL(n)    ( (n)<<0 )
#define SVSMHCTL_SVSMHRRL_LVL0  ( 0<<0 )
#define SVSMHCTL_SVSMHRRL_LVL1  ( 1<<0 )
#define SVSMHCTL_SVSMHRRL_LVL2  ( 2<<0 )
#define SVSMHCTL_SVSMHRRL_LVL3  ( 3<<0 )
#define SVSMHCTL_SVSMHRRL_LVL4  ( 4<<0 )
#define SVSMHCTL_SVSMHRRL_LVL5  ( 5<<0 )
#define SVSMHCTL_SVSMHRRL_LVL6  ( 6<<0 )
#define SVSMHCTL_SVSMHRRL_LVL7  ( 7<<0 )



#define SVSMLCTL_SVMLFP          ( 1<<15 )
#define SVSMLCTL_SVMLE           ( 1<<14 )
#define SVSMLCTL_SVMLOVPE        ( 1<<12 )
#define SVSMLCTL_SVSLFP          ( 1<<11 )
#define SVSMLCTL_SVSLE           ( 1<<10 )
#define SVSMLCTL_SVSLRVL_MASK    ( 3<<8 )
#define SVSMLCTL_SVSLRVL(n)      ( (n)<<8 )
#define SVSMLCTL_SVSMLACE        ( 1<<7 )
#define SVSMLCTL_SVSMLEVM        ( 1<<6 )
#define SVSMLCTL_SVSLMD          ( 1<<4 )
#define SVSMLCTL_SVSMLDLYST      ( 1<<3 )
#define SVSMLCTL_SVSMLRRL_MASK   ( 7<<0 )
#define SVSMLCTL_SVSMLRRL(n)     ( (n)<<0 )

//#define SVSMIO_SVMLVLROE        ( 1<<12 )
//#define SVSMIO_SVMHOE           ( 1<<11 )
//#define SVSMIO_SVMOUTPOL        ( 1<<5 )
//#define SVSMIO_SVMLVLROE        ( 1<<4 )
//#define SVSMIO_SVMLOE           ( 1<<3 )

#endif /* __ROCKET_UCS_H__ */
