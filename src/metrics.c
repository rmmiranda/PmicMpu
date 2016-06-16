/*****************************************************************************
 * Filename:    metrics.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     November 21, 2008
 *
 * Description: Rocket metrics file
 ****************************************************************************/

#include "basetype.h"
#include "rocket_metrics.h"
#include "version.h"
#include "ApiVersion.h"

#if defined( ROCKET_LOADER )
#include "config_hwvinfo.h"
#endif

#if !defined( CC4_PROJECT )
 #include "builder.h"
#else
 #define BUILD_USERNAME "CC4_PROJECT"
#endif

#if defined( ROCKET_LOADER)

#if defined( DEVID_UPDATER_SUPPORT )
#pragma DATA_SECTION( BootloaderMsg, ".loader_msg" )
const UpdateFlashMsg BootloaderMsg = {
    0,
    0
};
#endif

#pragma DATA_SECTION( RocketBootloaderMetrics, ".loader_metrics" )
const RocketMetricsStruct RocketBootloaderMetrics = {
    ROCKET_METRICS_VERSION,
    sizeof(RocketMetricsStruct),
    ROCKET_VERSION,
    ROCKET_BUILDINFO,
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
#pragma DATA_SECTION( BootloaderMsg, ".loader_msg" )
UpdateFlashMsg BootloaderMsg;
#endif

/**
 * Create image info struct located at the beginning of each image used when updating
 */
#if defined( DEVID_UPDATER_SUPPORT )
#pragma DATA_SECTION( BootImageInfo, ".boot_info" )
#endif
const ImageInfo BootImageInfo = {
    0,                              // CRC place holder
    0,                              // PairCRC place holder
    _c_int00,
    IMAGE_INFO_COOKIE,
    ROCKET_API_VERSION,
    (WORD)_start_of_boot_image,
    (WORD)_end_of_boot_image,
    IMAGE_TYPE_BOOT
};

#if defined( DEVID_UPDATER_SUPPORT )
#pragma DATA_SECTION( AppImageInfo, ".app_info" )
#endif
const ImageInfo AppImageInfo = {
    0,                              // CRC place holder
    0,                              // PairCRC place holder
    AppEntrypoint,
    IMAGE_INFO_COOKIE,
    ROCKET_API_VERSION,
    (WORD)_start_of_app_image,
    (WORD)_end_of_app_image,
    IMAGE_TYPE_APP
};

const RocketMetricsStruct RocketMetrics = {
    ROCKET_METRICS_VERSION,
    sizeof(RocketMetricsStruct),
    ROCKET_VERSION,
    ROCKET_BUILDINFO,
    BUILD_USERNAME,
    __DATE__,
    __TIME__,
    &BootImageInfo,
    &AppImageInfo,
    0,       // unused
    0        // unused
};

#endif
