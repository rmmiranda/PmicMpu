/*****************************************************************************
 * Filename:    lagavulin_metrics.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 4, 2011
 *
 * Description: Lagavulin metrics file (based from metrics.c which was authored
 * 				by Chris Book and created on November 21, 2008).
 ****************************************************************************/

#include "basetype.h"
#include "lagavulin_metrics.h"
#include "lagavulin_version.h"
#include "updater.h"
#include "ApiVersion.h"

#if defined( LAGAVULIN_LOADER )
#include "config_hwvinfo.h"
#endif

#if 0 //!defined( CC4_PROJECT )
 #include "builder.h"
#else
 #define BUILD_USERNAME "CC4_PROJECT"
#endif

#if defined( RIM_MODEL_GAUGE )
#pragma location="FG_PROFILE"
const MG_t FgProfile = { 
    0,
    0
    //TODO
};
#endif

#if defined( LAGAVULIN_LOADER)

#if defined( DEVID_UPDATER_SUPPORT )
#pragma location="BOOT_LOADER_MSG"
const UpdateFlashMsg BootloaderMsg = {
    0,
    0
};
#endif

#pragma location="BOOT_IMAGE_INFO"
const LagavulinMetricsStruct LagavulinBootloaderMetrics = {
	LAGAVULIN_METRICS_VERSION_MAJOR,
	LAGAVULIN_METRICS_VERSION_MINOR,
    sizeof(LagavulinMetricsStruct),
    LAGAVULIN_VERSION_MAJOR,
    LAGAVULIN_VERSION_MINOR,
    LAGAVULIN_RELEASE,
    LAGAVULIN_BUILD,
    BUILD_USERNAME,
    __DATE__,
    __TIME__,
    NULL,           // unused
    NULL,           // unused
    HW_VERSION_HARDWAREID,
    HW_VERSION_CLASS__PWRSEQ,
};

#else

#if defined( DEVID_UPDATER_SUPPORT )
#pragma location="BOOT_LOADER_MSG"
__no_init UpdateFlashMsg BootloaderMsg;
#endif

/**
 * Create image info struct located at the beginning of each image used when updating
 */
#if defined( DEVID_UPDATER_SUPPORT )
#pragma location="BOOT_IMAGE_INFO"
#endif
const ImageInfo BootImageInfo = {
    0, // _c_int00,
    IMAGE_INFO_COOKIE,
    PMIC_API_VERSION_MAJOR,
    PMIC_API_VERSION_MINOR,
    MQ31_BOOTAPP_START, //_start_of_boot_image,
    MQ31_BOOTAPP_END, //_end_of_boot_image,
    IMAGE_TYPE_BOOT,
    0,                              // PairCRC place holder
    0,                              // CRC place holder
};

#if defined( DEVID_UPDATER_SUPPORT )
#pragma location="APP_IMAGE_INFO"
#endif
const ImageInfo AppImageInfo = {
    AppEntrypoint,
    IMAGE_INFO_COOKIE,
    PMIC_API_VERSION_MAJOR,
    PMIC_API_VERSION_MINOR,
    MQ31_UPDATE_START, //_start_of_app_image,
    MQ31_UPDATE_END, //_end_of_app_image,
    IMAGE_TYPE_APP,
    0,                              // PairCRC place holder
    0,                              // CRC place holder
};

const LagavulinMetricsStruct LagavulinMetrics = {
	LAGAVULIN_METRICS_VERSION_MAJOR,
	LAGAVULIN_METRICS_VERSION_MINOR,
    sizeof(LagavulinMetricsStruct),
    LAGAVULIN_VERSION_MAJOR,
    LAGAVULIN_VERSION_MINOR,
    LAGAVULIN_RELEASE,
    LAGAVULIN_BUILD,
    BUILD_USERNAME,
    __DATE__,
    __TIME__,
    &BootImageInfo,
    &AppImageInfo,
    0,       // unused
    0        // unused
};

#endif
