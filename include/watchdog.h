/*****************************************************************************
 * Filename:    watchdog.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 14, 2010
 *
 * Description: Watchdog module definitions
 ****************************************************************************/
#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include <registers.h>

#if defined( PROCESSOR_PMU430 )

#define WDTCTL_REG16      ( 0x015C )
#define WDTCTL_WDTPW      ( 0x5A<<8 )
#define WDTCTL_WDTHOLD    ( 1<<7 )
#define WDTCTL_WDTSSEL    ( 3<<5 )
#define WDTCTL_WDTTMSEL   ( 1<<4 )
#define WDTCTL_WDTCNTCL   ( 1<<3 )
#define WDTCTL_WDTIS      ( 7<<0 )


// write upper password byte and lower count reset bit
#define WDT_HIT() { \
    WORD reg = ReadRegWord( WDTCTL_REG16 ) & 0xFF;                          \
    WriteRegWord( WDTCTL_REG16, WDTCTL_WDTPW | WDTCTL_WDTCNTCL | reg );     \
}

// enable WDT timer with ACLK(32kHz) source with 16sec timeout
#define WDT_ENABLE() { \
    WriteRegWord( WDTCTL_REG16, WDTCTL_WDTPW | WDTCTL_WDTCNTCL |            \
                  (1<<5) | ( 0x3 ) );                                     \
}

#define WDT_DISABLE() { \
    WORD reg = ReadRegWord( WDTCTL_REG16 ) & 0xFF;                          \
    WriteRegWord( WDTCTL_REG16, WDTCTL_WDTPW | WDTCTL_WDTHOLD | reg );      \
}

#elif defined( PROCESSOR_MQ31 )

#include "maxq31_sfr.h"

// reset the watchdog timer
#define WDT_HIT() {							\
    WriteRegByteOr( WDCN_REG8, WDCN_RWT );	\
}

// enable WDT timer with 12MHz sysclk and 1.398sec timeout
#define WDT_ENABLE() {															\
    WriteRegByte( WDCN_REG8, WDCN_EWDI | WDCN_WD_2_24 | WDCN_EWT | WDCN_RWT );	\
}

#define WDT_DISABLE() { 											\
    WriteRegByte( WDCN_REG8, WDCN_EWDI | WDCN_WD_2_24 | WDCN_RWT );	\
}

#else

#error Watchdog module not defined for this processor

#endif


#define RESET_COOKIE_UNEXPECTED             0xABAD      // signals that we did not trigger a reset (probably a watchdog?)
#define RESET_COOKIE_INITIATED              0xFA5D      // initiated reset
#define RESET_COOKIE_INITIATED_LPM          0xC0DE      // initiated reset from low power mode
#define RESET_COOKIE_INITIATED_HOST         0xB00D      // host requested reset via I2C/SYS_EN
#define RESET_COOKIE_INITIATED_ERROR        0xE5A0      // msp430 triggered a reset due to a SW error
#define RESET_COOKIE_INITIATED_KEYVOL       0x1526      // reset triggered by the volume keys
#define RESET_COOKIE_INITIATED_UPDATER      0x39A0      // reset triggered by the updater sequence
#define RESET_COOKIE_INITIATED_PMICFAULT    0x1717      // reset triggered by a Pmic fault
#define RESET_COOKIE_INITIATED_DEEPSLEEP    0xDEE9      // entering deep sleep was requested by host
#define RESET_COOKIE_INITIATED_FASTRST      0xFA55      // fast reset was requested by host
#define RESET_COOKIE_INITIATED_WARMRST      0x9191      // WARM reset occured on host
#define RESET_COOKIE_INITIATED_BRICKWAKE    0x1919      // Reset triggered by user exiting brick/shelf mode

void WdtInit( void );
void WdtStartTimer( void );
void WdtHit( void );
void WdtPause( void );
void WdtResume( void );
void ConfigureRstNmi( void );
BYTE GetResetReason( void );
void UpdateResetStat( WORD cookie );
void RimInitiateReset( WORD reason );

#endif // _WATCHDOG_H_
