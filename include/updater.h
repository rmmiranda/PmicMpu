/*****************************************************************************
 * Filename:    updater.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 8, 2011
 *
 * Description: Definitions for update code
 ****************************************************************************/
#ifndef __UPDATER_H__
#define __UPDATER_H__

#if defined( PROCESSOR_PMU430 )

// largest command payload which can be received
#define PMU430_CMD_PAYLOAD_SIZE     64

#define PMU430_FLASH_SEGMENT_SIZE   0x200

#elif defined( PROCESSOR_MQ31 )

// The size of a flash page in bytes
#define MQ31_FLASH_PGSIZ    0x200

// Maximum payload size in bytes
#define MQ31_PAYLOAD_MAXSIZ 64

// Define the locations for the various flash regions.  All addresses and sizes
// should be specified in word values

#define MQ31_BOOTMGR_START		  ( 0x1F00 )
#define MQ31_BOOTMGR_END		  ( 0x1FFF )

#define MQ31_BOOTAPP_START        ( 0x2000 )
#define MQ31_BOOTAPP_END          ( 0x8FFF )
#define MQ31_BOOTAPP_SIZE         ( MQ31_BOOTAPP_END - MQ31_BOOTAPP_START + 1 )

#define MQ31_UPDATE_START         ( 0x9000 )
#define MQ31_UPDATE_END           ( 0xFFFF )
#define MQ31_UPDATE_SIZE          ( MQ31_UPDATE_END - MQ31_UPDATE_START + 1 )

#define MQ31_MAX_BOOT_SIZE        ( MQ31_BOOTAPP_SIZE + MQ31_UPDATE_SIZE )

#else

#error Updater module not defined for this processor

#endif


typedef enum {
    UC_WRITE_BOOT,      // Writes a chunk of the boot update to flash
                        // [ Cmd(1) ][ BootCodeChunk(0-CMD_PAYLOAD_SIZE) ]

    UC_WRITE_APP,       // Writes a chunk of the app update to flash
                        // [ Cmd(1) ][ BootCodeChunk(0-CMD_PAYLOAD_SIZE) ]

    UC_PREP_BOOT,       // Tell PMU430 to prepare for an update for the Boot code with size of command payload
                        // [ Cmd(1) ][ BootUpdateSize(2) ]

    UC_VERIFY_BOOT,     // Request PMU430 to verify Boot image crc's with the command payload crc's
                        // [ Cmd(1) ][ CRC(2) CRCPAIR(2) ]

    UC_CHECK_UPDATE,    // Requests PMU430 to verify the BOOT/APP code running with the crc's within the payload
                        // Returns UR_SUCCESS if CRC's match otherwise UR_BOOT_REQ_UPDATE or UR_APP_REQ_UPDATE if
                        // either the BOOT or APP section requires an update
                        // [ Cmd(1) ][ CRC(2) CRCPAIR(2) ]

    UC_PREP_APP,        // Tell PMU430 to prepare for an update for the App code
                        // [ Cmd(1) ][ AppUpdateSize(2) ]

    UC_VERIFY_APP,      // Request PMU430 to verify App image CRC with the BOOT metrics CRCs
                        // [ Cmd(1) ]

    UC_BLOW_JTAG_FUSE,  // Tell PMU430 to blow the JTAG fuse to permanently disable JTAG access
                        // requires proper "key" or it is ignored
                        // [ Cmd(1) ][ 0xDEAD ]

    UC_IS_JTAG_ENABLED, // Request the JTAG fuse state, returns SUCCESS if JTAG is still enabled
                        // [ Cmd(1) ]

    UC_NONE
} UpdaterCommand;

typedef enum {
    UR_SUCCESS,
    UR_FAILURE,
    UR_BAD_PAYLOAD,
    UR_TOO_LARGE,
    UR_BADCRC,
    UR_BUSY,
    UR_BOOT_REQ_UPDATE,
    UR_APP_REQ_UPDATE,
    UR_UNKNOWN_COMMAND
} UpdaterResponse;

void CodeUpdateStateInit( void );

#endif // __UPDATER_H__
