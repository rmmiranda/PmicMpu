/*****************************************************************************
 * Filename:    app_init.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Aug 31, 2009
 *
 * Description: Rocket app init routines, called once the APP section has
 *              been verified and/or once a new app section is loaded
 *              from the host
 ****************************************************************************/

#include "critical.h"
#include "registers.h"
#include "intvector.h"
#if defined( PROCESSOR_PMU430 )
#include "rocket_metrics.h"
#else
#if defined( PROCESSOR_MQ31 )
#include "lagavulin_metrics.h"
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430
#include "adc.h"
#include "timer_mux.h"
#include "led.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

void FGInit( void );
void FGUninit( void );
void GpioRelayInit( void );
void GpioRelayUninit( void );

/**
 * AppEntrypoint()
 *
 * Called during init / after an APP update once we have determined
 * the app section is valid and ready to run
 */
void AppEntrypoint( void )
{

    V1( EventLogAdd( EVENTLOG_ADC_ENGINE_INIT ) );
    AdcInit();

    HostCtrlPaSlumpInit();

#if defined( PROCESSOR_PMU430 )
    #if defined( HOSTPROCESSOR_TAVOR_MG ) || defined( HOSTPROCESSOR_ESHEL )
    HostCtrlPwrI2cInit();
    #endif

    LedInitHw();
    LedInit();

    HostCtrlPwrInit();

    I2cRlyInit();

    GpioRelayInit();

    FGInit();

#if defined( RIM_BRICK_MODE )
    FtWatchdogInit();
#endif
#endif // PROCESSOR_PMU430

#if defined( RIM_PWM )
    PwmInit();
#endif
}

/**
 * AppExitpoint()
 *
 * Called before we stomp on the APP section with an update image
 * to de-register anything and stop anything APP related
 */
void AppExitpoint( void )
{
#if defined( RIM_PWM )
    PwmUninit();
#endif

#if defined( PROCESSOR_PMU430 )
    HostCtrlPwrUninit();

#if defined( RIM_BRICK_MODE )
    FtWatchdogUninit();
#endif

    FGUninit();

    GpioRelayUninit();

    I2cRlyUninit();

    #if defined( HOSTPROCESSOR_TAVOR_MG ) || defined( HOSTPROCESSOR_ESHEL )
    HostCtrlPwrI2cUninit();
    #endif
#endif // PROCESSOR_PMU430

    HostCtrlPaSlumpUninit();

    AdcUninit();
}
