/*****************************************************************************
 * Filename:    lagavulin_slowclock.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:		March 1, 2011
 *
 * Description: Lagavulin sleep and reset functions
 ****************************************************************************/
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "timer.h"
#include "msgque.h"
#if defined( DEVID_UPDATER_SUPPORT )
#include "updater.h"
#endif
#include "lagavulin_metrics.h"
#include "lagavulin_adc_hw.h"
#include "slowclock.h"
#if defined( RIM_PWM )
//#include "rocket_pwm.h"
#endif

#define VERBOSE_LEVEL 1
#include "eventlog.h"

#define OPERATING_VCORE_LEVEL		5
#define MIN_OPERATING_VCORE_LEVEL	1
#define SUPERCAP_VCORE_LEVEL		0

static volatile WORD scsleepDisabled = 0;

void AppExitpoint( void );

/******************************************************************************
 * VCoreLevel( )
 *
 * Get current VCore setting
 *****************************************************************************/
WORD VCoreLevel( void )
{
	BYTE reg = ReadRegByte( CKCN_REG8 );

	if( reg & CKCN_STOP ) {
        // This would never happen, but just keep it for code readability.
		return (WORD)(SUPERCAP_VCORE_LEVEL);
	} else if( reg & CKCN_PMME ) {
		return (WORD)(MIN_OPERATING_VCORE_LEVEL);
	} else {
		return (WORD)(OPERATING_VCORE_LEVEL - (reg & CKCN_CD_MASK));
	}
}

/******************************************************************************
 * SetVCoreLevel( level, fullPerformance )
 *
 * Set VCore product point
 *****************************************************************************/
void SetVCoreLevel( BYTE level, BOOL fullPerformance )
{
    WORD wason = CoreIntDisable();

    // program new vcore level
	switch( level ) {
		case SUPERCAP_VCORE_LEVEL:
            // It's unsafe to enter STOP mode here. Do nothing and let the
            // idle loop to hit the STOP mode.
			break;

		case MIN_OPERATING_VCORE_LEVEL:
            // The STOP bit should have been cleared automatically if
            // the code is being executed.
            //WriteRegByteAnd( CKCN_REG8, ~CKCN_STOP );
			WriteRegByteOr( CKCN_REG8, CKCN_PMME );
			break;

		default:
			WriteRegByteAnd( CKCN_REG8, ~(CKCN_CD_MASK | CKCN_PMME | CKCN_STOP) );
			WriteRegByteOr( CKCN_REG8, (OPERATING_VCORE_LEVEL - level) );
			break;
	}

    CoreIntEnable( wason );
}

/******************************************************************************
 * ConfigureFll()
 *
 * Configure MQ31 to proper VCore and Operating frequency; Also
 * configure Timer B0 as initial source of 32K clock.
 *****************************************************************************/
void ConfigureFll( void )
{
    BYTE level;

    // Ramp up the core voltage one step at a time
    for( level = VCoreLevel();
         level < OPERATING_VCORE_LEVEL;
         level = VCoreLevel() ) {
        SetVCoreLevel( level + 1, TRUE );
    }

    // Configure Timer B0 as an inaccurate 32KHz source
    // 1) PWM toggle mode, 12MHz input clock, no interrupts
    // 2) Set reload value to 0xB7 and compare value to 0x5B
    // 3) Enable timer
	WriteRegWord( TBCN0_REG16, TBCN_TBCR | TBCN_TBCS );
	WriteRegWord( TBR0_REG16, 0xB7 );
	WriteRegWord( TBC0_REG16, 0x5B );
	WriteRegWordOr( TBCN0_REG16, TBCN_TRB );
}

/******************************************************************************
 * ConfigureXt1External()
 *
 * Switch clock sources to use external XT1 (32.768 kHz) from PR66; Then
 * disable Timer B0 to stop the inaccurate 32K clock.
 *****************************************************************************/
void ConfigureXt1External( WORD data )
{
    EventLogAdd( EVENTLOG_EXT_XTAL );

    // switch rtc source from inaccurate 32K clock (Timer B0)
	// to the inaccurate 32K clock (from analog chip)
    WriteRegWordOr( RTCCFG_REG16, RTCCFG_OUTCLK );

    // wait until accurate 32K is actually driving rtc
    while( (ReadRegWord(RTCCFG_REG16) & RTCCFG_OUTCLKMD) == 0 )
    	; // do nothing

    // then disable the inaccurate 32K clock (Timer B0)
	WriteRegWord( TBCN0_REG16, 0x0000 );
	WriteRegWord( TBR0_REG16, 0x0000 );
	WriteRegWord( TBC0_REG16, 0x0000 );
}

/******************************************************************************
 * PreEnterDeepSleep()
 *
 * Prepare to enter highest power saving mode possible while maintaining the
 * RTC module. All other modules are either disabled, stopped, or their clock
 * is disabled to minimize wakeups.
 *
 * PreEnterDeepSleep should be used when we detect we are running off
 * the SuperCap
 *****************************************************************************/
void PreEnterDeepSleep( void )
{
    BYTE level;

    // stop any ADC activity
    AdcHalt();

#if defined( DEVID_UPDATER_SUPPORT )
    // if Apps exist its safe to call their supercap shutdown routine
    if( AppCallsAllowed ) {
        AppExitpoint();
    }
#else
    AppExitpoint();
#endif

    // Ramp down the core voltage one step at a time and disable full performance mode
    for( level = VCoreLevel();
         level > MIN_OPERATING_VCORE_LEVEL;
         level = VCoreLevel() ) {
        SetVCoreLevel( level - 1, FALSE );
    }
}

/******************************************************************************
 * SCSleep()
 *
 * Put core in IDLE mode while we wait for an interrupt to occur.
 *****************************************************************************/
void SCSleep( void )
{
    WORD wason = CoreIntDisable();

    /* If interrupt has been disabled, leave the function. This function
     * can only be called from our idle loop with interrupts enabled, otherwise
     * our interrupt state will get screwed up.
     */
    if( ( wason & IC_IGE ) == 0 ) {
        return;
    }

    V2( EventLogAddN( EVENTLOG_DEBUG_DATA, scsleepDisabled ) );
    // If sleep has been forbidden, re-enable interrupt and leave.
    if( scsleepDisabled ) {
        CoreIntEnable( wason );
        return;
    }

    // If there are more pending messages, re-enable interrupt and leave.
    if( GetQueueCount() ) {
        CoreIntEnable( wason );
        return;
    }

    V2( EventLogAdd( EVENTLOG_ENTER_SCSLEEP ) );
    
    /* Entry STOP mode.
     * Using the sequence Maxim recommended. In this case the code is still
     * protected by the system's interrupt latency.
     */
    __enable_interrupt();
    asm( "move ckcn.4, #1" );
    asm( "nop" );

    /* At here global interrupt is already enabled. But since we already made
     * sure interrupt was enabled before entering this function, we are fine.
     */

    V2( EventLogAdd( EVENTLOG_EXIT_SCSLEEP ) );
}

void SCSleepDisable(int module_)
{
    WORD wason = CoreIntDisable();

    scsleepDisabled |= ( 1 << (module_ & 0xf ) );

    CoreIntEnable( wason );
}

void SCSleepEnable(int module_)
{
    WORD wason = CoreIntDisable();
    
    scsleepDisabled &= ~( 1 << (module_ & 0xf ) );
    
    CoreIntEnable( wason );
}


