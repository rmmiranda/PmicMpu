/*****************************************************************************
 * Filename:    lagavulin_timer.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 08, 2010
 *
 * Description: Lagavulin Timer_B Module Routines
 ****************************************************************************/

#include "critical.h"
#include "registers.h"
#include "timer.h"
#include "gpio.h"
#include "intvector.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

// interrupt function handler for the mux timer
static FUNCTION muxTimerHandler = NULL;

// high 16 bits of the current hw timer count
static volatile WORD HighWord = 0;

// previous 32K counter value read
static WORD Last32kValue = 0x0;

// handles for interrupts triggered by the mux timer match event
void TimerMuxHandler( void )
{
    if( ReadRegByte(T32KCN_REG8) & T32KCN_T32KI ) {
    	// clear the overflow interrupt
    	WriteRegByteAnd( T32KCN_REG8, ~T32KCN_T32KI );
    	// call mux timer handler
        if( muxTimerHandler != NULL ) {
            muxTimerHandler();
        }
    	// increment high word counter if needed
    	if( ReadRegWord(T32KV_REG16) < Last32kValue ) {
        	HighWord++;
    	}
    	// store current 32k counter value
		Last32kValue = ReadRegWord( T32KV_REG16 );
    }
}

// retrieves the 16-bit resolution count of the hw timer
WORD GetHWTimerCountWord( void )
{
    WORD count;

    // must do a majority vote read while counter is active
    // to be sure we get the correct value (according to data sheet)
    do {
        count = ReadRegWord( T32KV_REG16 );
    } while( count != ReadRegWord( T32KV_REG16 ) );

    return( count );
}

// retrieves the 32-bit resolution count of the hw timer
DWORD GetHWTimerCountDword( void )
{
    WORD low;
    WORD high;
    WORD wason = CoreIntDisable();

    // make sure low and high word reads are in sync
    do {
    	// get high and low counter values
        high = HighWord;
        low = GetHWTimerCountWord();
    	// increment high word counter if needed
    	if( ReadRegWord(T32KV_REG16) < Last32kValue ) {
        	HighWord++;
    	}
    	// store current 32k counter value
		Last32kValue = ReadRegWord( T32KV_REG16 );
    } while( high != HighWord );

    CoreIntEnable( wason );
    
    return( ((DWORD)high<<16) | (DWORD)low );
}

// configures and restarts a particular timer
void SetHWTimer( TimerType timer, WORD ticksFromNow )
{
    WORD match;
    WORD wason = CoreIntDisable();

    if( ticksFromNow < 3 ) {
        ticksFromNow = 3;
    }

    switch( timer ) {
       case MUX_TIMER:
        	// disable the interrupt and clear any pending flag
        	WriteRegByteAnd( T32KCN_REG8, ~(T32KCN_T32KIE | T32KCN_T32KI) );
            // setup the new match value
            match = GetHWTimerCountWord() + ticksFromNow;
            WriteRegWord( T32KC_REG16, match );
        	// re-enable the timer interrupt
        	WriteRegByteOr( T32KCN_REG8, T32KCN_T32KIE );
        	V3( EventLogAddN( EVENTLOG_TIMER_HWMATCH, match ) );
            break;
        default:
            // TODO
            break;
    }

    CoreIntEnable( wason );
}

// stops a particular timer
void StopHWTimer( TimerType timer )
{
    WORD wason = CoreIntDisable();

    switch( timer ) {
        case MUX_TIMER:
        	// disable the interrupt and clear any pending flag
        	WriteRegByteAnd( T32KCN_REG8, ~(T32KCN_T32KIE | T32KCN_T32KI) );
            break;
        default:
            // TODO
            break;
    }

    CoreIntEnable( wason );
}

// initializes and enables the timer module
void TimerHWInit( void )
{
	// register timer interrupt handlers
    RegisterIntHandler( TIMER32K_VECTOR, TimerMuxHandler );

#if 0
    // source rtc from accurate 32K clock from analog chip
    WriteRegWord( RTCCFG_REG16, RTCCFG_OUTCLK );

    // wait until accurate 32K is actually driving rtc
    while( (ReadRegWord(RTCCFG_REG16) & RTCCFG_OUTCLKMD) == 0 )
    	; // do nothing
#else
	// initially source rtc from inaccurate 32K clock
    // generated by Timer B0; will later switch to the
    // accurate 32K clock from the analog chip
    WriteRegWordAnd( RTCCFG_REG16, ~RTCCFG_OUTCLK );

    // wait until inaccurate 32K is actually driving rtc
    while( ReadRegWord(RTCCFG_REG16) & RTCCFG_OUTCLKMD )
    	; // do nothing
#endif

    // start the 32K timer (mux) and keep it running even in stop mode
    WriteRegByte( T32KCN_REG8, T32KCN_T32KEN | T32KCN_T32KSTOP );
}

// disables the entire module (called when trying to enter deep sleep while running on the SuperCap
void TimerHWHalt( void )
{
	WriteRegByte( T32KCN_REG8, 0 );
}

// attaches interrupt handlers to specific timer interrupts
void RegisterHWTimerExpiry( TimerType timer, FUNCTION func )
{
    switch( timer ) {
        case MUX_TIMER:
            muxTimerHandler = func;
            break;
        default:
            // TODO
            break;
    }
}

