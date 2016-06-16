/*****************************************************************************
 * Filename:    ApiVersion.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Description: Rocket Register API Version Information
 *
 *              Any changes to the interface exposed to the host (ie
 *              rocket_registers.h) must include an update to this rev
 *
 *  2010-06-18  1.1  BH  Added KEYVOL feature control bit
 *  2010-07-15  1.2  BH  Added UPDATER_REG_STATUS bits
 *  2010-08-15  1.3  BH  Added deepsleep/fastreset and Bonsai Active flags
 *  2010-09-03  1.4  BH  Added ALLOW_AUD_PROG to prevent uninteded AUD_PROG handling
 *  2010-11-03  1.5  BH  Added ROCKET_RESETCTRL_WARMRST and other SYSFLAGS to support fast/warm reset
 *  2011-01-21  1.6  BH  Added ROCKET_RESET_PGFAULT as reset indicator for brownout of system supplies
 *  2011-02-17  1.7  BH  Added DEVID_PWRI2C interface for MG power i2c requests without 25mV step size restriction
 *  2011-04-06  1.8  BH  Added DEVID_FT interface for BRICK mode functionality and FT Watchdog
 *  2011-05-17  1.9  BH  Added DEVID_PASLUMP for radio PA slump detection feature
 * ****************************************************************************/
#ifndef _APIVERSION_H_
#define _APIVERSION_H_

#define PMIC_API_VERSION_MAJOR    1
#define PMIC_API_VERSION_MINOR    9

#if defined( PROCESSOR_PMU430 )
#define ROCKET_API_VERSION    ( ( PMIC_API_VERSION_MAJOR << 8 ) | \
                                ( PMIC_API_VERSION_MINOR ) )
#endif

#endif

