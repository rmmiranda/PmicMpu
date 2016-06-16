/*****************************************************************************
 * Filename:    init.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     November 21, 2008
 *
 * Description: Rocket init routines
 ****************************************************************************/

#include "critical.h"
#include "registers.h"
#include "watchdog.h"
#include "gpio.h"
#include "timer_mux.h"
#include "i2c.h"
#include "slowclock.h"
#include "msgque.h"
#include "spi.h"
#include "api.h"
#include "pmic_intctrl.h"
#include "intvector.h"
#include "delay.h"
#if defined( PROCESSOR_PMU430 )
#include "rocket_metrics.h"
#include "msp430_rtc.h"
#include "rocket_led_hw.h"
#include "rocket_pmic.h"
#include "rocket_ucs.h"
#include "rocket_adc_hw.h"
#else
#include "lagavulin_adc_hw.h"
#include "lagavulin_metrics.h"
#include "lagavulin_pmic.h"
#endif
#include "updater.h"

#include "led.h"
#include "hostctrl.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

static void ChargerHandler( void )
{
	if( PmicRead( PMIC_CHG_INT_OK ) & CHGINT_OK_BAT ) {
//		PadConfig( PAD_PORT_0_3, PAD_OUTPUT_HIGH );
//	    PmicSet( PMIC_L05_CNFG1, 0xC0 );
	    PmicSet( PMIC_VOUTCONFIG1B5, BUCK_PWR_MD_NORMAL );
//	    PmicWrite( PMIC_FLASHRGBON, 0x00 );
//	    PmicWrite( PMIC_RLEDSET, 0x00 );
//	    PmicWrite( PMIC_GLEDSET, 0x80 );
//	    PmicWrite( PMIC_BLEDSET, 0x00 );
//	    PmicWrite( PMIC_FLASHRGBON, 0xFF );
	} else {
//		PadConfig( PAD_PORT_0_3, PAD_OUTPUT_LOW );
//	    PmicClear( PMIC_L05_CNFG1, 0xC0 );
	    PmicClear( PMIC_VOUTCONFIG1B5, BUCK_PWR_MD_NORMAL );
//	    PmicWrite( PMIC_FLASHRGBON, 0x00 );
//	    PmicWrite( PMIC_RLEDSET, 0x80 );
//	    PmicWrite( PMIC_GLEDSET, 0x00 );
//	    PmicWrite( PMIC_BLEDSET, 0x00 );
//	    PmicWrite( PMIC_FLASHRGBON, 0xFF );
	}

	return;
}

#if defined( PROCESSOR_PMU430 )
// For now we're using the compiler libraries to do memory and stack
// initialization.  Before the memory init, system_pre_init() is
// called.  After the memory init, main() is called.
int _system_pre_init( void )
{
    // the compiler libraries don't clear .bss
    extern BYTE __bss__, __end__;
    memset( &__bss__, 0, (&__end__ - &__bss__) );
    return 1;
}
#endif // PROCESSOR_PMU430

void SwitchToXtal( WORD unused )
{
    QueueCall( ConfigureXt1External, 0 );
    return;
}

int main( void )
{
    WdtInit();

    InterruptControllerInit();

    EventLogInit();

    GpioInit();

    // Configure !RSN/NMI pin for NMI mode early
    ConfigureRstNmi();

    ConfigureFll();

    V1( EventLogAdd( EVENTLOG_TIMER_INIT ) );
    TimerInit();

    WdtStartTimer();

    V1( EventLogAdd( EVENTLOG_SPI_INIT ) );
    SpiInit();

#if !defined( PLATFORM_RIMEVALBOARD )
    HostCtrlPmicRegisterReset( RESET_LCD_IO );

#if defined( PROCESSOR_PMU430 )
    //resetting the fuel gauge LDO
    PmicClear( PMIC_VRTC, 1<<7 );
    HwDelay(25000000); //25 ms delay
    PmicSet( PMIC_VRTC, 1<<7 );
#endif

#endif

    ApiInit();

#if defined( DEVID_UPDATER_SUPPORT )
    CodeUpdateStateInit( );
#endif

    V1( EventLogAdd( EVENTLOG_ADC_HW_INIT ) );
    AdcInitHw();

    V1( EventLogAdd( EVENTLOG_MSGQUE_INIT ) );
    CallQueueInit();

    V1( EventLogAdd( EVENTLOG_PMIC_INTCTRL_INIT ) );
    PmicIntCtrlInit();

    // configure charger interrupt handler
    PmicRegisterIntHandler( INT_CHARGER_IRQ, ChargerHandler );
    PmicInterruptEnable( INT_CHARGER_IRQ );

    // enable battery insertion/removal interrupts
	PmicWrite( PMIC_CHG_INT, 0x00 );
    PmicClear( PMIC_CHG_INT_MASK, CHGINT_MASK_BAT );
    PmicClear( PMIC_MIRQ, INT_MASK_CHARGER );


    I2cInitInternalBus();

    // Need to enable GIE (PoR default is OFF)
    __enable_interrupt();

    RegisterTimerExpiry( RTC_EXTERNAL_XTAL_TIMER, SwitchToXtal, 0 );
    // switch to external xtal after 2 seconds
    SetTimer( RTC_EXTERNAL_XTAL_TIMER, 0x10000 );

#if defined( PLATFORM_RIMEVALBOARD )
    I2cInit();
#else
    HostCtrlInit();
#endif

    for (;;) {
        WdtHit();

        if( !ExecuteCall() ) {
            // if there was nothing to call, enter sleep mode
            SCSleep();
        }
    }
}

