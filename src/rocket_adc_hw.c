/*****************************************************************************
 * Filename:    rocket_adc_hw.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Sept 14, 2009
 *
 * Description: Rocket HW ADC Routines
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "delay.h"
#include "registers.h"
#include "intvector.h"
#include "rocket_adc_hw.h"
#include "rocket_registers.h"
#include "spi.h"
#include "timer_mux.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

FUNCTION1 ConvCompleteCallback = NULL;
WORD SampleResult;
BOOL MuxEnabled = FALSE;

// RAW ADC register interface
#define RAW_SAMPLE_INTERVAL_TICKS     ( 16 )        // 0.5ms interval
static volatile BYTE Config0;
static BYTE MuxAdcChannel;
static BYTE RawNumSamples;
static BOOL RawSampling;
static DWORD RawResultTotal;
static WORD RawResultAvg;

void AdcRawStopReceived( void );
void AdcRawRegWrite( BYTE reg, BYTE value );
BYTE AdcRawRegRead( BYTE reg );

static void AdcRawCb( WORD data );

/**************************************************************************
 * Function:    InitRefGen( void )
 *
 * Descrption:  Configure the internal voltage ref gen used by the ADC
 *
 *************************************************************************/
static void InitRefGen( void )
{

    // Use the Reference module as the master (don't let ADC module control the REF).
    // Set it for 2.0V ref, always on, and output externally for testing
    // TODO: How come we need to use REFON if the ADC is configured for ADC12CTL2_REFBURST ?
    // is that supposed to enable/disable it as required? without REFON using the REF module
    // doesn't seem to work right.
#if defined( ADC_REF )
    WriteRegWord( REFCTL0_REG16, REFCTL0_REFMSTR | REFCTL0_REFVSEL_2V5 | REFCTL0_REFON | REFCTL0_REFOUT );
    // set direction to output, and enable the alt mode for the REFOUT
    PadConfig( ADC_REF, PAD_OUTPUT_LOW );
    PadConfig( ADC_REF, PAD_ALT_MODE );
#else
    WriteRegWord( REFCTL0_REG16, REFCTL0_REFMSTR | REFCTL0_REFVSEL_2V5 | REFCTL0_REFON );
#endif
}

/**************************************************************************
 * Function:    AdcReadSampleHw( void )
 *
 * Descrption:  Retrieves the last sample completed by the ADC HW
 *
 *************************************************************************/
WORD AdcReadSampleHw( void )
{
    return( SampleResult );
}

/**************************************************************************
 * Function:    AdcCompleteIsrHw( void )
 *
 * Descrption:  Called when a requested ADC conversion is complete
 *
 *************************************************************************/
void AdcCompleteIsrHw( void )
{

    EventLogAdd( EVENTLOG_PMU430_ADC_INT );
    // diasble the 'sample complete' interrupt
    WriteRegWord( ADC12IE_REG16, 0x00 );

    // if mux was active; turn it off to reduce power consumption
    if( MuxEnabled ) {
        PmicWrite( PMIC_REG_ADCIN_SEL, CH_MUXCHANNEL_DISABLED );
        MuxEnabled = FALSE;
    }

    // grab the result from the register (also clears interrupt)
    SampleResult = ReadRegWord( ADC12MEM_N_REG16(0) );

    V2( EventLogAddN( EVENTLOG_ADC_SAMPLE_COMPLETE, SampleResult ) );

    if( ConvCompleteCallback != NULL ) {
        ConvCompleteCallback( SampleResult );
    }
}

/**************************************************************************
 * Function:    AdcInitHw( void )
 *
 * Descrption:  Low level ADC init routine
 *
 *************************************************************************/
void AdcInitHw( void )
{
    ApiRegisterHandlers( DEVID_ADC_RAW, AdcRawRegRead, AdcRawRegWrite, NULL, AdcRawStopReceived );
    RegisterTimerExpiry( ADC_RAW_TIMER, AdcRawCb, 0 );

    Config0 = 0;
    MuxAdcChannel = 0;
    RawResultAvg = 0;

    // configure the internal reference generator
    InitRefGen();

    RegisterIntHandler( ADC_VECTOR, AdcCompleteIsrHw );

    // make sure module is off so we can configure registers (defaults to off ... this is not required)
    WriteRegWordAnd( ADC12CTL0_REG16, ~ADC12CTL0_ENC );

    // configure conversion results sequence to always start at mem location addr 0
    WriteRegWordAnd( ADC12CTL1_REG16, ~0x0FFF );

    // Configure Sample Time in ADC12CLKCyclesx4 increments  ie Sample Time of ( 2^(SHTn+2) )/ADC12CLK
    WriteRegWord( ADC12CTL0_REG16, (4<<12) | (4<<8) | ADC12CTL0_ON );

    // Sample/Hold pulse mode enabled (Sample Timer determines S/H time), Conversion sequence mode = single-channel, single-conversion
    WriteRegWord( ADC12CTL1_REG16, ADC12CTL1_SHP | ADC12CTL1_SSEL_SMCLK | ADC12CTL1_CONSEQ_SCH_SC );

    // pre-divide ADC12CLK by 4, Enable Temperature Sensor, Set ADC resolution to 12-bit, dataformat binary unsigned
    // sampling rate supports up to ~200ksps
#if defined( ADC_REFBURST_ENABLE )
    WriteRegWord( ADC12CTL2_REG16, ADC12CTL2_PDIV | ADC12CTL2_RES_12BIT | ADC12CTL2_REFBURST );
#else
    WriteRegWord( ADC12CTL2_REG16, ADC12CTL2_PDIV | ADC12CTL2_RES_12BIT );
#endif
}

/**************************************************************************
 * Function:    AdcConfigISource
 *
 * Descrption:  Enables/disables the current source on the specific
 *              ADC P3.X channel.
 *
 *              Use chMux to specify the source strength (3, 10, 50uA)
 *
 *************************************************************************/
void AdcConfigISource( AdcChannel ch, AdcMuxChannel chMux, BOOL en )
{
    if( en ) {
        PmicWrite( PMIC_REG_ADCIN_SEL, chMux );
        WriteRegByte( ADCP3CE_REG8, ADCP3CE_EN(ch)  );
    } else {
        WriteRegByte( ADCP3CE_REG8, 0 );
        PmicWrite( PMIC_REG_ADCIN_SEL, 0 );
    }
}

/**************************************************************************
 * Function:    AdcConvertHw( AdcChannel ch, AdcChannelRange cr, FUNCTION ccCallback )
 *
 * Descrption:  Requests a conversion for the specified channel, given the
 *              specified sample range. When sample is complete, conversion
 *              complete callback is called. If callback is NULL AdcConvertHw
 *              won't return until sample is complete
 *
 *************************************************************************/
void AdcConvertHw( AdcChannel ch, AdcMuxChannel chMux, AdcChannelRef cr, FUNCTION1 ccCallback )
{
    WORD wason = CoreIntDisable();

    // Disable Conversions
    WriteRegWordAnd( ADC12CTL0_REG16, ~ADC12CTL0_ENC );

    // setup conversion on specific input channel using specific reference
    WriteRegByte( ADC12MCTL_N_REG8(0), cr<<4 | ch );

    if( ch == CH_MUX ) {
        MuxEnabled = TRUE;
        // configure ADC mux
        PmicWrite( PMIC_REG_ADCIN_SEL, chMux );
        // 20uS tsettle required when switching mux channel
        HwDelay( 20000 );
    }

    // clear any pending interrupts
    WriteRegWord( ADC12IFG_REG16, 0x00 );

    // if we have a callback
    if( ccCallback ) {
        // enable the 'sample complete' interrupt
        WriteRegWord( ADC12IE_REG16, 1<<0 );

        ConvCompleteCallback = ccCallback;
    }

    // Enable Conversions again
    WriteRegWordOr( ADC12CTL0_REG16, ADC12CTL0_ENC );

    // Trigger conversion to start
    WriteRegWordOr( ADC12CTL0_REG16, ADC12CTL0_SC );

    V2( EventLogAdd( EVENTLOG_ADC_SAMPLE_CONFIGURED ) );

    // if we didn't have a callback, lets poll until the conversion is complete
    if( ccCallback == NULL ) {
        V2( EventLogAdd( EVENTLOG_ADC_POLLING ) );
        while( (ReadRegWord( ADC12IFG_REG16 ) & 0x1) == 0 ) {};

        SampleResult = ReadRegWord( ADC12MEM_N_REG16(0) );

        // if mux was active; turn it off to reduce power consumption
        if( MuxEnabled ) {
            PmicWrite( PMIC_REG_ADCIN_SEL, CH_MUXCHANNEL_DISABLED );
            MuxEnabled = FALSE;
        }

        V2( EventLogAddN( EVENTLOG_ADC_SAMPLE_COMPLETE, SampleResult ) );
    }

    CoreIntEnable( wason );
}

/**************************************************************************
 * Function:    AdcRawCb( void )
 *
 * Descrption:  Does a 32 sample conversion of the specified channel
 *              used by host Bootrom for quick battery measurement
 *              before we know the APP section is "OK/up to date"
 *
 *************************************************************************/
static void AdcRawCb( WORD data )
{

    // if we were just sampling, get the result and schedule the timer
    if( RawSampling ) {
        RawSampling = FALSE;

        RawResultTotal += SampleResult;
        RawNumSamples++;

        // we have enough samples. calc the average and flag the host
        if( RawNumSamples > 32 ) {
            // Total/32 = avg
            RawResultAvg = RawResultTotal >> 5;
            // clear the busy flag
            Config0 &= ~ADC_RAW_CONFIG0_BUSY;

        } else {
            // we need more samples. schedule the timer
            SetTimer( ADC_RAW_TIMER, RAW_SAMPLE_INTERVAL_TICKS );
        }

    } else {
        RawSampling = TRUE;
        AdcConvertHw( (AdcChannel)Config0>>2, (AdcMuxChannel)MuxAdcChannel, CHREF_AVCC_AVSS, AdcRawCb );
    }

}

/**************************************************************************
 * Function:    AdcRawConvert( void )
 *
 * Descrption:  Does a 32 sample conversion of the specified channel
 *              used by host Bootrom for quick battery measurement
 *              before we know the APP section is "OK/up to date"
 *
 *************************************************************************/
static void AdcRawConvert( void )
{
    RawNumSamples = 0;
    RawResultTotal = 0;
    RawResultAvg = 0;
    RawSampling = TRUE;
    AdcConvertHw( (AdcChannel)Config0>>2, (AdcMuxChannel)MuxAdcChannel, CHREF_AVCC_AVSS, AdcRawCb );
}

/**************************************************************************
 * Function:    AdcRawConvertVsys( void )
 *
 * Descrption:  Does a 32 sample conversion of vsys. Should only be
 *              used when the host is not up and running since it hijaks
 *              the RAW adc device engine
 *
 *              NOTE: this relies on the fact that the timer expiry and
 *              adc complete runs in the isr context, not the main loop
 *
 *************************************************************************/
WORD AdcRawConvertVsys( void )
{
    Config0 = ADC_RAW_CONFIG0_CHANNEL( CH_MUX ) | ADC_RAW_CONFIG0_BUSY;
    MuxAdcChannel = CH_MUXCHANNEL_VSYS;
    AdcRawConvert();

    // wait for adc conversion to complete
    while( Config0 & ADC_RAW_CONFIG0_BUSY ) {};

    return( RawResultAvg );
}

/**************************************************************************
 * Function:    AdcHalt( void )
 *
 * Descrption:  Hard stop for any current ADC operations to immediatly
 *              stop anything pre-supercap mode
 *
 *************************************************************************/
void AdcHalt( void )
{
    // Set mode to single-coversion mode so that setting ADC_ENC = 0 stops the current conversion immediatly
    WriteRegWordAnd( ADC12CTL1_REG16, ~ADC12CTL1_CONSEQ_MASK );

    // disable the conversion
    WriteRegWordAnd( ADC12CTL0_REG16, ~ADC12CTL0_ENC );

}


BYTE AdcRawRegRead( BYTE reg )
{

    switch( reg ) {
        case ADC_RAW_REG_CONFIG0:
            return( Config0 );

        case ADC_RAW_REG_MUXCHANNEL:
            return( MuxAdcChannel );

        case ADC_RAW_REG_RESULT_MSB:
            return( RawResultAvg>>8 );

        case ADC_RAW_REG_RESULT_LSB:
            return( RawResultAvg );

        default:
            return( 0 );
    }
}

void AdcRawRegWrite( BYTE reg, BYTE value )
{

    switch( reg ) {
        case ADC_RAW_REG_CONFIG0:
            Config0 = value;
            break;

        case ADC_RAW_REG_MUXCHANNEL:
            MuxAdcChannel = value;
            break;

        default:
            break;
    }
}

/*void AdcStartReceived( void )
{
    return;
}*/

void AdcRawStopReceived( void )
{

    // are we being requested to start and ADC engine
    if( Config0 & ADC_RAW_CONFIG0_START ) {
        // 'self clearing' START bit needs cleared
        Config0 &= ~ADC_RAW_CONFIG0_START;
        Config0 |= ADC_RAW_CONFIG0_BUSY;
        AdcRawConvert();
    }
}

