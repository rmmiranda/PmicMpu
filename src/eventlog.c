/*****************************************************************************
 * Filename:    rocket_eventlog.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     April 1, 2009
 *
 * Description: Rocket eventlog routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "eventlog.h"
#include "string.h"
#include "timer_mux.h"

// TI CCE Compiler doesn't zero globals which are not inited explicitly ( slau156h.pdf C.5.1 )
// Using that fact to maintain & detect log accross a reset if power is not lost
// Place CurrentEventLog in an area of ram the bootloader won't corrupt so its
// retrievable after an initiated reset where we don't lose power
#if defined( PROCESSOR_PMU430 )
 #pragma DATA_SECTION( CurrentEventLog, ".preserved" )
 EventLog CurrentEventLog;
#elif defined( PROCESSOR_MQ31 )
 __no_init EventLog CurrentEventLog;
#else
 EventLog CurrentEventLog;
#endif

EventLog SavedEventLog;
WORD SavedEventLogByteIndex;
WORD SavedEventLogSize;
WORD SavedEventLogReadCount;

void EventLogInit( void )
{
#if 0

    SavedEventLogByteIndex = 0;
    SavedEventLogSize = 0;
    SavedEventLogReadCount = 0;

    // save pre-reset log if it exists
    if( CurrentEventLog.cookie == EVENTLOG_COOKIE ) {
        memcpy( &SavedEventLog, &CurrentEventLog, sizeof( EventLog ) );
        // set it up to be read by the host
        SavedEventLogRewind();
    }

    CurrentEventLog.head = 0;
    CurrentEventLog.tail = 0;
    CurrentEventLog.numEntries = 0;
    memset( CurrentEventLog.log.event, 0, sizeof( CurrentEventLog.log.event ) );
    CurrentEventLog.cookie = EVENTLOG_COOKIE;

    EventLogAdd( EVENTLOG_INIT );
#endif
}

void EventLogCapture( void )
{
    WORD wason = CoreIntDisable();
#if 0

    // save pre-reset log if it exists
    if( CurrentEventLog.cookie == EVENTLOG_COOKIE ) {
        memcpy( &SavedEventLog, &CurrentEventLog, sizeof( EventLog ) );
        SavedEventLogRewind();
    }
#endif
    CoreIntEnable( wason );
}

void EventLogAdd( WORD event )
{
    WORD wason = CoreIntDisable();

#if 0
    if( CurrentEventLog.cookie == EVENTLOG_COOKIE ) {
        // if we wrapped and are stomping on a EVENTLOG_PRINTN flag, we don't want
        // to send garbage to the host in the log (aka try to decode data as an event)
        // so if we are clobbering EVENTLOG_PRINTN make sure the next 2 entries
        // get wiped too
        if( CurrentEventLog.log.event[ CurrentEventLog.head ] == EVENTLOG_PRINTN ) {
            CurrentEventLog.tail = (CurrentEventLog.tail + 1) & (EVENTLOG_MAX_ENTRIES-1);
            CurrentEventLog.log.event[ CurrentEventLog.tail ] = EVENTLOG_NOEVENT;
            CurrentEventLog.tail = (CurrentEventLog.tail + 1) & (EVENTLOG_MAX_ENTRIES-1);
            CurrentEventLog.log.event[ CurrentEventLog.tail ] = EVENTLOG_NOEVENT;
            CurrentEventLog.tail = (CurrentEventLog.tail + 1) & (EVENTLOG_MAX_ENTRIES-1);
            CurrentEventLog.numEntries -= 2;
        } else if ( CurrentEventLog.log.event[ CurrentEventLog.head ] == EVENTLOG_NOEVENT ) {
            CurrentEventLog.numEntries++;
        } else {
            // we are wrapping by 1 entry, update tail
            CurrentEventLog.tail = (CurrentEventLog.tail + 1) & (EVENTLOG_MAX_ENTRIES-1);
        }

        // insert new event
        CurrentEventLog.log.event[ CurrentEventLog.head ] = event;
        CurrentEventLog.head = (CurrentEventLog.head + 1) & (EVENTLOG_MAX_ENTRIES-1);
    }
#endif

    CoreIntEnable( wason );
}

void EventLogAddN( WORD event, WORD data )
{
    WORD wason = CoreIntDisable();

#if 0
    // add the PRINTN signal, EVENT and DATA to the eventlog ring buffer
    EventLogAdd( EVENTLOG_PRINTN );
    EventLogAdd( event );
    EventLogAdd( data );
#endif

    CoreIntEnable( wason );
}

void SavedEventLogRewind( void )
{
#if 0

    SavedEventLogReadCount = 0;

    // if we don't have a valid log saved, don't change our indexes/counters
    if( SavedEventLog.cookie != EVENTLOG_COOKIE ) {
        return;
    }

    SavedEventLogByteIndex = (WORD)SavedEventLog.tail<<1;
    SavedEventLogSize = (WORD)SavedEventLog.numEntries<<1;
#endif
}

BYTE SavedEventLogRead( void )
{
    BYTE ret = 0;
#if 0

    if( SavedEventLogReadCount < SavedEventLogSize ) {
        ret = SavedEventLog.log.byte_event[ SavedEventLogByteIndex ];
        SavedEventLogByteIndex = (SavedEventLogByteIndex + 1) & ((EVENTLOG_MAX_ENTRIES<<1)-1);
        SavedEventLogReadCount++;
    } else {
        ret = EVENTLOG_NOEVENT;
    }
#endif
    return ret;
}

BOOL SavedEventLogExists( void )
{
    return( SavedEventLog.cookie == EVENTLOG_COOKIE );
}

