/*****************************************************************************
 * Filename:    timer_mux.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 22, 2010
 *
 * Description: PMIC Timer Defines
 ****************************************************************************/
#ifndef _TIMER_MUX_H_
#define _TIMER_MUX_H_

#include "basetype.h"
#include "timer.h"
#if defined( PROCESSOR_PMU430 )
#include "rocket_registers.h"
#else
#include "lagavulin_registers.h"
#endif
#include "gpio.h"

#define RTC_TICKS_PER_SECOND    (32768u)

#define MSEC_TO_RTC_TICKS( X ) (DWORD)((DWORD)(((DWORD)(X)*RTC_TICKS_PER_SECOND)/1000))
#define USEC_TO_RTC_TICKS( X ) (DWORD)((DWORD)(((DWORD)(X)*RTC_TICKS_PER_SECOND)/1000000))

typedef enum {
    WATCHDOG_TIMER,
    HEARTBEAT_TIMER,
    STATUS_LED_TIMER,
    ADC_ENGINE_TIMER_BASE,
    TEST_TIMER = ADC_ENGINE_TIMER_BASE + ADC_NUM_ENGINES + 1,
    HOSTCTRL_TIMER,
    SUPPLY_SEQUENCE_TIMER,
    UTC_TIMER,
    ADC_INHIBIT_DEBUG_TIMER,
    ADC_RAW_TIMER,
    I2C_FG_SPEED_TIMER,
#if defined( AUD_PROG_EN )
    AUD_PROG_EN_TIMER,
#endif
#if defined( RIM_KEYVOL_RESET )
    KEYVOL_UP_DN_TIMER,
#endif
    RTC_EXTERNAL_XTAL_TIMER,
#if defined( DIGRF_CLK_EN )
    DIGRF_CLK_EN_TIMER,
#endif
#if defined( RIM_BRICK_MODE )
    BRICK_TIMER,
#endif
    NUM_TIMERS
} TimerId;

void TimerInit( void );
void SetTimer( TimerId timer, DWORD ticksFromNow );
void StopTimer( TimerId timer );
void RegisterTimerExpiry( TimerId timer, FUNCTION1 func, WORD data );

#define ReadRtcDword() GetHWTimerCountDword()
#define ReadRtcWord() GetHWTimerCountWord()

#endif /*_TIMER_MUX_H_*/
