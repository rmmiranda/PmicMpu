/*****************************************************************************
 * Filename:    rocket_ft.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     March 2011
 *
 * Description: Rocket Fault tolerance handling
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "timer_mux.h"
#include "rocket_pmic.h"
#include "config_pwrseq.h"
#include "spi.h"
#include "eventlog.h"
#include "hostctrl.h"
#include "rocket_ft.h"
#include "rocket_ucs.h"
#include "rocket_adc_hw.h"
#include "msgque.h"
#include "rocket_registers.h"
#include "api.h"
#include "pmic_intctrl.h"

#define SUPERCAP_VCORE_LEVEL   PMMCTL0_PMMCOREV_LVL0

#define FT_STATE_DISABLED (0)
#define FT_STATE_ENABLED  (1)
#define FT_STATE_TRIPPED  (2)

static BYTE ftWatchdogState;
extern BOOL AllowDeepSleep;

static void DummyFunc( void )
{
    return;
}

__inline static void SetBrickBit( void )
{
    // disable FLSHMODE
    PmicWrite( PMIC_FLASHCTRL0, 0x00 );
    // LDO_MODE must be cleared
    PmicWrite( PMIC_CTRL_PPCHG2, 0x00 );
    // Set the BRICK bit, clear the PPSEL bits
    PmicWrite( PMIC_CTRL_PPCHG0, 1<<3 );
}

/******************************************************************************
 * FtWatchdogHandler()
 *
 * Handles the tripped watcdhog
 *****************************************************************************/
static void FtWatchdogHandler( void )
{
    BYTE level;
    BYTE i;
    static BOOL usbWasPresent = FALSE;

    WDT_HIT();
    if( ftWatchdogState == FT_STATE_TRIPPED ) {
#if defined( END_KEY ) && !defined( WORKAROUND_END_KEY_PGFAULT )
        // if the END_KEY is pressed, reset us
        if( !PadRead( END_KEY ) ) {
            PmicWrite( PMIC_CTRL_PPCHG0, 0x00 );
            RimInitiateReset( RESET_COOKIE_INITIATED_BRICKWAKE );
        } else
#endif
        if( (PmicRead( PMIC_STAT_PPCHG1 ) & PMIC_STAT_PPCHG1_USB_PRESENT) ) {
            if( !usbWasPresent ) {
                PmicWrite( PMIC_CTRL_PPCHG0, 0x00 );
                RimInitiateReset( RESET_COOKIE_INITIATED_BRICKWAKE );
            }
        } else {
            usbWasPresent = FALSE;
        }

    } else {
        ftWatchdogState = FT_STATE_TRIPPED;

        AdcHalt();

        // disconnect the backup supercap from the system; rocket is running off
        // the battery
        PadConfig( BACKUP_PWR_EN, PAD_OUTPUT_LOW );

        // make sure all supplies are turned off
        PmicWrite( PMIC_EN_CMD0, 0 );
        PmicWrite( PMIC_EN_CMD1, 0 );
        PmicWrite( PMIC_EN_CMD2, 0 );

        // rest most Power die registers to their defaults
        HostCtrlPmicRegisterReset( RESET_LCD_IO | RESET_HOST_WARM );

        // Make sure FG LDO is off, its turned on in HostCtrlPmicRegisterReset above
        PmicClear( PMIC_VRTC, VRTC_FGLDO_EN );

        // clear/disable the pg fault interrupts
        PmicInterruptDisable( INT_LOWSYS );
        for( i=INT_PWRGD_FLT_LDO0 ; i<=INT_PWRGD_FLT_SM3 ; i++ ) {
            // except LDO5 (USBLDO). Don't trip a PG fault with it
            if( i != INT_PWRGD_FLT_LDO5 ) {
                PmicInterruptDisable( (PmicInterrupt)i );
            }
        }

        usbWasPresent = PmicRead( PMIC_STAT_PPCHG1 ) & PMIC_STAT_PPCHG1_USB_PRESENT;

        // kill the battery fet and disconnect all power from the rest of the system
        SetBrickBit();

        WriteRegWord( UCSCTL0_REG16, 0x0100 );      // set lowest possible DCOx MODx values

        // lower core speed to 1MHz
        WriteRegWord( UCSCTL1_REG16, 0x20 );       // Set operation range
        WriteRegWord( UCSCTL2_REG16, 0x101F );     // Set DCO multiplier to 1MHz (1 + N ) * FLLRef = DCO
                                               // (1 + 30) * 32768 = 1MHz
        level = VCoreLevel();
        // Ramp down the core voltage one step at a time and disable full performance mode
        do {
            SetVCoreLevel( level--, FALSE );
        } while( VCoreLevel() > SUPERCAP_VCORE_LEVEL );

        // disable REF system to reduce current consumption
        WriteRegWord( REFCTL0_REG16, REFCTL0_REFTCOFF | REFCTL0_REFMSTR );

        // reset all IOs to their low power state
        HostCtrlPowerLost();

        // reconfigure the SPI gpio's so we can still talk to the power die
        PadConfig( PMIC_SPI_CS, PAD_OUTPUT_LOW );
        PadConfig( PMIC_SPI_SIMO, PAD_ALT_MODE );
        PadConfig( PMIC_SPI_SOMI, PAD_ALT_MODE );
        PadConfig( PMIC_SPI_CLK, PAD_ALT_MODE );

#if defined( END_KEY ) && !defined( WORKAROUND_END_KEY_PGFAULT )
        // if the End key is defined, make sure its reconfigured as an input and
        // ready to be used to poll for wakeup (also disables any PULL on this IO)
        PadConfig( END_KEY, PAD_INPUT_ENABLE );
#endif

        EventLogAdd( EVENTLOG_ENTER_BRICK_LPM );
        AllowDeepSleep = TRUE;  // allow us to go into LPM3
        // post a msg to make sure the SCSleep() function is run so LPM3 is entered
        QueueCall( DummyFunc, 0 );

    }
    SetTimer( BRICK_TIMER, FT_END_KEY_POLL );
}

/******************************************************************************
 * FtWatchdogEnable( BOOL )
 *
 * Starts/stops the FT watchdog feature
 *****************************************************************************/
static void FtWatchdogEnable( BOOL enable )
{
    if( enable ) {
        ftWatchdogState = FT_STATE_ENABLED;
    } else {
        StopTimer( BRICK_TIMER );
        ftWatchdogState = FT_STATE_DISABLED;
    }
}

/******************************************************************************
 * FtWatchdogKick()
 *
 * Called by the i2c interface to kick the FT Watchdog from tripping
 *****************************************************************************/
void FtWatchdogKick( void )
{
    if( ftWatchdogState == FT_STATE_ENABLED ) {
        SetTimer( BRICK_TIMER, FT_WATCHDOG_TIMEOUT );
    }
}

/******************************************************************************
 * FtRegRead()
 *
 * I2C Interface to FT feature (read)
 *****************************************************************************/
static BYTE FtRegRead( BYTE reg )
{
    switch( reg ) {
        case FT_REG_STATUS:
            return( (ftWatchdogState == FT_STATE_ENABLED) );

        default:
            return 0;
    }
}

/******************************************************************************
 * FtRegWrite()
 *
 * I2C Interface to FT feature (write)
 *****************************************************************************/
static void FtRegWrite( BYTE reg, BYTE value )
{
    switch( reg ) {
        case FT_REG_CONFIG:
            if( value & FT_KICK ) {
                FtWatchdogKick();
            }
            if( value & FT_BRICK ) {
                FtWatchdogHandler();
            }
            break;

        case FT_REG_PASSWORD:
            if( value == FT_PASSWORD ) {
                // disable the FT watchdog
                FtWatchdogEnable( FALSE );
            } else {
                // start up the watchdog if it was disabled previously
                if( ftWatchdogState == FT_STATE_DISABLED ) {
                    FtWatchdogEnable( TRUE );
                }
            }
            break;

        default:
            break;
    }
}

/******************************************************************************
 * FtWatchdogInit()
 *
 * Starts and enables the FT watchdog feature
 *****************************************************************************/
void FtWatchdogInit( void )
{
    ApiRegisterHandlers( DEVID_FT, FtRegRead, FtRegWrite, NULL, NULL );
    RegisterTimerExpiry( BRICK_TIMER, FtWatchdogHandler, 0 );
    ftWatchdogState = FT_STATE_DISABLED;
#if defined( RIM_FT_WATCHDOG )
    FtWatchdogEnable( TRUE );
#endif
}

/******************************************************************************
 * FtWatchdogUninit()
 *
 * Stops and disables the FT watchdog feature
 *****************************************************************************/
void FtWatchdogUninit( void )
{
    FtWatchdogEnable( FALSE );
    ApiUnregisterHandlers( DEVID_FT );
}
