/*****************************************************************************
 * Filename:    lagavulin_updater.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 8, 2011
 *
 * Description: Lagavulin code update routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "updater.h"
#if defined( PROCESSOR_PMU430 )
#include "rocket_registers.h"
#else
#include "lagavulin_registers.h"
#endif
//#include "msp430_flash.h"
#include "lagavulin_metrics.h"
#include "watchdog.h"
#include "rtc.h"
#include "api.h"
#include "sfr.h"
#include "hostctrl.h"
#include "msgque.h"
#include "crc.h"
#include "flash.h"

#define VERBOSE_LEVEL 2
#include "eventlog.h"

#define UPDATER_LOCKED       0
#define UPDATER_KEY1_VALID   1
#define UPDATER_UNLOCKED     2

typedef struct {
    WORD command;
    BYTE payload[ MQ31_PAYLOAD_MAXSIZ ];
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
 * Temporarily disable JTAG access by clearing the TAP bit
 * of the system control (SC) register.
 */
#if defined( JTAG_FUSE_BLOW_ENABLED )
static void BlowJtagAccessFuse( void )
{
	// disable JTAG in system control register
	WriteRegWordAnd( SC_REG16, ~SC_TAP );
}
#endif

static BOOL IsJtagEnabled( void )
{
    // read system control register TAP bit
   if( ReadRegWord(SC_REG16) & SC_TAP ) {
	   // JTAG is enabled
	   return TRUE;
   }

   // if we got here, JTAG is disabled
   return FALSE;
}

void CodeUpdateStateInit( void )
{

    ApiRegisterHandlers( DEVID_UPDATER, UpdaterRegRead, UpdaterRegWrite, UpdaterStartReceived, UpdaterStopReceived );

    V1( EventLogAdd( EVENTLOG_UPDATER_INIT ) );
    // set flag if our APP section is the correct one that goes with the current BOOT section.
    AppCallsAllowed = IsImageValid( (ImageInfo *)LagavulinMetrics.AppImage,
                                    LagavulinMetrics.BootImage->pairCrc,
                                    LagavulinMetrics.BootImage->crc );

    // if the CRC's match up ok, lets make sure the versions are equal too
    if( AppCallsAllowed ) {
        if( (LagavulinMetrics.BootImage->versionMaj != LagavulinMetrics.AppImage->versionMaj) ||
        	(LagavulinMetrics.BootImage->versionMin != LagavulinMetrics.AppImage->versionMin) ) {
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

    // is the image size (in words) within our flash space?
    imageSize = imageLocation->endAddr - imageLocation->loadAddr + 1;
    if( imageSize > MQ31_UPDATE_SIZE ) {
        return( FALSE );
    }
    if( (imageLocation->loadAddr + imageSize) > MQ31_UPDATE_END ) {
        return( FALSE );
    }

    // CRC doesn't include the last 2 words of image (CRC and PairCRC)
    // Note: loadAddr should be in word address and imageSize should
    //		 be in the length in terms of words
    imageCRC = GenerateCrc16( (BYTE *)imageLocation->loadAddr, imageSize-4 );

    if( imageCRC != expectedCrc ) {
        return( FALSE );
    }

    return( TRUE );
}

static void WriteBootloaderMsg( WORD msg, WORD key )
{
    V1( EventLogAdd( EVENTLOG_UPDATER_WRITE_MSG ) );
    flash_writeWord( (WORD)&BootloaderMsg.msg, msg );
    flash_writeWord( (WORD)&BootloaderMsg.key, key );
}

static void ProcessUpdateCommand( WORD unused )
{
    WORD writeAddr;

    switch( Cmd.command ) {
        case UC_WRITE_BOOT:
			{
				WORD index;

				V2( EventLogAdd( EVENTLOG_UPDATER_WRITE_BOOT ) );
				writeAddr = MQ31_UPDATE_START + (UpdateBytesWritten >> 1);
				// make sure we arn't going to write past the end of flash
				// CmdIndex is the number of bytes received in this transaction to be written
				if( (writeAddr + (CmdIndex >> 1)) > MQ31_UPDATE_END ) {
					UpdateResponse = UR_TOO_LARGE;
					break;
				}
				for( index = 0; index < (CmdIndex >> 1); index++ ) {
					flash_writeWord( (WORD)(writeAddr + index), *(((WORD *)&Cmd.payload) + index) );
				}

				UpdateBytesWritten += CmdIndex;
			}
            break;
        case UC_PREP_BOOT:
			{
				V1( EventLogAdd( EVENTLOG_UPDATER_PREP_BOOT ) );
				UpdateBytesWritten = 0;

				// don't allow app calls anymore since we are wiping the APP section
				if( AppCallsAllowed ) {
					AppExitpoint();
					UtcDisable();
					AppCallsAllowed = FALSE;
				}

				UpdateSize = ((Cmd.payload[0]<<8) + Cmd.payload[1]) >> 1;
				if( UpdateSize > MQ31_UPDATE_SIZE ) {
					UpdateResponse = UR_TOO_LARGE;
					break;
				}

				// Erase enough space for the code update to be written to
				writeAddr = MQ31_UPDATE_START;
				while( writeAddr < MQ31_UPDATE_SIZE ) {
					EventLogAddN( EVENTLOG_UPDATER_ERASE_SEG, writeAddr );
					flash_erasePage( (WORD)(writeAddr >> 1) );
					writeAddr += MQ31_FLASH_PGSIZ;
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

				if( !IsImageValid( (ImageInfo*)(MQ31_UPDATE_START+(MQ31_BOOTAPP_START-((WORD)LagavulinMetrics.BootImage))), crcHost, pairCrcHost ) ) {
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

                if( crcBootHost != LagavulinMetrics.BootImage->crc ||
                    crcAppHost != LagavulinMetrics.BootImage->pairCrc ) {

                    UpdateResponse = UR_BOOT_REQ_UPDATE;

                } else if( !AppCallsAllowed ||
                           crcBootHost != LagavulinMetrics.AppImage->pairCrc ||
                           crcAppHost != LagavulinMetrics.AppImage->crc ) {

                    UpdateResponse = UR_APP_REQ_UPDATE;
                } else {
                    UpdateResponse = UR_SUCCESS;
                }
                break;
            }
        case UC_WRITE_APP:
			{
				WORD index;

				V2( EventLogAdd( EVENTLOG_UPDATER_WRITE_APP ) );
				writeAddr = MQ31_UPDATE_START + UpdateBytesWritten;
				// make sure we arn't going to write past the end of flash
				// CmdIndex is the number of bytes received in this transaction to be written
				if( (writeAddr + CmdIndex) > MQ31_UPDATE_END ) {
					UpdateResponse = UR_TOO_LARGE;
					break;
				}
				for( index = 0; index < CmdIndex; index += 2 ) {
					flash_writeWord( (WORD)((writeAddr + index) >> 1), *((WORD *)&(Cmd.payload[index])) );
				}

				UpdateBytesWritten += CmdIndex;
			}
            break;

        case UC_PREP_APP:
			{
				V1( EventLogAdd( EVENTLOG_UPDATER_PREP_APP ) );
				//UpdateBytesWritten = 0;

				// don't allow app calls anymore since we are wiping the APP section
				if( AppCallsAllowed ) {
					AppExitpoint();
					AppCallsAllowed = FALSE;
				}

				UpdateSize = ((Cmd.payload[0]<<8) + Cmd.payload[1]) >> 1;
				if( UpdateSize > (MQ31_UPDATE_START - ((WORD)LagavulinMetrics.AppImage)) ) {
					UpdateResponse = UR_TOO_LARGE;
					break;
				}

				// erase the APP image section so it can accept the update
				writeAddr = MQ31_UPDATE_START;
				while( writeAddr < MQ31_UPDATE_SIZE ) {
					EventLogAddN( EVENTLOG_UPDATER_ERASE_SEG, writeAddr );
					flash_erasePage( (WORD)writeAddr );
					writeAddr += MQ31_FLASH_PGSIZ;
				}

			}
			break;

        case UC_VERIFY_APP:
            {
                V1( EventLogAdd( EVENTLOG_UPDATER_VERIFY_APP ) );
                if( !IsImageValid( (ImageInfo *)LagavulinMetrics.AppImage, LagavulinMetrics.BootImage->pairCrc, LagavulinMetrics.BootImage->crc ) ) {
                    UpdateResponse = UR_BADCRC;
                } else {
                    AppCallsAllowed = TRUE;
                    // Run the AppInit code now that we have it up and running
                    LagavulinMetrics.AppImage->entrypoint();
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

