/*****************************************************************************
 * Filename:    rocket_slowclock.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:
 *
 * Description: Rocket sleep and reset functions
 ****************************************************************************/
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "rocket_sfr.h"
#include "rocket_ucs.h"
#include "timer.h"
#include "msgque.h"
#if defined( DEVID_UPDATER_SUPPORT )
#include "updater.h"
#endif
#include "rocket_metrics.h"
#include "rocket_adc_hw.h"
#if defined( RIM_PWM )
#include "rocket_pwm.h"
#endif

#define VERBOSE_LEVEL 1
#include "eventlog.h"

#define OPERATING_VCORE_LEVEL  PMMCTL0_PMMCOREV_LVL2

#define SUPERCAP_VCORE_LEVEL   PMMCTL0_PMMCOREV_LVL0

volatile BOOL AllowDeepSleep = FALSE;

/******************************************************************************
 * VCoreLevel( )
 *
 * Get current VCore setting
 *****************************************************************************/
WORD VCoreLevel( void )
{
    return( ReadRegWord( PMMCTL0_REG16 ) & PMMCTL0_PMMCOREV_MASK );
}

/******************************************************************************
 * SetVCoreLevel( level, fullPerformance )
 *
 * Set VCore product point and enable/disable full performance mode
 * normal mode = 150uS latency on wakeup
 * full performance mode = 8-10 uS latency on wakeup
 *****************************************************************************/
void SetVCoreLevel( BYTE level, BOOL fullPerformance )
{
    WORD wason = CoreIntDisable();

    // Write PMM Key to unlock PMM Module and allow us to configure it
    WriteRegByte( PMMCTL0_H_REG8, PMMCTL0_H_PMMKEY );


    // set SVS/SVM high side to new level
    WriteRegWord( SVSMHCTL_REG16, SVSMHCTL_SVSHE | SVSMHCTL_SVMHE | (SVSMHCTL_SVSHRVL(level)) | (SVSMHCTL_SVSMHRRL(level)) );

    // set SVS/SVM low side to level 0 for BACKUP?? (TI sample code does this. not sure why yet)
    WriteRegWord( SVSMLCTL_REG16, SVSMLCTL_SVMLE | SVSMLCTL_SVSLE | (SVSMLCTL_SVSLRVL(0)) | (SVSMLCTL_SVSMLRRL(0)) );

    if( fullPerformance ) {
        WriteRegWordOr( SVSMHCTL_REG16, SVSMHCTL_SVMHFP | SVSMHCTL_SVSHFP );
        WriteRegWordOr( SVSMLCTL_REG16, SVSMLCTL_SVMLFP | SVSMLCTL_SVSLFP );
    }

    // Wait for SVSMLDLYIFG to be set indicating SVS and SVM low side have settled
    while( ReadRegWord( PMMIFG_REG16 ) & PMMIFG_SVSMLDLYIFG == 0 ) {};

    // Clear flags which we are about to test after VCore change
    WriteRegWordAnd( PMMIFG_REG16, ~(PMMIFG_SVMLVLRIFG | PMMIFG_SVMLIFG) );

    // Program new PMMCOREV level
    WriteRegByte( PMMCTL0_L_REG8, level );

    // If VCore is under the new voltage (it should be!!)
    if( ReadRegWord( PMMIFG_REG16 ) & PMMIFG_SVMLIFG ) {
        // wait for new voltage to be reached
        while( (ReadRegWord( PMMIFG_REG16 ) & PMMIFG_SVMLVLRIFG) == 0) {};
    }

    if( fullPerformance ) {
        WriteRegWordOr( PMMCTL1_REG16, PMMCTL1_PMMCMD_FULL );
    } else {
        WriteRegWordAnd( PMMCTL1_REG16, ~PMMCTL1_PMMCMD_MASK );
    }

    // Clear PMM Key now that we are done modifying the module
    WriteRegByte( PMMCTL0_H_REG8, 0x00 );

    CoreIntEnable( wason );
}

/******************************************************************************
 * ConfigureFll()
 *
 * Configure PMU430 core to proper VCore and Operating frequency; also
 * configure peripheral clock speed and 32k source
 *****************************************************************************/
void ConfigureFll( void )
{

    BYTE level = VCoreLevel();
    // Ramp up the core voltage one step at a time and enable full performance mode
    do {
        // set VCore to level and enable full performance mode
        SetVCoreLevel( level++, TRUE );
    } while( VCoreLevel() < OPERATING_VCORE_LEVEL );

    WriteRegWord( UCSCTL0_REG16, 0x1000 );      // set Middle possible DCOx and min MODx values

    WriteRegWord( UCSCTL1_REG16, 0x60 );        // Set operation range to 16MHz (see data sheet)
#if defined( PROCESSOR_PMU430 )
    WriteRegWord( UCSCTL2_REG16, 487 );         // Set DCO multiplier to 16MHz (1 + N ) * FLLRef = DCO
                                                // (1 + 487) * 32768 = 16MHz
#else
#warn Verify Core frequency is running at 12Mhz and possibly reduce core voltage above
    WriteRegWord( UCSCTL2_REG16, 365 );         // Set DCO multiplier to 12MHz (1 + N ) * FLLRef = DCO
                                                // (1 + 365) * 32768 = 12MHz
#endif


    WriteRegWord( UCSCTL3_REG16, UCSCTL3_SELREF_REFOCLK );        // FLL reference set to REFOCLK(32kHz)

    // Set SMCLKdiv to 4   4Mhz  (3Mhz)
    WriteRegWord( UCSCTL5_REG16, 2<<4 );

    // Configure clock sources ACLK=32Khz, MCLK=16Mhz (12Mhz), SMCLK = 4Mhz (3Mhz)
    WriteRegWord( UCSCTL4_REG16, UCSCTL4_SELA_REFOCLK | UCSCTL4_SELM_DCOCLK | UCSCTL4_SELS_DCOCLKDIV );

    // let DCO lock/settle
    do {
        WriteRegWordAnd( UCSCTL7_REG16, ~UCSCTL7_DCOFFG );
        WriteRegWordAnd( SFRIFG1_REG16, ~SFRIFG1_OFIFG );
    } while( ReadRegWord( UCSCTL7_REG16 ) & UCSCTL7_DCOFFG );

#if defined( PROCESSOR_PMU430 )
    // XT2 Off, XT1 On, Low drive strength (for 4-8Mhz),
    WriteRegWord( UCSCTL6_REG16, UCSCTL6_XT2OFF | UCSCTL6_XT1DRIVE_MIN |
                  UCSCTL6_XT1BYPASS );


#else
 #if defined( HAS_EXTERNAL_32KHZ_XTAL )
    // XT2 Off, XT1 On, Min drive strength, 2pF internal caps SMCLK on
    WriteRegWord( UCSCTL6_REG16, UCSCTL6_XT2OFF | UCSCTL6_XT1DRIVE_MIN | UCSCTL6_XT1CAP_6PF );

    // Configure XIN and XOUT to their Alt mode for external XTAL
    PadConfig( XIN_32K, PAD_ALT_MODE );
    PadConfig( XOUT_32K, PAD_ALT_MODE );
 #endif
#endif
}

/******************************************************************************
 * ConfigureXt1External()
 *
 * Switch clock sources to use external XT1 (32.768 kHz) XTAL; takes approx
 * 315mS to become stable and switch over
 *****************************************************************************/
void ConfigureXt1External( WORD unused )
{
    EventLogAdd( EVENTLOG_EXT_XTAL );
    // make sure DCO is locked/settled
    do {
        WriteRegWordAnd( UCSCTL7_REG16, ~UCSCTL7_XT1LFOFFG );
        WriteRegWordAnd( SFRIFG1_REG16, ~SFRIFG1_OFIFG );
    } while( ReadRegWord( UCSCTL7_REG16 ) & UCSCTL7_XT1LFOFFG );


    // Switch FLL over to use XT1, since we were previously using REFO @ 32.768 KHz no further
    // FLL changes are needed to maintain MCLK and SMCLK frequencies.
    WriteRegWord( UCSCTL3_REG16, UCSCTL3_SELREF_XT1CLK );

    // switching to XT1 takes approx 8000 cycles (or 244ms)
    // let DCO lock/settle
    do {
        WriteRegWordAnd( UCSCTL7_REG16, ~UCSCTL7_DCOFFG );
        WriteRegWordAnd( SFRIFG1_REG16, ~SFRIFG1_OFIFG );
    } while( ReadRegWord( UCSCTL7_REG16 ) & UCSCTL7_DCOFFG );

    // Switch ACLK over to use XT1
    WriteRegWordAnd( UCSCTL4_REG16, ~UCSCTL4_SELA_MASK );

    // clear and enable the OFIFG
    WriteRegWordAnd( SFRIFG1_REG16, ~(SFRIFG1_OFIFG | SFRIFG1_VMAIFG) );
    // Enable Oscillator Fault interrupt
    WriteRegWordOr( SFRIE1_REG16, SFRIE1_OFIE | SFRIE1_VMAIE );
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
    // Disable the REQEN HW mechanism which will hold clocks on if a peripheral is using it
    // except for ACLK(32kHz) ... we still need that one
    //WriteRegWordAnd( UCSCTL8_REG16, ~(UCSCTL8_SMCLK_REQEN | UCSCTL8_MCLK_REQEN /*| UCSCTL8_ACLK_REQEN*/) );

    // Disable the SMCLK manually since entering LPM doens't alter it?
    //WriteRegWordOr( UCSCTL6_REG16, UCSCTL6_SMCLKOFF );

    WriteRegWord( UCSCTL0_REG16, 0x0100 );      // set lowest possible DCOx MODx values

    // lower core speed to 1MHz
    WriteRegWord( UCSCTL1_REG16, 0x20 );       // Set operation range
    WriteRegWord( UCSCTL2_REG16, 0x101F );     // Set DCO multiplier to 1MHz (1 + N ) * FLLRef = DCO
                                               // (1 + 30) * 32768 = 1MHz
    // let DCO lock/settle
    do {
        WriteRegWordAnd( UCSCTL7_REG16, ~UCSCTL7_DCOFFG );
        WriteRegWordAnd( SFRIFG1_REG16, ~SFRIFG1_OFIFG );
    } while( ReadRegWord( UCSCTL7_REG16 ) & UCSCTL7_DCOFFG );

    level = VCoreLevel();
    // Ramp down the core voltage one step at a time and disable full performance mode
    do {
        SetVCoreLevel( level--, FALSE );
    } while( VCoreLevel() > SUPERCAP_VCORE_LEVEL );

    // stop Timer
    TimerHWHalt();

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

}

/******************************************************************************
 * SCSleep()
 *
 * Put core in IDLE mode while we wait for an interrupt to occur.
 *****************************************************************************/
void SCSleep( void )
{
    WORD wason = CoreIntDisable();

    // can only be called from our idle loop with interrupts enabled otherwise
    // our interrupt state will get screwed up.
    if( wason == 0 ) {
        return;
    }

    // if there are any pending messages, don't sleep yet
    if( GetQueueCount() ) {
        CoreIntEnable( wason );
        return;
    }

    // Entering LPM now
    V2( EventLogAddN( EVENTLOG_ENTER_SCSLEEP, AllowDeepSleep ) );

    // Enter LPM3: CPU/MCLK=OFF, FLL=OFF, ACLK=ON, Vcore=ON
    // Enable interrupts so we can wakeup.
//    _bis_SR_register( LPM3_SR_BITS | STATUS_REG_GIE );

    if( AllowDeepSleep ) {
        _bis_SR_register( LPM3_SR_BITS | STATUS_REG_GIE );
        _no_operation();
    } else {
    #if defined( RIM_PWM )
        if( PwmIsActive() ) {
            _bis_SR_register( LPM0_SR_BITS | STATUS_REG_GIE );
        } else {
            _bis_SR_register( LPM1_SR_BITS | STATUS_REG_GIE );
        }
    #else
        _bis_SR_register( LPM3_SR_BITS | STATUS_REG_GIE );
    #endif
        _no_operation();
    }

    // Exiting LPM now (with interrupts automatically re-enabled)
    V2( EventLogAdd( EVENTLOG_EXIT_SCSLEEP ) );

//    CoreIntEnable( wason );
}

