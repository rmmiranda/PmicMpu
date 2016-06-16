/*****************************************************************************
 * Filename:    rocket_api.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     December 2, 2008
 *
 * Description: Implementation of an API between Rocket and a host processor
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "api.h"
#include "ApiVersion.h"
#include "rocket_metrics.h"
#include "gpio.h"
#include "pmic_intctrl.h"
#include "spi.h"
#include "updater.h"
#include "msp430_rtc.h"
#include "watchdog.h"
#include "msgque.h"
#include "rocket_registers.h"
#include "hostctrl.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

#if defined( RIM_KEYVOL_RESET )
BOOL IsKeyVolumeResetEnabled( void );
void KeyVolumeResetEnable( BOOL enable );
#endif

void RocketRegWrite( BYTE reg, BYTE value );
BYTE RocketRegRead( BYTE reg );
void RocketRegStopReceived( void );

void RocketPmicWrite( BYTE reg, BYTE value );
BYTE RocketPmicRead( BYTE reg );
void RocketPmicStopReceived( void );

void PmicUpdateIntMaskRegisters( void ) ;

typedef struct {
    BYTE intmask;
    BYTE intflag;
    BYTE evtlog0;
} RocketRegisters;
RocketRegisters rReg;

static BOOL newTransaction;

extern BYTE ResetStat;
extern BYTE ResetExtra;

static struct {
    ApiRegReadFunc regRead;
    ApiRegWriteFunc regWrite;
    ApiRegFunc startRxd;
    ApiRegFunc stopRxd;
} ApiHandler[ DEVID_NUM_DEVICES ];

BYTE SysFlags0;

void ApiInit( void )
{
    BYTE i;

    for( i=0; i<DEVID_NUM_DEVICES; i++ ) {
        ApiHandler[ i ].regRead = NULL;
        ApiHandler[ i ].regWrite = NULL;
        ApiHandler[ i ].startRxd = NULL;
        ApiHandler[ i ].stopRxd = NULL;
    }
#if !defined( PLATFORM_RIMEVALBOARD )
    ApiRegisterHandlers( DEVID_ROCKET, RocketRegRead, RocketRegWrite, NULL, RocketRegStopReceived );
#endif // PLATFORM_RIMEVALBOARD

    // PMIC read/write passthrough
    ApiRegisterHandlers( DEVID_PMIC, RocketPmicRead, RocketPmicWrite, NULL, RocketPmicStopReceived );

    rReg.intmask = 0xFF;
    rReg.intflag = 0x00;
    SysFlags0 = 0;
}

void ApiUnregisterHandlers( BYTE devId )
{
    ApiHandler[ devId ].regRead = NULL;
    ApiHandler[ devId ].regWrite = NULL;
    ApiHandler[ devId ].startRxd = NULL;
    ApiHandler[ devId ].stopRxd = NULL;
}

void ApiRegisterHandlers( BYTE devId, ApiRegReadFunc funcRead,
                                      ApiRegWriteFunc funcWrite,
                                      ApiRegFunc funcStartRxd,
                                      ApiRegFunc funcStopRxd )
{
    if( devId >= DEVID_NUM_DEVICES ) {
        return;
    }

    ApiHandler[ devId ].regRead = funcRead;
    ApiHandler[ devId ].regWrite = funcWrite;
    ApiHandler[ devId ].startRxd = funcStartRxd;
    ApiHandler[ devId ].stopRxd = funcStopRxd;
}

static void UpdateInterruptHost()
{
#if defined( PMIC_INT_N )
    if( rReg.intflag & ~rReg.intmask ) {
        // Assert interrupt to host
        PadConfig( PMIC_INT_N, PAD_OUTPUT_LOW );
    } else {
        // Clear host interrupt
        PadConfig( PMIC_INT_N, PAD_OUTPUT_HIGH );
    }
#endif
}

void ApiInterruptHost( BYTE intId )
{
    WORD wason = CoreIntDisable();

    V1( EventLogAddN( EVENTLOG_API_HOSTINT, intId ) );

    // set interrupt flag for host to read
    rReg.intflag |= intId;
    UpdateInterruptHost();

    CoreIntEnable( wason );
}

void ApiClearInterruptHost( BYTE intId )
{
    WORD wason = CoreIntDisable();

    rReg.intflag &= ~intId;
    UpdateInterruptHost();

    CoreIntEnable( wason );
}

void ApiStopReceived( BYTE devId )
{
    if( devId >= DEVID_NUM_DEVICES ) {
        return;
    }


    if( ApiHandler[ devId ].stopRxd != NULL ) {
        ApiHandler[ devId ].stopRxd();
    }

    newTransaction = FALSE;
}

void ApiStartReceived( BYTE devId )
{
    newTransaction = TRUE;

    if( devId >= DEVID_NUM_DEVICES ) {
        return;
    }

    if( ApiHandler[ devId ].startRxd != NULL ) {
        ApiHandler[ devId ].startRxd();
    }
}

BOOL ApiAutoIncrementOverried( BYTE devId, BYTE reg )
{
    BOOL override = FALSE;

    // always override the autoincrement on a fresh transaction
    if( newTransaction ) {
        return( TRUE );
    }

    switch( devId ) {
        case DEVID_ROCKET:
            if( reg == ROCKET_REG_EVTLOG0_LOG ) {
                override = TRUE;
            }else if( reg == ROCKET_REG_EVTLOG0 ) {
                override = TRUE;
            }
            break;
        case DEVID_UPDATER:
            if( reg == UPDATER_REG_COMMAND ) {
                override = TRUE;
            }
            break;
        default:
            break;
    }

    return( override );
}

BYTE ApiRegRead( BYTE devId, BYTE reg )
{
    BYTE ret = 0;

    if( devId >= DEVID_NUM_DEVICES ) {
        return( ret );
    }


    if( ApiHandler[ devId ].regRead != NULL ) {
        ret = ApiHandler[ devId ].regRead( reg );
    }

    newTransaction = FALSE;

    return( ret );
}

void ApiRegWrite( BYTE devId, BYTE reg, BYTE value )
{
    if( devId >= DEVID_NUM_DEVICES ) {
        return;
    }

    if( ApiHandler[ devId ].regWrite != NULL ) {
        ApiHandler[ devId ].regWrite( reg, value );
    }

    newTransaction = FALSE;
}


// General/Control Rocket Registers
#if !defined( PLATFORM_RIMEVALBOARD )
BYTE RocketRegRead( BYTE reg )
{
    static BYTE LatchedUtcRead[8];

    switch( reg ) {
        case ROCKET_REG_EVTLOG0_LOG:
            return( SavedEventLogRead() );

        case ROCKET_REG_EVTLOG0:
            return( SavedEventLogExists()<<2 );

        case ROCKET_REG_VERSION1:
            return( RocketMetrics.RocketVersion >> 8 );

        case ROCKET_REG_VERSION2:
            return( RocketMetrics.RocketVersion );

        case ROCKET_REG_INTMASK0:
            return( rReg.intmask );

        case ROCKET_REG_INTFLAG0:
            return( rReg.intflag );

        case ROCKET_REG_UTCCTRL:
            if( AppCallsAllowed ) {
                return( UtcIsDisabled() );
            } else {
                return( 1 );
            }

        case ROCKET_REG_UTCCOUNT0:
            if( AppCallsAllowed ) {
                // grab all 64 bits of time
                UtcGetTime( LatchedUtcRead );
            }
            // fallthrough
        case ROCKET_REG_UTCCOUNT1:
        case ROCKET_REG_UTCCOUNT2:
        case ROCKET_REG_UTCCOUNT3:
        case ROCKET_REG_UTCCOUNT4:
        case ROCKET_REG_UTCCOUNT5:
        case ROCKET_REG_UTCCOUNT6:
        case ROCKET_REG_UTCCOUNT7:
            if( AppCallsAllowed ) {
                return LatchedUtcRead[reg-ROCKET_REG_UTCCOUNT0];
            } else {
                return( 0 );
            }

        case ROCKET_REG_RESETCTRL:
#if defined( RIM_KEYVOL_RESET )
            if( AppCallsAllowed ) {
                return( IsKeyVolumeResetEnabled()<<2 );
            } else {
                return( 0 );
            }
#else
            return( 0 );
#endif

        case ROCKET_REG_RESETSTAT:
            return ResetStat;

        case ROCKET_REG_RESETEXTRA:
            return ResetExtra;

        case ROCKET_REG_LOADER_VERSION1:
            return( ((RocketMetricsStruct*)PMU430_LOADER_START)->RocketVersion>>8 );

        case ROCKET_REG_LOADER_VERSION2:
            return( ((RocketMetricsStruct*)PMU430_LOADER_START)->RocketVersion );

        case ROCKET_REG_HWV_HARDWAREID0:
        case ROCKET_REG_HWV_HARDWAREID1:
        case ROCKET_REG_HWV_HARDWAREID2:
        case ROCKET_REG_HWV_HARDWAREID3:
            return( ((RocketMetricsStruct*)PMU430_LOADER_START)->HwvHardwareId>>((reg-ROCKET_REG_HWV_HARDWAREID0)*8) );

        case ROCKET_REG_HWV_CLASS_PWRSEQ:
            return( ((RocketMetricsStruct*)PMU430_LOADER_START)->HwvPwrSeq );

        case ROCKET_REG_BUILDINFO1:
            return( RocketMetrics.BuildInfo >> 8 );

        case ROCKET_REG_BUILDINFO2:
            return( RocketMetrics.BuildInfo );

        case ROCKET_REG_LOADER_BUILDINFO1:
            return( ((RocketMetricsStruct*)PMU430_LOADER_START)->BuildInfo >> 8 );

        case ROCKET_REG_LOADER_BUILDINFO2:
            return( ((RocketMetricsStruct*)PMU430_LOADER_START)->BuildInfo );

        case ROCKET_REG_API_VERSION_MAJOR:
            return( RocketMetrics.BootImage->version >> 8 );

        case ROCKET_REG_API_VERSION_MINOR:
            return( RocketMetrics.BootImage->version );

        case ROCKET_REG_SYSFLAGS0:
            return( SysFlags0 );

        default:
            return 0;
    }
}

void RocketRegWrite( BYTE reg, BYTE value )
{
    static BYTE LatchedUtcWrite[8];

    switch( reg ) {
        case ROCKET_REG_EVTLOG0:
            rReg.evtlog0 = value;
            break;

        case ROCKET_REG_UTCCTRL:
            if( AppCallsAllowed & (value & ROCKET_UTC_UTCDIS) ) {
                UtcDisable();
            }
            break;

        case ROCKET_REG_UTCCOUNT0:
        case ROCKET_REG_UTCCOUNT1:
        case ROCKET_REG_UTCCOUNT2:
        case ROCKET_REG_UTCCOUNT3:
        case ROCKET_REG_UTCCOUNT4:
        case ROCKET_REG_UTCCOUNT5:
        case ROCKET_REG_UTCCOUNT6:
            LatchedUtcWrite[ reg - ROCKET_REG_UTCCOUNT0 ] = value;
            break;

        case ROCKET_REG_UTCCOUNT7:
            LatchedUtcWrite[7] = value;
            if( AppCallsAllowed ) {
                // store all 64 bits of time
                UtcSetTime( LatchedUtcWrite );
            }
            break;

        case ROCKET_REG_INTMASK0:
            rReg.intmask = value;
            UpdateInterruptHost();
            break;

        case ROCKET_REG_RESETCTRL:
            if( value & ROCKET_RESETCTRL_WATCHDOG ) {
                WdtHit();
                CoreIntDisable();
                while( 1 ) {
                    __no_operation();
                }
            } else if( value & ROCKET_RESETCTRL_DORESET ) {
                RimInitiateReset( RESET_COOKIE_INITIATED_HOST );
#if !defined( PLATFORM_RIMEVALBOARD )
            } else if( value & ROCKET_RESETCTRL_DEEPSLEEP ) {
                HostCtrlInitiateReset( RESET_COOKIE_INITIATED_DEEPSLEEP, RESET_WAIT_EXTPWR | RESET_LCD_IO | RESET_DDR_SUPPLY );
            } else if( value & ROCKET_RESETCTRL_WARMRST ) {
                // if we are supposed to keep the LCD alive, don't reset it too
                if( SysFlags0 & ROCKET_SYSFLAGS0_LCD_RET_EN ) {
                    HostCtrlInitiateReset( RESET_COOKIE_INITIATED_WARMRST, RESET_HOST_WARM );
                } else {
                    HostCtrlInitiateReset( RESET_COOKIE_INITIATED_WARMRST, RESET_HOST_WARM | RESET_LCD_IO );
                }
#endif // PLATFORM_RIMEVALBOARD
            }

            // only enable this if we have an app section (must be enabled
            // after we are powered up since OR gate is powered by V1_8DIG
#if defined( RIM_KEYVOL_RESET )
            if( AppCallsAllowed ) {
                KeyVolumeResetEnable( (value & ROCKET_RESETCTRL_KEYVOL_EN) == ROCKET_RESETCTRL_KEYVOL_EN );
            }
#endif
            break;
        case ROCKET_REG_SYSFLAGS0:
            SysFlags0 = value;
            break;

        default:
            break;
    }
}

void RocketRegStopReceived( void )
{
    if( rReg.evtlog0 & ROCKET_EVTLOG0_CAPTURE ) {
        rReg.evtlog0 &= ~(ROCKET_EVTLOG0_CAPTURE|ROCKET_EVTLOG0_REWIND);
        //QueueCall( EventLogCapture, 0 );
        // A workaround just to be able to dump the event log when the queue is blocked
        EventLogCapture();
    }else if( rReg.evtlog0 & ROCKET_EVTLOG0_REWIND ) {
        rReg.evtlog0 &= ~ROCKET_EVTLOG0_REWIND;
        //QueueCall( SavedEventLogRewind, 0);
        // A workaround just to be able to dump the event log when the queue is blocked
        SavedEventLogRewind();
    }
}
#endif // PLATFORM_RIMEVALBOARD

// register access for I2C from host passed through to PMIC via SPI

BYTE RocketPmicRead( BYTE reg )
{

    // TODO: once the interrupt control shadow register system has been tested,
    // possibly move it into here rather then having a different DEVID_PMIC_INTCTRL ?
    // for now, don't guard access to those registers so we can read them
    // while evaluating the part.
    if( reg >= PMIC_REG_STAT_PPCHG0 && reg <= PMIC_REG_INTMASK5 ) {
        return PmicIntctrlRegRead( reg );
    } else {
        return( PmicRead( reg ) );
    }
}

void RocketPmicWrite( BYTE reg, BYTE value )
{
    // TODO: once the interrupt control shadow register system has been tested,
    // possibly move it into here rather then having a different DEVID_PMIC_INTCTRL ?
    if( reg >= PMIC_REG_STAT_PPCHG0 && reg <= PMIC_REG_INTMASK5 ) {
        PmicIntctrlRegWrite( reg, value );
    } else {
        PmicWrite( reg, value );
    }
}

void RocketPmicStopReceived( void )
{
    if( IntctrlUpdateStat & INTCTRL_UPDATE_REQ ) {
        IntctrlUpdateStat &= ~INTCTRL_UPDATE_REQ;
        if( !(IntctrlUpdateStat & INTCTRL_UPDATE_QUEUED) ) {
            IntctrlUpdateStat |= INTCTRL_UPDATE_QUEUED;
            QueueCall( PmicUpdateIntMaskRegisters, 0 );
        }
    }
}

