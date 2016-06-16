/*****************************************************************************
 * Filename:    lagavulin_metrics.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Lagavulin
 *
 * Created:     March 4, 2011
 *
 * Description: Lagavulin metrics definitions (copied from rocket_metrics which
 * 				was authored by Chris Book and created on November 21, 2008).
 ****************************************************************************/

#ifndef _LAGAVULIN_METRICS_H_
#define _LAGAVULIN_METRICS_H_

#include "basetype.h"

#define LAGAVULIN_METRICS_VERSION_MAJOR		1
#define LAGAVULIN_METRICS_VERSION_MINOR		0

#define IMAGE_TYPE_EMPTY     0x0000
#define IMAGE_TYPE_BOOT      0x0001
#define IMAGE_TYPE_APP       0x0002

#define IMAGE_INFO_COOKIE    0xA596

#define FLASHMSG_APPLY_BOOT_UPDATE     (0x01)
typedef struct {
    volatile WORD msg;       // 'Message/flag' for the bootloader to read/act on
    volatile WORD key;       // key to ensure flag wasn't set in error somehow
} UpdateFlashMsg;

typedef struct {
    FUNCTION entrypoint;
    WORD cookie;            // cookie used for initial search of info header
    WORD versionMaj;        // Pmic API version major
    WORD versionMin;        // Pmic API version minor
    WORD loadAddr;          // location where image should be located (starting with image info struct)
    WORD endAddr;           // Size of image in bytes including image info struct
    WORD type;              // type of image
    WORD pairCrc;           // CRC of APP/BOOT pair image (used to make sure we are running a matched pair)
    WORD crc;               // checksum of entire image (everything except the checksum number)
}ImageInfo;

typedef struct {
    WORD  MetricsVersionMaj;
    WORD  MetricsVersionMin;
    WORD   MetricsSize;
    WORD  BuildVersionMaj;     // Lagavulin build version major
    WORD  BuildVersionMin;     // Lagavulin build version minor
    WORD  BuildInfoMaj;        // Lagavulin build info major
    WORD  BuildInfoMin;        // Lagavulin build info minor
    char    BuildUserName[16];
    char    BuildDate[16];
    char    BuildTime[12];
    const ImageInfo* BootImage;
    const ImageInfo* AppImage;
    WORD   HwvHardwareId;
    BYTE    HwvPwrSeq;

} LagavulinMetricsStruct;

extern const LagavulinMetricsStruct LagavulinMetrics;

// Entrypoint for BOOT section
void _c_int00( void );
// Entrypoint for APP section
void AppEntrypoint( void );
void AppExitpoint( void );

extern BOOL AppCallsAllowed;

BOOL IsImageValid( ImageInfo *imageLocation, WORD expectedCrc, WORD expectedPairCrc );

void UpdaterRegWrite( BYTE reg, BYTE data );
BYTE UpdaterRegRead( BYTE reg );
void UpdaterStartReceived( void );
void UpdaterStopReceived( void );

void Mq31UpdateDebug( void );

#endif

