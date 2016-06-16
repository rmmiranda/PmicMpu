/*****************************************************************************
 * Filename:    rocket_updater.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     July 10, 2009
 *
 * Description: Rocket code update routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "updater.h"
#include "rocket_registers.h"
#include "msp430_flash.h"
#include "rocket_metrics.h"
#include "rocket_sfr.h"
#include "hostctrl.h"
#include "msgque.h"

#define VERBOSE_LEVEL 2
#include "eventlog.h"

#define UPDATER_LOCKED       0
#define UPDATER_KEY1_VALID   1
#define UPDATER_UNLOCKED     2

typedef struct {
    WORD command;
    BYTE payload[ PMU430_CMD_PAYLOAD_SIZE ];
} HostCommand;

BYTE CmdIndex;
HostCommand Cmd = { UC_NONE };

WORD UpdateSize;
WORD UpdateBytesWritten;

BYTE UpdatePermission = UPDATER_LOCKED;
UpdaterResponse UpdateResponse = UR_FAILURE;

BOOL UpdateCmdPending = FALSE;

extern UpdateFlashMsg BootloaderMsg;

BOOL AppCallsAllowed = FALSE;

/**
 * BlowJtagAccessFuse
 *
 * Permanently disable JTAG access by "blowing" the jtag fuse
 * (writing a non-zero and non-0xFFFFFFFF value to 0x17FC disables
 * JTAG access)
 *
 */
#if defined( JTAG_FUSE_BLOW_ENABLED )
static void BlowJtagAccessFuse( void )
{

    // allow BSL access
    WriteRegWordAnd( SYSBSLC_REG16, ~SYSBSLC_SYSBSLPE );

    // erase segment so we can write a pattern to the JTAG lock area of flash
    FlashEraseSegment( 0x17FC );

    FlashWrite( 0x17FC, 0xA5A5A5A5, FLASH_WRITE_DWORD );

    // lock BSL memory
    WriteRegWordOr( SYSBSLC_REG16, SYSBSLC_SYSBSLPE );
}
#endif

static BOOL IsJtagEnabled( void )
{
    BOOL ret = FALSE;
    DWORD reg;

    // allow BSL access
    WriteRegWordAnd( SYSBSLC_REG16, ~SYSBSLC_SYSBSLPE );


    reg = ReadRegDword( 0x17FC );

    if( reg == 0 || reg == 0xFFFFFFFF ) {
        ret = TRUE;
    }

    // lock BSL memory
    WriteRegWordOr( SYSBSLC_REG16, SYSBSLC_SYSBSLPE );

    return( ret );
}

void CodeUpdateStateInit( void )
{

    ApiRegisterHandlers( DEVID_UPDATER, UpdaterRegRead, UpdaterRegWrite, UpdaterStartReceived, UpdaterStopReceived );

    V1( EventLogAdd( EVENTLOG_UPDATER_INIT ) );
    // set flag if our APP section is the correct one that goes with the current BOOT section.
    AppCallsAllowed = IsImageValid( (ImageInfo *)RocketMetrics.AppImage,
                                    RocketMetrics.BootImage->pairCrc,
                                    RocketMetrics.BootImage->crc );

    // if the CRC's match up ok, lets make sure the versions are equal too
    if( AppCallsAllowed ) {
        if( RocketMetrics.BootImage->version != RocketMetrics.AppImage->version ) {
            AppCallsAllowed = FALSE;
        }
    }

}

// verifys the APP image at the expected location
BOOL IsImageValid( ImageInfo *imageLocation, WORD expectedCrc, WORD expectedPairCrc )
{
    WORD imageSize;
    WORD imageCRC;

    // Is cookie present
    if( imageLocation->cookie != IMAGE_INFO_COOKIE ) {
        return( FALSE );
    }

    if( imageLocation->crc != expectedCrc ) {
        return( FALSE );
    }

    if( imageLocation->pairCrc != expectedPairCrc ) {
        return( FALSE );
    }

    // is the image size within our flash space?
    imageSize = imageLocation->endAddr - imageLocation->loadAddr;
    if( imageSize > PMU430_MAX_BOOT_SIZE ) {
        return( FALSE );
    }
    if( (DWORD)(imageLocation->loadAddr + imageSize) > PMU430_LOADER_START ) {
        return( FALSE );
    }

    // CRC doesn't include the first 4 bytes of image (CRC and PairCRC)
    imageCRC = GenerateCrc16( (WORD)imageLocation+4 , imageSize-4 );

    if( imageCRC != expectedCrc ) {
        return( FALSE );
    }

    return( TRUE );
}

static void WriteBootloaderMsg( WORD msg, WORD key )
{
    V1( EventLogAdd( EVENTLOG_UPDATER_WRITE_MSG ) );
    FlashEraseSegment( (WORD)&BootloaderMsg );
    FlashWrite( (WORD)&BootloaderMsg.msg, (DWORD)msg, FLASH_WRITE_WORD );
    FlashWrite( (WORD)&BootloaderMsg.key, (DWORD)key, FLASH_WRITE_WORD );
}

static void ProcessUpdateCommand( WORD unused )
{
    WORD writeAddr;

    switch( Cmd.command ) {
        case UC_WRITE_BOOT:
            V2( EventLogAdd( EVENTLOG_UPDATER_WRITE_BOOT ) );
            writeAddr = PMU430_BOOT_UPDATE_START + UpdateBytesWritten;
            // make sure we arn't going to write past the end of flash
            // CmdIndex is the number of bytes received in this transaction to be written
            if( (DWORD)(writeAddr + CmdIndex) > PMU430_BOOTAPP_END ) {
                UpdateResponse = UR_TOO_LARGE;
                break;
            }
            FlashWriteBuffer( writeAddr, Cmd.payload, CmdIndex );

            UpdateBytesWritten += CmdIndex;

            break;
        case UC_WRITE_APP:
            V2( EventLogAdd( EVENTLOG_UPDATER_WRITE_APP ) );
            writeAddr = (WORD)RocketMetrics.AppImage + UpdateBytesWritten;
            // make sure we arn't going to write past the end of flash
            // CmdIndex is the number of bytes received in this transaction to be written
            if( (DWORD)(writeAddr + CmdIndex) > PMU430_BOOTAPP_END ) {
                UpdateResponse = UR_TOO_LARGE;
                break;
            }
            FlashWriteBuffer( writeAddr, Cmd.payload, CmdIndex );

            UpdateBytesWritten += CmdIndex;

            break;

        case UC_PREP_BOOT:
            {
                V1( EventLogAdd( EVENTLOG_UPDATER_PREP_BOOT ) );
                UpdateBytesWritten = 0;

                // don't allow app calls anymore since we are wiping the APP section
                if( AppCallsAllowed ) {
                    AppExitpoint();
                    // Exitpoint doesn't disable the UTC, so do it now
                    UtcDisable();
                    AppCallsAllowed = FALSE;
                }

                UpdateSize = (Cmd.payload[0]<<8) + Cmd.payload[1];
                if( UpdateSize > PMU430_MAX_BOOT_SIZE ) {
                    UpdateResponse = UR_TOO_LARGE;
                    break;
                }

                // Erase enough space for the code update to be written to
                writeAddr = PMU430_BOOT_UPDATE_START;
                while( writeAddr < UpdateSize+PMU430_BOOT_UPDATE_START ) {
                    EventLogAddN( EVENTLOG_UPDATER_ERASE_SEG, writeAddr );
                    FlashEraseSegment( writeAddr );
                    writeAddr += PMU430_FLASH_SEGMENT_SIZE;
                }
            }
            break;
        case UC_VERIFY_BOOT:
            {

                // verify image is valid as well as the 2 CRC's at the
                // start of the image
                WORD crcHost = (Cmd.payload[0]<<8) + Cmd.payload[1];
                WORD pairCrcHost = (Cmd.payload[2]<<8) + Cmd.payload[3];

                V1( EventLogAdd( EVENTLOG_UPDATER_VERIFY_BOOT ) );

                if( !IsImageValid( (ImageInfo*)PMU430_BOOT_UPDATE_START, crcHost, pairCrcHost ) ) {
                    UpdateResponse = UR_BADCRC;
                } else {
                    // now that we have a valid image ready to be loaded,
                    // flag it
                    WriteBootloaderMsg( FLASHMSG_APPLY_BOOT_UPDATE, crcHost );

                    // Trigger a full reset sequence that also resets the PMU430 core
                    // - Don't trust the host processor to do this for us (for now?)
                    #if !defined( PLATFORM_RIMEVALBOARD )
                    HostCtrlInitiateReset( RESET_COOKIE_INITIATED_UPDATER, RESET_PMU430_CORE | RESET_LCD_IO );
                    #endif
                }
            }
            break;
        case UC_CHECK_UPDATE:
            {
                WORD crcBootHost = (Cmd.payload[0]<<8) + Cmd.payload[1];
                WORD crcAppHost = (Cmd.payload[2]<<8) + Cmd.payload[3];

                V1( EventLogAdd( EVENTLOG_UPDATER_CHECK_UPDATE ) );

                if( crcBootHost != RocketMetrics.BootImage->crc ||
                    crcAppHost != RocketMetrics.BootImage->pairCrc ) {

                    UpdateResponse = UR_BOOT_REQ_UPDATE;

                } else if( !AppCallsAllowed ||
                           crcBootHost != RocketMetrics.AppImage->pairCrc ||
                           crcAppHost != RocketMetrics.AppImage->crc ) {

                    UpdateResponse = UR_APP_REQ_UPDATE;
                } else {
                    UpdateResponse = UR_SUCCESS;
                }
                break;
            }
        case UC_PREP_APP:
            {
                V1( EventLogAdd( EVENTLOG_UPDATER_PREP_APP ) );
                UpdateBytesWritten = 0;

                // don't allow app calls anymore since we are wiping the APP section
                if( AppCallsAllowed ) {
                    AppExitpoint();
                    AppCallsAllowed = FALSE;
                }

                UpdateSize = (Cmd.payload[0]<<8) + Cmd.payload[1];
                if( UpdateSize > (PMU430_BOOTAPP_END - (WORD)RocketMetrics.AppImage) ) {
                    UpdateResponse = UR_TOO_LARGE;
                    break;
                }

                // erase the APP image section so it can accept the update
                writeAddr = (WORD)RocketMetrics.AppImage;
                while( writeAddr < UpdateSize+(WORD)RocketMetrics.AppImage ) {
                        EventLogAddN( EVENTLOG_UPDATER_ERASE_SEG, writeAddr );
                        FlashEraseSegment( writeAddr );
                        writeAddr += PMU430_FLASH_SEGMENT_SIZE;
                }

            }
            break;

        case UC_VERIFY_APP:
            {
                V1( EventLogAdd( EVENTLOG_UPDATER_VERIFY_APP ) );
                if( !IsImageValid( (ImageInfo *)RocketMetrics.AppImage, RocketMetrics.BootImage->pairCrc, RocketMetrics.BootImage->crc ) ) {
                    UpdateResponse = UR_BADCRC;
                } else {
                    AppCallsAllowed = TRUE;
                    EventLogAdd( EVENTLOG_RTC_INIT );
                    UtcInit();
                    // Run the AppInit code now that we have it up and running
                    RocketMetrics.AppImage->entrypoint();
                }
            }
            break;
        case UC_BLOW_JTAG_FUSE:
            {
                if( Cmd.payload[0] == 0xDE && Cmd.payload[1] == 0xAD ) {
#if defined( JTAG_FUSE_BLOW_ENABLED )
                    V1( EventLogAdd( EVENTLOG_UPDATER_BLOW_JTAG_FUSE ) );
                    // kill the fuse :(
                    BlowJtagAccessFuse();
#else
                    // if the build doesn't allow this, be a bit more descriptive then FAILURE
                    UpdateResponse = UR_UNKNOWN_COMMAND;
#endif
                } else {
                    UpdateResponse = UR_FAILURE;
                }
            }
            break;
        case UC_IS_JTAG_ENABLED:
            {
                if( !IsJtagEnabled() ) {
                    // FAILURE in this case, means JTAG isn't available
                    UpdateResponse = UR_FAILURE;
                }
            }
            break;
        default:
            UpdateResponse = UR_UNKNOWN_COMMAND;
            break;
    }

    if( UpdateResponse == UR_BUSY ) {
        UpdateResponse = UR_SUCCESS;
    }
}

BYTE UpdaterRegRead( BYTE reg )
{
    BYTE ret;

    switch( reg ) {
        case UPDATER_REG_RESPONSE:
            ret = UpdateResponse;
            break;
        case UPDATER_REG_STATUS:
            ret = AppCallsAllowed;
            break;
        default:
            ret = 0;
    }

    return( ret );
}

void UpdaterStartReceived( void )
{

    // if we are already processing a command, or one is pending, don't accept another
    if( UpdateResponse == UR_BUSY ) {
        return;
    }

    CmdIndex = 0;
    Cmd.command = UC_NONE;
}

void UpdaterStopReceived( void )
{

    if( (UpdatePermission == UPDATER_UNLOCKED) && Cmd.command != UC_NONE ) {
        // if we are already processing a command, or one is pending, don't accept another
        if( UpdateResponse == UR_BUSY ) {
            return;
        }
        UpdateResponse = UR_BUSY;
        QueueCall( ProcessUpdateCommand, 0 );
    }
}



void UpdaterRegWrite( BYTE reg, BYTE data )
{

    // don't allow any access to write to the update registers unless the
    // host unlocks them first
    if( UpdatePermission == UPDATER_UNLOCKED ) {
        // if we are already processing a command, or one is pending, don't accept another
        if( UpdateResponse == UR_BUSY ) {
            return;
        }
        switch( reg ) {
            case UPDATER_REG_COMMAND:
                // fill command payload
                if( Cmd.command != UC_NONE ) {
                    Cmd.payload[ CmdIndex++ ] = data;
                // 1st byte is command
                } else {
                    Cmd.command = data;
                }
                break;
            case UPDATER_REG_KEY1:
            case UPDATER_REG_KEY2:
            default:
                UpdatePermission = UPDATER_LOCKED;
                break;
        }
    } else if( UpdatePermission == UPDATER_LOCKED ) {
        if( reg == UPDATER_REG_KEY1  && data == UPDATER_VALID_KEY1 ) {
            UpdatePermission = UPDATER_KEY1_VALID;
        }
    } else if( UpdatePermission == UPDATER_KEY1_VALID ) {
        if( reg == UPDATER_REG_KEY2  && data == UPDATER_VALID_KEY2 ) {
            UpdatePermission = UPDATER_UNLOCKED;

            // unlocking the module also resets all pointers/memory for a fresh update

        } else {
            UpdatePermission = UPDATER_LOCKED;
        }
    } else {
        UpdatePermission = UPDATER_LOCKED;
    }
}

