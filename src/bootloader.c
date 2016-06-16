/*****************************************************************************
 * Filename:    bootloader.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     July 30, 2009
 *
 * Description: Rocket bootloader init routines
 ****************************************************************************/

#include "critical.h"
#include "registers.h"
#include "watchdog.h"
#include "rocket_metrics.h"
#include "rocket_ucs.h"
#include "msp430_flash.h"

// Embed BOOT/APP image for this build directly within the bootloader image (so we actually have something
// to jump to on Day 1 when the SW is loaded). This file is generated by the build process and requires
// the output of the standard app/boot image build
#pragma DATA_SECTION( rocket_image, ".boot_sec" );
#include "rocket_image.h"

// Msg in flash from BOOT code to trigger a code update in bootloader
extern const UpdateFlashMsg BootloaderMsg;

int _system_pre_init( void )
{
    return 1;
}

// looks at the image info header and, if it exists, verifies image
// located there. If its valid returns TRUE and the ImageInfo
static BOOL isImageValid( ImageInfo *imgLocation, ImageInfo *image )
{
    WORD imageSize;
    WORD imageCRC;

    // Is cookie present
    if( imgLocation->cookie != IMAGE_INFO_COOKIE ) {
        return( FALSE );
    }

    // we are only shuffling around BOOT images
    if( imgLocation->type != IMAGE_TYPE_BOOT ) {
        return( FALSE );
    }

    // is the image size within our flash space?
    imageSize = imgLocation->endAddr - imgLocation->loadAddr;
    if( imageSize > PMU430_MAX_BOOT_SIZE ) {
        return( FALSE );
    }
    if( (DWORD)(imgLocation->loadAddr + imageSize) > PMU430_LOADER_START ) {
        return( FALSE );
    }

    // CRC doesn't include the first 4 bytes of image (CRC and PairCRC)
    imageCRC = GenerateCrc16( (WORD)imgLocation+4 , imageSize-4 );

    if( imageCRC != imgLocation->crc ) {
        return( FALSE );
    }

    memcpy( image, imgLocation, sizeof(ImageInfo) );

    return( TRUE );
}

static void EraseImage( WORD start, WORD size )
{
    WORD seg = start;

    while( seg <= (start+size) ) {
        FlashEraseSegment( seg );
        seg += 0x200;
    }
}

static BOOL WriteImage( WORD src, ImageInfo *img )
{
    WORD size;
    ImageInfo i;

    size = img->endAddr - img->loadAddr;

    // erase the segments which will be written to
    EraseImage( img->loadAddr, size );

    FlashWriteBuffer( img->loadAddr, (BYTE*)src, size );

    // ensure pairCrc was copied correctly too since it isnt' verified in the
    // isImageValid call below
    if( ((ImageInfo*)img->loadAddr)->pairCrc != img->pairCrc ) {
        return( FALSE );
    }

    return( isImageValid( (ImageInfo*)img->loadAddr, &i ) );
}

static void WriteBootloaderMsg( WORD msg, WORD key )
{
    FlashEraseSegment( (WORD)&BootloaderMsg );
    FlashWrite( (WORD)&BootloaderMsg.msg, (DWORD)msg, FLASH_WRITE_WORD );
    FlashWrite( (WORD)&BootloaderMsg.key, (DWORD)key, FLASH_WRITE_WORD );
}

void main( void )
{
    ImageInfo imageOne;
    ImageInfo imageTwo;
    BOOL readyToCopy = FALSE;

    WDT_DISABLE();


    // if we arn't supposed to be updating anything, just jump to the
    // BOOT code
    if( BootloaderMsg.msg == FLASHMSG_APPLY_BOOT_UPDATE ) {

        imageTwo.type = IMAGE_TYPE_EMPTY;


        // Is BOOT image valid?
        if( !isImageValid( (ImageInfo*)PMU430_BOOTAPP_START, &imageOne ) ) {
            // if NOT, we must have gotten interrupted in the middle of a previous update,
            // need to try to apply the update again.
            readyToCopy = TRUE;

            memcpy( &imageTwo, (ImageInfo *)PMU430_BOOT_UPDATE_START, sizeof(ImageInfo) );

        } else {

            // make sure there is an updated BOOT image waiting
            if( isImageValid( (ImageInfo*)PMU430_BOOT_UPDATE_START, &imageTwo ) ) {
                readyToCopy = TRUE;
            }
        
            // somehow we triggered an update but the update image isn't
            // the one we expect?
            if( imageTwo.crc != BootloaderMsg.key ) {
                readyToCopy = FALSE;
                WriteBootloaderMsg( 0, 0 );
            }
        }

        if( readyToCopy && imageTwo.type == IMAGE_TYPE_BOOT ) {
            //  copy update image from imageTwo to BOOT
            while( !WriteImage( PMU430_BOOT_UPDATE_START, &imageTwo ) ) {
                // keep writing the image until it works
            }

            // update has been applied, clear this flag in flash
            WriteBootloaderMsg( 0, 0 );
        }
    }
    
    WDT_ENABLE();
    WDT_HIT();

    // We have completed all our updates and checks, there *should* be a BOOT
    // image ready to jump to now
    ((ImageInfo*)PMU430_BOOTAPP_START)->entrypoint();
}