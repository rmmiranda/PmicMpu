/*****************************************************************************
 * Filename:    rocket_adc.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     January 8, 2009
 *
 * Description: Rocket ADC Routines
 *
 * TODO: Platform specific timing/driver changes
 *          - Ensure Sample time is long enough (Section 18.2.5 in msp430f53x.pdf)
 *
 *          Voltage result can be calculated by
 *          mVResult = ( (adcResult * VOLTAGE_RANGE_IN_MV ) / (4096) );
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "intvector.h"
#include "gpio.h"
#include "delay.h"
#include "adc.h"
#if defined( PROCESSOR_PMU430 )
#include "rocket_adc_hw.h"
#else
#include "lagavulin_adc_hw.h"
#endif
#include "api.h"
#include "timer_mux.h"
#include "string.h"

#define VERBOSE_LEVEL 3
#include "eventlog.h"

// maps the PAD_PORT_3_X LS_IN configuration to the correct AdcChannel enum value
#if defined( LS_IN ) && defined( LS_PWR )
 #if (LS_IN <= PAD_PORT_3_6) && (LS_IN >= PAD_PORT_3_1 )
  #define LS_ADC_CHANNEL  (LS_IN - PAD_PORT_3_0)
 #endif
#endif


// helper macros for addressing each engine over i2c
// upper 3 bits determines which engine is being configured, lower
// 5 bits indicate which i2c register within the engine is being configured
#define ADC_ENGINE_INDEX(reg)           ( (0xE0 & reg)>>5 )
#define ADC_REGISTER_INDEX(reg)         ( 0x1F & reg )

#define ADC_ENGINE_TIMER(x)             ((TimerId)((int)ADC_ENGINE_TIMER_BASE+(x)))

#define ADC_INTMASK                     (ADC_CONFIG1_ADCIE | ADC_CONFIG1_MAXIE | ADC_CONFIG1_MINIE)


// Union to provide byte access to I2C word variables
typedef union
{
    WORD Val;
    struct {
        BYTE LB;
        BYTE HB;
    } byte;
} WORD_VAL;

typedef struct {

    /* 'External' I2C Registers */
    BYTE config0;
    BYTE config1;
    BYTE config2;
    BYTE muxchannel;
    BYTE status0;
    BYTE numSamples;
    WORD_VAL waitTime;
    WORD_VAL delayTime;
    WORD_VAL result;
    WORD_VAL maxThresh;
    WORD_VAL minThresh;
    WORD_VAL lastReading;

    /* 'Internal' state/support variables */
    DWORD runningTotal;
    WORD samplesRemaining;
    WORD maxReading;
    WORD minReading;
    BYTE state;
    BYTE inhCare;

} AdcEngine;
static AdcEngine adcEngine[ ADC_NUM_ENGINES_REAL ];

typedef enum {
    ADC_HOST_REQ,
    ADC_HOST_CANCEL,
    ADC_INH_CHANGE,
    ADC_TIMER_EXP,
    ADC_HW_DONE,
    ADC_NUM_EVENTS
} AdcEvent;

typedef enum {
    ADC_IDLE,
    ADC_WAIT_TRIGGER,
    ADC_WAIT_DELAY,
    ADC_WAIT_HW_START,
    ADC_WAIT_HW_DONE,
    ADC_WAIT_PERIOD,
    ADC_INHIBITED,
    ADC_NUM_STATES
} AdcState;

// HW arbitration regs
static BYTE engineWaitingForSample = 0; // eId+1, 0 == idle
static BYTE enginesWaitingForHw = 0;

// inhibit
static BOOL adcInhibit[ ADC_NUM_INHIBITS ];

static BYTE currentEngine;         // current engine being modified by i2c interface

// internal functions
static void adcTimerCb( WORD eId );
static void adcCompleteCb( WORD result );
static BYTE adcInhibitGetSource( AdcEngine *e );
static void updateAdcInterrupts( void );
static void adcStartConversion( BYTE eId );
static void adcSched( void );
static void adcSetState( BYTE eId, BYTE state );
static void adcSm( BYTE eId, BYTE evt, WORD data );

static BYTE adcRegRead( BYTE reg );
static void adcRegWrite( BYTE reg, BYTE value );
static void adcStopReceived( void );

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
static void adcTimerCb( WORD eId )
{
    adcSm( eId, ADC_TIMER_EXP, 0 );
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
static void adcCompleteCb( WORD result )
{
    if( engineWaitingForSample ) {
        adcSm( engineWaitingForSample - 1, ADC_HW_DONE, result );
    } else {
        V1( EventLogAdd( EVENTLOG_ADC_SAMPLE_NOT_EXPECTED ) );
        adcSched();
    }
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
void AdcInhibitChange( BYTE inh, BOOL active )
{
    if( active != adcInhibit[ inh ] ) {
        BYTE i;

        adcInhibit[ inh ] = active;
        for( i = 0; i < ADC_NUM_ENGINES_REAL; i++ ) {
            if( adcEngine[i].inhCare == inh ) {
                adcSm( i, ADC_INH_CHANGE, inh );
            }
        }
    }
}

// maps the various channels to an inhibit source
#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
static BYTE adcInhibitGetSource( AdcEngine *e )
{
    BYTE channel;

    channel = (e->config0 & ADC_CONFIG0_CHANNEL_MASK) >> 2;

    switch( channel ) {
        case ADC_CHANNEL_A4:
            return( ADC_INHIBIT_TESTGPIO );
        default:
            // IE: channel doesn't have an inhibit signal assigned to it
            return( ADC_NUM_INHIBITS );
    }
}

// updates the global ADC device interrupt status as required
#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
static void updateAdcInterrupts( void )
{
    BYTE x;

    // if any engines have a pending interrupt, signal host
    for( x = 0; x < ADC_NUM_ENGINES_REAL; x++ ) {
        if( adcEngine[x].config1 & adcEngine[x].status0 & ADC_INTMASK ) {
            if( x == PA_SLUMP_ENGN ) {
                // clear interrupt flag right away since we handle it below
                adcEngine[x].status0 &= ~ADC_INTMASK;
                // call PA slump detection complete handler instead of interrupting host
                PaSlumpConversionComplete( adcEngine[x].result.Val );
            } else {
                // signal host that ADC conversion is complete
                V1( EventLogAddN( EVENTLOG_ADC_INTERRUPT_HOST, x ) );
                ApiInterruptHost( ROCKET_INT_ADC );
                return;
            }
        }
    }
    // clear signal to host
    ApiClearInterruptHost( ROCKET_INT_ADC );
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
static void adcStartConversion( BYTE eId )
{
    AdcChannel adcCh;

    // keep track of which engine the HW is working for right now
    engineWaitingForSample = eId + 1;

    // channel select bits map directly to adc hardware channel
    adcCh = (AdcChannel)((adcEngine[eId].config0 & ADC_CONFIG0_CHANNEL_MASK)>>2);

    enginesWaitingForHw--;

    V3( EventLogAddN( EVENTLOG_ADC_SAMPLE_STARTED, eId | (enginesWaitingForHw)) );
    // move this engine to wait for HW sample
    adcEngine[eId].state = ADC_WAIT_HW_DONE;
#if defined( PROCESSOR_PMU430 )
    // trigger ADC Conversion to start
    AdcConvertHw( adcCh, (AdcMuxChannel)adcEngine[eId].muxchannel, CHREF_VREFP_AVSS, adcCompleteCb );
#else
    AdcConvertHw( adcCh, CHREF_VREFP_AVSS, adcCompleteCb );
#endif

}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
static void adcSched( void )
{
    BYTE i;

    // if engines are waiting for hw, give it to them
    if( enginesWaitingForHw && !engineWaitingForSample ) {
        for( i = 0; i < ADC_NUM_ENGINES_REAL; i++ ) {
            if( adcEngine[i].state == ADC_WAIT_HW_START ) {
                adcStartConversion( i );
                break;
            }
        }
    }
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
// called to initiate a state transition (enter/exit actions)
static void adcSetState( BYTE eId, BYTE state )
{
    AdcEngine *e = &adcEngine[ eId ];
    e->state = state;

    EventLogAddN( EVENTLOG_ADC_CHNGSTATE, eId<<8 | state );

    switch( state ) {

        case ADC_WAIT_TRIGGER: // trigger entry
            e->inhCare = adcInhibitGetSource( e );
            if( e->inhCare == ADC_NUM_INHIBITS || adcInhibit[ e->inhCare ] ) {
                // channel has no inhibit source, or inhibit is already set
                e->state = ADC_WAIT_DELAY;
            } else {
                break;
            }
            //-lint,fallthrough

        case ADC_WAIT_DELAY:
            if( e->delayTime.Val == 0 ) {
                e->state = ADC_WAIT_HW_START;
            } else {
                SetTimer( ADC_ENGINE_TIMER( eId ), e->delayTime.Val );
                break;
            }
            //-lint,fallthrough

        case ADC_WAIT_HW_START:
        hwStartCase:
            if( (e->config2 & (ADC_CONFIG2_INHIBIT_EN|ADC_CONFIG2_TRIGGER_MODE)) == ADC_CONFIG2_INHIBIT_EN ) {
                e->inhCare = adcInhibitGetSource( e );
            }
            if( e->inhCare != ADC_NUM_INHIBITS && adcInhibit[ e->inhCare ] ) {
                e->state = ADC_INHIBITED;
            } else {
                enginesWaitingForHw++;
                // if HW is idle, start the conversion now
                if( !engineWaitingForSample ) {
                    adcStartConversion( eId );
                } else {
                    EventLogAddN( EVENTLOG_ADC_WAIT_HW_START, engineWaitingForSample );
                }
            }
            break;

        case ADC_WAIT_PERIOD:
            if( e->waitTime.Val == 0 ) {
                e->state = ADC_WAIT_HW_START;
                goto hwStartCase;
            } else {
                SetTimer( ADC_ENGINE_TIMER( eId ), e->waitTime.Val );
            }
            break;

    }
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
// called when an event occurs
static void adcSm( BYTE eId, BYTE evt, WORD data )
{
    AdcEngine *e = &adcEngine[ eId ];

    EventLogAddN( EVENTLOG_ADC_SMENTRY, eId<<8 | evt );

    switch( e->state ) {

        case ADC_IDLE:
            if( evt == ADC_HOST_REQ ) {
                e->status0 |= ADC_STATUS0_BUSY;

                e->maxReading = 0;
                e->minReading = 0xFFFF;

                e->runningTotal = 0;

                switch( e->config2 & ADC_CONFIG2_MODE_MASK ) {

                    case ADC_CONFIG2_MODE_SINGLE:
                    case ADC_CONFIG2_MODE_RANGE:
                        e->samplesRemaining = 1;
                        break;

                    case ADC_CONFIG2_MODE_RANGE_AVG:
                        e->samplesRemaining = (1 << e->numSamples) + 1;
                        break;

                    case ADC_CONFIG2_MODE_AVERAGE:
                    case ADC_CONFIG2_MODE_MIN:
                    case ADC_CONFIG2_MODE_MAX:
                        e->samplesRemaining = 1 << e->numSamples;
                        break;

                    default:
                        break;
                }

                V1( EventLogAddN( EVENTLOG_ADC_START_ENGINE, ReadRtcDword() ) );

                // if we are taking a LS reading and rocket powers the LS, turn it on now
                // to allow it to powerup/settle
                #if defined( LS_IN ) && defined( LS_PWR )
                if( ((e->config0 & ADC_CONFIG0_CHANNEL_MASK)>>2) == LS_ADC_CHANNEL ) {
                    PadConfig( LS_PWR, PAD_OUTPUT_HIGH );
                }
                #endif

                if( (e->config2 & (ADC_CONFIG2_INHIBIT_EN|ADC_CONFIG2_TRIGGER_MODE)) == (ADC_CONFIG2_INHIBIT_EN|ADC_CONFIG2_TRIGGER_MODE) ) {
                    adcSetState( eId, ADC_WAIT_TRIGGER );
                } else {
                    adcSetState( eId, ADC_WAIT_DELAY );
                }
            }
            break;

        case ADC_WAIT_TRIGGER:
            if( evt == ADC_HOST_CANCEL ) {
                e->inhCare = ADC_NUM_INHIBITS;
                e->state = ADC_IDLE;
            } else if( evt == ADC_INH_CHANGE ) {
                e->inhCare = ADC_NUM_INHIBITS;
                adcSetState( eId, ADC_WAIT_DELAY );
            }
            break;

        case ADC_WAIT_DELAY:
            if( evt == ADC_HOST_CANCEL ) {
                StopTimer( ADC_ENGINE_TIMER( eId ) ); // stop timer
                e->state = ADC_IDLE;
            } else if( evt == ADC_TIMER_EXP ) {
                adcSetState( eId, ADC_WAIT_HW_START );
            }
            break;

        case ADC_WAIT_HW_START: // leave this state by calling adcSched from ADC isr
            if( evt == ADC_HOST_CANCEL ) {
                e->inhCare = ADC_NUM_INHIBITS;
                enginesWaitingForHw--;
                e->state = ADC_IDLE;
            } else if( evt == ADC_INH_CHANGE ) {
                enginesWaitingForHw--;
                e->state =  ADC_INHIBITED;
            }
            break;

        case ADC_WAIT_HW_DONE:
            if( evt == ADC_HOST_CANCEL ) {
                AdcHalt();
                e->inhCare = ADC_NUM_INHIBITS;
                e->state = ADC_IDLE;
                engineWaitingForSample = 0;
                adcSched();
            } else if( evt == ADC_INH_CHANGE ) {
                AdcHalt();
                e->state = ADC_INHIBITED;
                engineWaitingForSample = 0;
                adcSched();
            } else if( evt == ADC_HW_DONE ) {
                WORD engineIfg = 0;

                V3( EventLogAddN( EVENTLOG_ADC_SAMPLE_COMPLETE, data ) );

                e->lastReading.Val = data;

                // update engine appropriatelly

                if( data > e->maxReading ) {
                    e->maxReading = data;
                }

                if( data < e->minReading ) {
                    e->minReading = data;
                }

                switch( e->config2 & ADC_CONFIG2_MODE_MASK ) {

                    case ADC_CONFIG2_MODE_SINGLE:
                    case ADC_CONFIG2_MODE_AVERAGE:
                    case ADC_CONFIG2_MODE_MIN:
                    case ADC_CONFIG2_MODE_MAX:

                        e->runningTotal += data;
                        e->samplesRemaining--;
                        break;

                    case ADC_CONFIG2_MODE_RANGE_AVG:

                        if( e->samplesRemaining > 1 ) {
                            e->runningTotal += data;
                            e->samplesRemaining--;
                            break;
                        } else {
                            WORD tmp = e->runningTotal >> e->numSamples;
                            if( tmp <= data ) {
                                e->runningTotal += data - tmp;
                            } else {
                                e->runningTotal -= tmp - data;
                            }
                        }
                        data = e->runningTotal >> e->numSamples;
                        goto adcCheckRange;

                    case ADC_CONFIG2_MODE_RANGE:

                        e->runningTotal += data;
                    adcCheckRange:
                        // Max threshold met/exceeded
                        if( data >= e->maxThresh.Val ) {
                            engineIfg |= ADC_STATUS0_MAXIFG;
                            // flag us to interrupt host now
                            e->samplesRemaining = 0;
                        // Min threshold met/exceeded
                        } else if( data <= e->minThresh.Val ) {
                            engineIfg |= ADC_STATUS0_MINIFG;
                            // flag us to interrupt host now
                            e->samplesRemaining = 0;
                        }
                        break;

                    default:
                        // TODO: error
                        break;
                }

                if( e->samplesRemaining ) {
                    e->inhCare = ADC_NUM_INHIBITS;
                    engineWaitingForSample = 0;
                    adcSetState( eId, ADC_WAIT_PERIOD );
                } else {

                    // if we are taking a LS reading and rocket powers the LS, turn it off now
                    // to allow it to powerup/settle
                    #if defined( LS_IN ) && defined( LS_PWR )
                    if( ((e->config0 & ADC_CONFIG0_CHANNEL_MASK)>>2) == LS_ADC_CHANNEL ) {
                        PadConfig( LS_PWR, PAD_OUTPUT_LOW );
                    }
                    #endif

                    // Calculate the result register contents
                    switch( e->config2 & ADC_CONFIG2_MODE_MASK ) {
                        case ADC_CONFIG2_MODE_AVERAGE:
                            e->result.Val =  e->runningTotal >> e->numSamples;
                            break;
                        case ADC_CONFIG2_MODE_MIN:
                            e->result.Val = e->minReading;
                            break;
                        case ADC_CONFIG2_MODE_MAX:
                            e->result.Val = e->maxReading;
                            break;
                        case ADC_CONFIG2_MODE_SINGLE:
                        case ADC_CONFIG2_MODE_RANGE:
                        case ADC_CONFIG2_MODE_RANGE_AVG:
                            e->result.Val = data;
                            break;
                         default:
                            break;
                    }
                    V2( EventLogAddN( EVENTLOG_ADC_ENGINE_DONE, e->result.Val ) );

                    // Generate any appropriate interrupts to the host & flag them in the status register
                    // Engine reading complete interrupt flag
                    engineIfg |= ADC_STATUS0_ADCIFG;

                    // set interrupt flags, and clear ADC BUSY flag
                    e->status0 = engineIfg;

                    // if the interrupt is enabled, and the flag has been set, signal the host
                    if( e->config1 & e->status0 & ADC_INTMASK ) {
                        V1( EventLogAddN( EVENTLOG_ADC_INTERRUPT_HOST, eId ) );
                        // signal host that ADC conversion is complete
                        //ApiInterruptHost( ROCKET_INT_ADC );
                        updateAdcInterrupts();
                    }
                    e->inhCare = ADC_NUM_INHIBITS;
                    engineWaitingForSample = 0;
                    e->state = ADC_IDLE;
                }
                adcSched();
            }
            break;

        case ADC_WAIT_PERIOD:
            if( evt == ADC_HOST_CANCEL ) {
                StopTimer( ADC_ENGINE_TIMER( eId ) );
                e->state = ADC_IDLE;
            } else if( evt == ADC_TIMER_EXP ) {
                adcSetState( eId, ADC_WAIT_HW_START );
            }
            break;

        case ADC_INHIBITED:
            if( evt == ADC_HOST_CANCEL ) {
                e->inhCare = ADC_NUM_INHIBITS;
                e->state = ADC_IDLE;
            } else if( evt == ADC_INH_CHANGE ) {
                adcSetState( eId, ADC_WAIT_HW_START );
            }
            break;

        default:
            // error
            break;
    }
}

// I2C Input Callbacks

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
BYTE adcRegRead( BYTE reg )
{
    AdcEngine *e;

    currentEngine = ADC_ENGINE_INDEX( reg );
    e = &adcEngine[ currentEngine ];

    // mask off the top 3 bits which indicate which engine we are talking to
    reg = ADC_REGISTER_INDEX( reg );

    switch( reg ) {
        case ADC_REG_CONFIG0(0):
            return( e->config0 );
        case ADC_REG_CONFIG1(0):
            return( e->config1 );
        case ADC_REG_CONFIG2(0):
            return( e->config2 );
        case ADC_REG_MUXCHANNEL(0):
            return( e->muxchannel );
        case ADC_REG_STATUS0(0):
            return( e->status0 );
        case ADC_REG_NUM_SAMPLES(0):
            return( e->numSamples );
        case ADC_REG_WAIT_TIME_MSB(0):
            return( e->waitTime.byte.HB );
        case ADC_REG_WAIT_TIME_LSB(0):
            return( e->waitTime.byte.LB );
        case ADC_REG_DELAY_TIME_MSB(0):
            return( e->delayTime.byte.HB );
        case ADC_REG_DELAY_TIME_LSB(0):
            return( e->delayTime.byte.LB );
        case ADC_REG_RESULT_MSB(0):
            // reading MSB of result auto-clears any interrupt flags
            e->status0 &= ~ADC_INTMASK;
            updateAdcInterrupts();
            return( e->result.byte.HB );
        case ADC_REG_RESULT_LSB(0):
            return( e->result.byte.LB );
        case ADC_REG_MAX_THRESH_MSB(0):
            return( e->maxThresh.byte.HB );
        case ADC_REG_MAX_THRESH_LSB(0):
            return( e->maxThresh.byte.LB );
        case ADC_REG_MIN_THRESH_MSB(0):
            return( e->minThresh.byte.HB );
        case ADC_REG_MIN_THRESH_LSB(0):
            return( e->minThresh.byte.LB );
        case ADC_REG_LASTREADING_MSB(0):
            // reading MSB of result auto-clears any interrupt flags
            e->status0 &= ~ADC_INTMASK;
            updateAdcInterrupts();
            return( e->lastReading.byte.HB );
        case ADC_REG_LASTREADING_LSB(0):
            return( e->lastReading.byte.LB );
        case ADC_REG_STATE(0):
            return( e->state );
        case ADC_REG_DEBUG1:
            return( engineWaitingForSample );
        case ADC_REG_DEBUG2:
            return( enginesWaitingForHw );
        default:
            return( 0 );
    }
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
void adcRegWrite( BYTE reg, BYTE value )
{
    AdcEngine *e;

    currentEngine = ADC_ENGINE_INDEX( reg );
    e = &adcEngine[ currentEngine ];

    // mask off the top 3 bits which indicate which engine we are talking to
    reg = ADC_REGISTER_INDEX( reg );

    switch( reg ) {
        case ADC_REG_CONFIG0(0):
            // if enable bit was set, and is being cleared this write, stop any pending ADC
            // operations for this engine.
            if( (e->config0 & ADC_CONFIG0_ENABLE) && !(value & ADC_CONFIG0_ENABLE) ) {
                // clear any pending interrupts
                e->status0 &= ~(ADC_STATUS0_BUSY | ADC_INTMASK);
                updateAdcInterrupts();
                adcSm( currentEngine, ADC_HOST_CANCEL, 0 );
            }
            e->config0 = value;
            break;
        case ADC_REG_CONFIG1(0):
            e->config1 = value;
            updateAdcInterrupts();
            break;
        case ADC_REG_CONFIG2(0):
            e->config2 = value;
            break;
        case ADC_REG_MUXCHANNEL(0):
            e->muxchannel = value;
            break;
        case ADC_REG_STATUS0(0):
            e->status0 = value;
            updateAdcInterrupts();
            break;
        case ADC_REG_NUM_SAMPLES(0):
            e->numSamples = value;
            break;
        case ADC_REG_WAIT_TIME_MSB(0):
            e->waitTime.byte.HB = value;
            break;
        case ADC_REG_WAIT_TIME_LSB(0):
            e->waitTime.byte.LB = value;
            break;
        case ADC_REG_DELAY_TIME_MSB(0):
            e->delayTime.byte.HB = value;
            break;
        case ADC_REG_DELAY_TIME_LSB(0):
            e->delayTime.byte.LB = value;
            break;
//        case ADC_REG_RESULT_MSB: // READ ONLY
//        case ADC_REG_RESULT_LSB: // READ ONLY
        case ADC_REG_MAX_THRESH_MSB(0):
            e->maxThresh.byte.HB = value;
            break;
        case ADC_REG_MAX_THRESH_LSB(0):
            e->maxThresh.byte.LB = value;
            break;
        case ADC_REG_MIN_THRESH_MSB(0):
            e->minThresh.byte.HB = value;
            break;
        case ADC_REG_MIN_THRESH_LSB(0):
            e->minThresh.byte.LB = value;
            break;
//        case ADC_REG_LASTREADING_MSB(0): // READ ONLY
//        case ADC_REG_LASTREADING_LSB(0): // READ ONLY
        default:
            break;
    }
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
void adcStopReceived( void )
{
    AdcEngine *e = &adcEngine[ currentEngine ];

    // are we being requested to start and ADC engine
    if( (e->config0 & (ADC_CONFIG0_START | ADC_CONFIG0_ENABLE)) == (ADC_CONFIG0_START | ADC_CONFIG0_ENABLE)  ) {
        // 'self clearing' START bit needs cleared
        e->config0 &= ~ADC_CONFIG0_START;
        // start the engine
        adcSm( currentEngine, ADC_HOST_REQ, 0 );
    }
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
void PaSlumpStartConversion( void )
{
    AdcEngine *e = &adcEngine[ PA_SLUMP_ENGN ];
    e->config0 |= ADC_CONFIG0_ENABLE;
    adcSm( PA_SLUMP_ENGN, ADC_HOST_REQ, 0 );
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
void PaSlumpStopConversion( void )
{
    AdcEngine *e = &adcEngine[ PA_SLUMP_ENGN ];
    // stop PA Slump conversion if its enabled
    if( (e->config0 & ADC_CONFIG0_ENABLE) ) {
        // clear any interrupts and busy flag
        e->status0 &= ~(ADC_STATUS0_BUSY | ADC_INTMASK);
        adcSm( currentEngine, ADC_HOST_CANCEL, 0 );
        e->config0 &= ~ADC_CONFIG0_ENABLE;
    }
}

/******************************************************************************
 * PaSlumpConfigAdcEngine()
 *
 * setup all the ADC engine registers for PA slump detection. this is a dedicated
 * engine for PA slump detection that the host does not have access to.
 *****************************************************************************/
#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
void PaSlumpConfigAdcEngine( void )
{
    AdcEngine *e = &adcEngine[ PA_SLUMP_ENGN ];

#if defined( PROCESSOR_FAMILY_MSP430 )
    // VBAT is on MUX
    e->config0 = ADC_CONFIG0_CHANNEL( ADC_CHANNEL_MUX );
    e->muxchannel = ADC_MUXCHANNEL_VBAT;
#else
    e->config0 = ADC_CONFIG0_CHANNEL( ADC_CHANNEL_VBAT );
#endif

    // take sample as quick as possible and as soon as interrupt occurs (turns out
    // this is approx 100uS before first sample, and 100uS between samples)
    e->waitTime.Val = 0;
    e->delayTime.Val = 0;

    // keep track of minimum value read over the range of samples
    e->config2 = ADC_CONFIG2_MODE_AVERAGE;

    // take X samples after the trigger occurs (2^2) = 4 samples
    e->numSamples = 2;

    // 'interrupt' us when conversion is complete (IE, call our callback)
    e->config1 = ADC_CONFIG1_ADCIE;

}

/**
 * AdcInit()
 * ADC12CLK Frequency = SMCLK / 4
 *
 * Total ADCTime = (( 2^(SHTn+2) )/ADC12CLK) + ((AdcResolution+1)/ADC12CLK)
 *
 * Currently configured for :
 *          77uS = (( 2^(4+2) )/1000000) + ((12+1)/1000000)
 *
 * NOTE: Assumes AdcInitHw() has been called during BOOT
 */
#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
void AdcInit( void )
{
    int i;

    for( i = 0; i < ADC_NUM_ENGINES_REAL; i++ ) {
        RegisterTimerExpiry( ADC_ENGINE_TIMER(i), adcTimerCb, i );
    }

    ApiRegisterHandlers( DEVID_ADC, adcRegRead, adcRegWrite, NULL, adcStopReceived );

    // reset driver state
    memset( adcEngine, 0, sizeof(AdcEngine) * ADC_NUM_ENGINES_REAL );
    for( i = 0; i < ADC_NUM_ENGINES_REAL; i++ ) {
        adcEngine[i].inhCare = ADC_NUM_INHIBITS;
    }
    memset( adcInhibit, 0, sizeof(BOOL) * ADC_NUM_INHIBITS );
    engineWaitingForSample = 0;
    enginesWaitingForHw = 0;

//    RegisterTimerExpiry( ADC_INHIBIT_DEBUG_TIMER, testGpioInhibitCb, 0 );
//    SetTimer( ADC_INHIBIT_DEBUG_TIMER, 32768 );
}

#if defined( PROCESSOR_MQ31 )
#pragma location="SECONDARY_CODE"
#endif
void AdcUninit( void )
{
    int i;

    AdcHalt();
    engineWaitingForSample = 0;
//    StopTimer( ADC_INHIBIT_DEBUG_TIMER );
    for( i = 0; i < ADC_NUM_ENGINES_REAL; i++ ) {
        adcEngine[i].state = ADC_IDLE;
        StopTimer( ADC_ENGINE_TIMER(i) );
    }
    ApiUnregisterHandlers( DEVID_ADC );
}

