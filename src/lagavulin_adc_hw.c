/*****************************************************************************
 * Filename:    lagavulin_adc_hw.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     Feb 18, 2011
 *
 * Description: Lagavulin HW ADC Routines
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "delay.h"
#include "registers.h"
#include "api.h"
#include "spi.h"
#include "intvector.h"
#include "lagavulin_adc_hw.h"
#include "timer_mux.h"
#include "lagavulin_pmic.h"
#include "lagavulin_registers.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

FUNCTION1 ConvCompleteCallback = NULL;
WORD SampleResult;

// RAW ADC register interface
#define RAW_SAMPLE_INTERVAL_TICKS     ( 16 )        // 0.5ms interval
static volatile BYTE Config0;
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
    // enable ADC reference voltage
    PmicWrite( PMIC_ADCREF, ADCREF_BGEN | ADCREF_REFEN ); // implies 2.5V
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

	//clear the interrupt
    WriteRegWordAnd( ADST_REG16, ~ADST_ADDAI );

    // diasble the 'sample complete' interrupt
    WriteRegWordAnd( ADCN_REG16, ~(ADCN_ADDAIE | ADCN_ADDAINV) );

    // turn adc conversion off to reduce power consumption
    WriteRegWord( ADSEL0_REG16, 0x0000 );
    WriteRegByte( ADSEL1_REG8, 0x00 );

    // grab the result from the register
	WriteRegByte( ADCH_REG8, (AdcChannel)(Config0>>2) );
    SampleResult = ReadRegWord( ADDATA_REG16 );

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
    RawResultAvg = 0;

    // configure the internal reference generator
    InitRefGen();

    RegisterIntHandler( ADC_VECTOR, AdcCompleteIsrHw );

    // configure ADC: single-channel, single-conversion, 12-bit res
    WriteRegWord( ADCN_REG16, 0x0000 );
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
void AdcConfigISource( AdcChannel ch, BOOL en )
{
#if 0
    if( en ) {
        PmicWrite( PMIC_REG_ADCIN_SEL, chMux );
        WriteRegByte( ADCP3CE_REG8, ADCP3CE_EN(ch)  );
    } else {
        WriteRegByte( ADCP3CE_REG8, 0 );
        PmicWrite( PMIC_REG_ADCIN_SEL, 0 );
    }
#endif
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
void AdcConvertHw( AdcChannel ch, AdcChannelRef cr, FUNCTION1 ccCallback )
{
    WORD wason = CoreIntDisable();

	if( ch == CH_VBBATT ) {

		// main battery is monitored by the PR66 Fuel Gauge ADC
		SampleResult = PmicRead( PMIC_FGADC_VOLT0 ) | (PmicRead( PMIC_FGADC_VOLT1 ) << 8);

		// if we have a callback, call it now
		if( ccCallback ) {
			ccCallback( SampleResult );
		}

	} else {
        BYTE adcref = PmicRead( PMIC_ADCREF );
        Config0 = ( Config0 & ~ADC_CONFIG0_CHANNEL_MASK ) |
                  ( ADC_CONFIG0_CHANNEL(ch) & ADC_CONFIG0_CHANNEL_MASK );
                  
        PmicWrite( PMIC_ADCREF, adcref | ADCREF_SW );
        
		// disable adc
		WriteRegWordAnd( ADCN_REG16, ~ADCN_ADEN );

		// select ADC channel
		if( ch > CH_ADC6) {
			WriteRegByte( ADSEL1_REG8, (1u << (ch-CH_ADC7)) );
		} else {
			WriteRegWord( ADSEL0_REG16, (1u << ch) );
		}

		// 80uS T-settle required when switching mux channel
		HW_DELAY_USEC( 80 );

		// clear any pending interrupts
		WriteRegWordAnd( ADST_REG16, ~(ADST_ADDAI | ADST_ADTRGI) );

		// if we have a callback
		if( ccCallback ) {
			// enable the 'sample complete' interrupt
			WriteRegWord( ADCN_REG16, ADCN_ADDAIE | ADCN_ADDAINV );

			ConvCompleteCallback = ccCallback;
		}

		// enable ADC again
		WriteRegWordOr( ADCN_REG16, ADCN_ADEN );

		// Trigger conversion to start
		WriteRegWordOr( ADST_REG16, ADST_ADCONV );

		V2( EventLogAdd( EVENTLOG_ADC_SAMPLE_STARTED ) );

		// if we didn't have a callback, lets poll until the conversion is complete
		if( ccCallback == NULL ) {
			V2( EventLogAdd( EVENTLOG_ADC_POLLING ) );
			while( ReadRegWord( ADST_REG16 ) & ADST_ADCONV ) {};

			WriteRegByte( ADCH_REG8, ch );
			SampleResult = ReadRegWord( ADDATA_REG16 );

			// turn adc conversion off to reduce power consumption
			WriteRegWord( ADSEL0_REG16, 0x0000 );
			WriteRegByte( ADSEL1_REG8, 0x00 );

			V2( EventLogAddN( EVENTLOG_ADC_SAMPLE_COMPLETE, SampleResult ) );
		}
        PmicWrite( PMIC_ADCREF, adcref );
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
        if( RawNumSamples >= 32 ) {
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
        AdcConvertHw( (AdcChannel)(Config0>>2), CHREF_AVCC_AVSS, AdcRawCb );
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
    AdcConvertHw( (AdcChannel)(Config0>>2), CHREF_AVCC_AVSS, AdcRawCb );
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
    Config0 = ADC_RAW_CONFIG0_CHANNEL( CH_VSYS ) | ADC_RAW_CONFIG0_BUSY;
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
    // stop the conversion
    WriteRegWordAnd( ADST_REG16, ~ADST_ADCONV );

    // disable adc
    WriteRegWordAnd( ADCN_REG16, ~ADCN_ADEN );
}


BYTE AdcRawRegRead( BYTE reg )
{

    switch( reg ) {
        case ADC_RAW_REG_CONFIG0:
            return( Config0 );

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

