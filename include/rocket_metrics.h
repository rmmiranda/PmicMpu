/*****************************************************************************
 * Filename:    rocket_metrics.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     November 21, 2008
 *
 * Description: Rocket metrics definitions
 ****************************************************************************/

#ifndef _ROCKET_METRICS_H_
#define _ROCKET_METRICS_H_

#include "basetype.h"

#define ROCKET_METRICS_VERSION_MAJOR  1
#define ROCKET_METRICS_VERSION_MINOR  0
#define ROCKET_METRICS_VERSION  ((ROCKET_METRICS_VERSION_MAJOR<<8)|ROCKET_METRICS_VERSION_MINOR)

#define IMAGE_TYPE_EMPTY     0x00
#define IMAGE_TYPE_BOOT      0x01
#define IMAGE_TYPE_APP       0x02

#define IMAGE_INFO_COOKIE    0xA596

#define FLASHMSG_APPLY_BOOT_UPDATE     (0x01)
typedef struct {
    volatile WORD msg;       // 'Message/flag' for the bootloader to read/act on
    volatile WORD key;       // key to ensure flag wasn't set in error somehow
} UpdateFlashMsg;

typedef struct {
    WORD crc;               // checksum of entire image (everything except the checksum number)
    WORD pairCrc;           // CRC of APP/BOOT pair image (used to make sure we are running a matched pair)
    FUNCTION entrypoint;
    WORD cookie;            // cookie used for initial search of info header
    WORD version;           // Rocket API version
    WORD loadAddr;          // location where image should be located (starting with image info struct)
    WORD endAddr;           // Size of image in bytes including image info struct
    BYTE type;              // type of image
    BYTE reserved[1];       // pad info struct to 16 bytes
}ImageInfo;

typedef struct {
    WORD   MetricsVersion;
    WORD   MetricsSize;
    WORD   RocketVersion;       // Rocket build version
    WORD   BuildInfo;           // rocket build info ( SCM/Developer and minor build#)
    char    BuildUserName[16];
    char    BuildDate[16];
    char    BuildTime[12];
    const ImageInfo* BootImage;
    const ImageInfo* AppImage;
    DWORD   HwvHardwareId;
    BYTE    HwvPwrSeq;

} RocketMetricsStruct;

extern const RocketMetricsStruct RocketMetrics;

// Entrypoint for BOOT section
void _c_int00( void );
// Entrypoint for APP section
void AppEntrypoint( void );
void AppExitpoint( void );

// Symbols genereated by the linker related to location/flash allocation
// These are not functions, but need to be declared like this or the linker blows up
void _start_of_boot_sec( void );
void _end_of_boot_sec( void );

#if !defined( ROCKET_LOADER )
void _start_of_boot_image( void );
void _end_of_boot_image( void );
void _start_of_app_image( void );
void _end_of_app_image( void );
#endif

#define PMU430_BOOTAPP_START        ( (WORD)_start_of_boot_sec )
#define PMU430_BOOTAPP_END          ( (WORD)_end_of_boot_sec )
#define PMU430_BOOTAPP_SIZE         ( PMU430_BOOTAPP_END - PMU430_BOOTAPP_START )

#define PMU430_MAX_BOOT_SIZE        ( PMU430_BOOTAPP_SIZE/2 )
#define PMU430_BOOT_UPDATE_START    ( PMU430_BOOTAPP_START + PMU430_MAX_BOOT_SIZE )

#define PMU430_LOADER_START         ( (WORD)_end_of_boot_sec )

extern BOOL AppCallsAllowed;

BOOL IsImageValid( ImageInfo *imageLocation, WORD expectedCrc, WORD expectedPairCrc );

void UpdaterRegWrite( BYTE reg, BYTE data );
BYTE UpdaterRegRead( BYTE reg );
void UpdaterStartReceived( void );
void UpdaterStopReceived( void );

void Pmu430UpdateDebug( void );

#endif

