/*****************************************************************************
 * Filename:    rocket_timer_mux.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     December 5, 2008
 *
 * Description: Rocket timer routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "timer.h"
#include "timer_mux.h"
#include "gpio.h"

#define VERBOSE_LEVEL 1
#include "eventlog.h"

static BOOL         IsSet       [ NUM_TIMERS ];
static DWORD        MatchValue  [ NUM_TIMERS ];
static FUNCTION1    TimerHandler[ NUM_TIMERS ];
static WORD         HandlerData [ NUM_TIMERS ];

static TimerId  pq[ NUM_TIMERS ]; // binary heap priority queue
static WORD     numSet = 0;

#if defined( PROCESSOR_PMU430 )
// Check if match value is between the first and second value in
// a circular way.
static __inline BOOL betweeneq( DWORD first, DWORD matchVal, DWORD second )
{
    // The following depends on the calculations being unsigned:
    return (matchVal - first) <= (second - first);
}
static __inline BOOL between( DWORD first, DWORD matchVal, DWORD second )
{
    // The following depends on the calculations being unsigned:
    return (matchVal - first) < (second - first);
}
#else
#if defined( PROCESSOR_MQ31 )
// Check if match value is between the first and second value in
// a circular way.
#pragma inline=forced
static BOOL betweeneq( DWORD first, DWORD matchVal, DWORD second )
{
    // The following depends on the calculations being unsigned:
    return (matchVal - first) <= (second - first);
}
#pragma inline=forced
static BOOL between( DWORD first, DWORD matchVal, DWORD second )
{
    // The following depends on the calculations being unsigned:
    return (matchVal - first) < (second - first);
}
#endif // PROCESSOR_MQ31
#endif // PROCESSOR_PMU430

static void setEarliest( DWORD ticksFromNow )
{
    if( ticksFromNow > 0xFFFF ) {
        // incase ticksFromNow = 0x10000 lets fire slightly earlier
        // so we have time to setup the timer since 1tick won't be enough
        SetHWTimer( MUX_TIMER, 0xFFF0 );
    } else {
        SetHWTimer( MUX_TIMER, (WORD)ticksFromNow );
    }
}

static void TimerCallbackFailure( WORD data )
{
    // TODO: some error handling
}

// remove pq[n], call with ints disabled
static void unSetTimer( WORD n, DWORD now )
{
    TimerId tmp;
    WORD c; // child

    IsSet[ pq[n] ] = FALSE;
    if( n == (numSet-1) ) {
        numSet--;
        return;
    }
    pq[n] = pq[--numSet];
    // no nodes left, all done
    // one node left, no need to heapify
    if( numSet >= 2 ) {
        // to handle the wrap case, assume all timers are shorter than 0x7FFFFFFF
        now += 0x80000000;
        // heapify
        c = (n+1)<<1;
        if( c > numSet ) return; // has no children, all done
        // if c==numSet, there is only one child, otherwise use the lesser child
        if( c == numSet || between( now, MatchValue[pq[c-1]], MatchValue[pq[c]] ) ) {
            c--;
        }
        while( between( now, MatchValue[pq[c]], MatchValue[pq[n]] ) ) {
            // node is greater than its lowest child, swap them
            tmp = pq[n];
            pq[n] = pq[c];
            pq[c] = tmp;
            n = c;
            // find the new lowest child
            c = (n+1)<<1;
            if( c > numSet ) return; // has no children, all done
            // if c==nTimers, there is only one child, otherwise use the lesser child
            if( c == numSet || between( now, MatchValue[pq[c-1]], MatchValue[pq[c]] ) ) {
                c--;
            }
        }
    }
}

static void expireMuxTimers( void )
{
    TimerId e;
    DWORD now = GetHWTimerCountDword();

    V2( EventLogAdd( EVENTLOG_TIMER_TRY_EXPIRE ) );
    while( numSet && betweeneq( now + 0x80000000, MatchValue[pq[0]], now + 3 ) ) {
        V1( EventLogAddN( EVENTLOG_TIMER_EXPIRY, pq[0] ) );
        V2( EventLogAddN( EVENTLOG_TIMER_NOW, now >> 16 ) );
        V2( EventLogAddN( EVENTLOG_TIMER_NOW, now ) );
        e = pq[0];
        unSetTimer( 0, now );
        (TimerHandler[ e ])( HandlerData[ e ] );
        now = GetHWTimerCountDword();
    }
    V2( EventLogAddN( EVENTLOG_TIMER_NUM, numSet ) );
    if( numSet ) {
        V3( for( e = (TimerId)0; (WORD)e < numSet; e++ ) {
            EventLogAddN( EVENTLOG_TIMER_DUMP, pq[e] );
            EventLogAddN( EVENTLOG_TIMER_SCHEDULED, MatchValue[pq[e]] >> 16 );
            EventLogAddN( EVENTLOG_TIMER_SCHEDULED, MatchValue[pq[e]] );
        } )
        setEarliest( MatchValue[pq[0]] - now );
    } else if( !numSet ) {
        StopHWTimer( MUX_TIMER );
    }
}

void SetTimer( TimerId timer, DWORD ticksFromNow )
{
    if( timer < NUM_TIMERS ) {
        DWORD now;
        TimerId tmp;
        WORD n, // node
             p; // parent
        WORD wason = CoreIntDisable();

        V1( EventLogAddN( EVENTLOG_TIMER_SET, timer ) );

        // let's not screw things up
        if( IsSet[timer] ) {
            StopTimer( timer );
        }
        now = GetHWTimerCountDword();

        V2( EventLogAddN( EVENTLOG_TIMER_NOW, now >> 16 ) );
        V2( EventLogAddN( EVENTLOG_TIMER_NOW, now ) );

        // enforce a minimum period
        if( ticksFromNow < 5 ) {
            ticksFromNow = 5;
        }

        n = numSet++;
        pq[n] = timer;
        MatchValue[timer] = now + ticksFromNow;
        IsSet[timer] = TRUE;
        // if only one node (n=0), all done
        if( n ) {
            // heapify
            p = (n-1)>>1; // parent of the new node

            // to handle the wrap case, assume all timers are shorter than 0x7FFFFFFF
            while( between( now + 0x80000000, MatchValue[pq[n]], MatchValue[pq[p]] ) ) {
                // node is less than its parent, swap them
                tmp = pq[n];
                pq[n] = pq[p];
                pq[p] = tmp;
                if( !p ) break; // stop if we're at the root
                n = p;
                // find the parent of the new node position
                p = (n-1)>>1;
            }
        }
        V3( EventLogAddN( EVENTLOG_TIMER_NUM, numSet );
        for( n = 0; n < numSet; n++ ) {
            EventLogAddN( EVENTLOG_TIMER_DUMP, pq[n] );
            EventLogAddN( EVENTLOG_TIMER_SCHEDULED, MatchValue[pq[n]] >> 16 );
            EventLogAddN( EVENTLOG_TIMER_SCHEDULED, MatchValue[pq[n]] );
        } )
        expireMuxTimers();
        CoreIntEnable( wason );
    }
}

void StopTimer( TimerId timer )
{
    if( timer < NUM_TIMERS ) {
        DWORD now;
        WORD i;
        WORD wason = CoreIntDisable();

        if( IsSet[ timer ] ) {
            // scan for the timer in the pq
            for( i = 0; timer != pq[i] && i < numSet; i++ );
            V1( EventLogAddN( EVENTLOG_TIMER_STOP, (i << 8) | timer ) );
            now = GetHWTimerCountDword();
            unSetTimer( i, now );
            V2( EventLogAddN( EVENTLOG_TIMER_NOW, now >> 16 ) );
            V2( EventLogAddN( EVENTLOG_TIMER_NOW, now ) );
            expireMuxTimers();
            if( numSet ) {
                V3( EventLogAddN( EVENTLOG_TIMER_NUM, numSet );
                for( i = 0; i < numSet; i++ ) {
                    EventLogAddN( EVENTLOG_TIMER_DUMP, pq[i] );
                    EventLogAddN( EVENTLOG_TIMER_SCHEDULED, MatchValue[pq[i]] >> 16 );
                    EventLogAddN( EVENTLOG_TIMER_SCHEDULED, MatchValue[pq[i]] );
                } )
                setEarliest( MatchValue[pq[0]] - now );
            } else {
                StopHWTimer( MUX_TIMER );
            }
        }
        CoreIntEnable( wason );
    }
}

void RegisterTimerExpiry( TimerId timer, FUNCTION1 func, WORD data )
{
    // register the new handler function. If no function was passed in
    // set it to the failure callback
    if( timer < NUM_TIMERS ) {
        TimerHandler[ timer ] = func ? func : TimerCallbackFailure;
        HandlerData[ timer ] = data;
    }
}

void TimerInit( void )
{
    WORD i;
    TimerHWInit();

    numSet = 0;

    for( i = 0 ; i < NUM_TIMERS ; i++ ) {
        IsSet[ i ] = FALSE;
        TimerHandler[ i ] = TimerCallbackFailure;
    }

    RegisterHWTimerExpiry( MUX_TIMER, expireMuxTimers );
}
