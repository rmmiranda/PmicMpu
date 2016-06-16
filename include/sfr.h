/************************************************************************************
 * Filename:    sfr.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 7, 2011
 *
 * Description: Definitions of special function registers (renamed from rocket_sfr.h
 * 				which was authored by Bryon Hummel and created on December 3, 2008).
 ***********************************************************************************/
#ifndef __SFR_H__
#define __SFR_H__

#if defined( PROCESSOR_PMU430 )

#define SFR_BASE            ( 0x100 )

#define SFRIE1_REG16        ( SFR_BASE + 0x00 )
#define SFRIFG1_REG16       ( SFR_BASE + 0x02 )
#define SFRRPCR_REG16       ( SFR_BASE + 0x04 )

#define SFRIE1_JMBOUTIE     ( 1<<7 )
#define SFRIE1_JMBINIE      ( 1<<6 )
#define SFRIE1_ACCVIE       ( 1<<5 )
#define SFRIE1_NMIIE        ( 1<<4 )
#define SFRIE1_VMAIE        ( 1<<3 )
#define SFRIE1_OFIE         ( 1<<1 )
#define SFRIE1_WDTIE        ( 1<<0 )


#define SFRIFG1_JMBOUTIFG   ( 1<<7 )
#define SFRIFG1_JMBINIFG    ( 1<<6 )
#define SFRIFG1_NMIIFG      ( 1<<4 )
#define SFRIFG1_VMAIFG      ( 1<<3 )
#define SFRIFG1_OFIFG       ( 1<<1 )
#define SFRIFG1_WDTIFG      ( 1<<0 )


#define SFRRPCR_SYSRSTRE    ( 1<<3 )
#define SFRRPCR_SYSRSTUP    ( 1<<2 )
#define SFRRPCR_SYSNMIIES   ( 1<<1 )
#define SFRRPCR_SYSNMI      ( 1<<0 )

#define SYS_BASE            ( 0x180 )

#define SYSCTL_REG16        ( SYS_BASE + 0x00 )
#define SYSBSLC_REG16       ( SYS_BASE + 0x02 )
#define SYSARB_REG16        ( SYS_BASE + 0x04 )
#define SYSJMBC_REG16       ( SYS_BASE + 0x06 )
#define SYSJMBI0_REG16      ( SYS_BASE + 0x08 )
#define SYSJMBI1_REG16      ( SYS_BASE + 0x0A )
#define SYSJMBO0_REG16      ( SYS_BASE + 0x0C )
#define SYSJMBO1_REG16      ( SYS_BASE + 0x0E )

#define SYSBERRIV_REG16     ( SYS_BASE + 0x18 )
#define SYSUNIV_REG16       ( SYS_BASE + 0x1A )
#define SYSSNIV_REG16       ( SYS_BASE + 0x1C )
#define SYSRSTIV_REG16      ( SYS_BASE + 0x1E )

#define SYSCTL_SYSJTAGPIN   ( 1<<5 )
#define SYSCTL_SYSBSLIND    ( 1<<4 )
#define SYSCTL_SYSPMMPE     ( 1<<2 )
#define SYSCTL_SYSRIVECT    ( 1<<0 )

#define SYSBSLC_SYSBSLPE    ( 1u<<15)
#define SYSBSLC_SYSBSLOFF   ( 1<<14)
#define SYSBSLC_SYSBSLR     ( 1<<2)
#define SYSBSLC_SYSBSLSIZE  ( 3<<0)

#define SYSRSTIV_BOR        ( 0x02 )
#define SYSRSTIV_RST        ( 0x04 )
#define SYSRSTIV_DOBOR      ( 0x06 )
#define SYSRSTIV_PORTWU     ( 0x08 )
#define SYSRSTIV_SECV       ( 0x0A )
#define SYSRSTIV_SVSL       ( 0x0C )
#define SYSRSTIV_SVSH       ( 0x0E )
#define SYSRSTIV_SVML_OVP   ( 0x10 )
#define SYSRSTIV_SVMH_OVP   ( 0x12 )
#define SYSRSTIV_DOPOR      ( 0x14 )
#define SYSRSTIV_WDT        ( 0x16 )
#define SYSRSTIV_WDT_KEYV   ( 0x18 )
#define SYSRSTIV_FLASH_KEYV ( 0x1A )
#define SYSRSTIV_PLL_UNLOCK ( 0x1C )
#define SYSRSTIV_PERF       ( 0x1E )
#define SYSRSTIV_PMM_KEYV   ( 0x20 )



// Status Register bits
#define STATUS_REG_C        ( 0x0001 )
#define STATUS_REG_Z        ( 0x0002 )
#define STATUS_REG_N        ( 0x0004 )
#define STATUS_REG_V        ( 0x0100 )
#define STATUS_REG_GIE      ( 0x0008 )
#define STATUS_REG_CPUOFF   ( 0x0010 )
#define STATUS_REG_OSCOFF   ( 0x0020 )
#define STATUS_REG_SCG0     ( 0x0040 )
#define STATUS_REG_SCG1     ( 0x0080 )

#define LPM0_SR_BITS        ( STATUS_REG_CPUOFF )
#define LPM1_SR_BITS        ( STATUS_REG_SCG0 | STATUS_REG_CPUOFF)
#define LPM2_SR_BITS        ( STATUS_REG_SCG1 | STATUS_REG_CPUOFF)
#define LPM3_SR_BITS        ( STATUS_REG_SCG1 | STATUS_REG_SCG0 | STATUS_REG_CPUOFF )
#define LPM4_SR_BITS        ( STATUS_REG_SCG1 | STATUS_REG_SCG0 | STATUS_REG_OSCOFF | STATUS_REG_CPUOFF)

#elif defined( PROCESSOR_MQ31 )

#include "maxq31_sfr.h"

#else

#error Unknown SFR register definitions

#endif

#endif

