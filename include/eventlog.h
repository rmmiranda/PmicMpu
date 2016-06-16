/****************************************************************************
 * Filename:    eventlog.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 8, 2011
 *
 * Description: Eventlog defines (renamed from rocket_eventlog.h which was
 * 				authored by Bryon Hummel and created on April 1, 2009).
 ****************************************************************************/
#ifndef __EVENTLOG_H__
#define __EVENTLOG_H__

#include <basetype.h>
#include "eventlog_events.h"

// must be power of 2 and currently index size is only a BYTE
// Note: 2 buffers are allocated; total memory used is:
//   4*EVENTLOG_MAX_ENTRIES Bytes (aka 512 Bytes) + some log overhead
//   variables/indexes
//   The "CurrentEventLog" is within preserved memory so the bootloader doesn't
//   corrupt it on a reset before we can retrieve it.
#define EVENTLOG_MAX_ENTRIES    ( 128 )

// flags a log as valid/existing
#define EVENTLOG_COOKIE         ( 0x1357 )

/**
 *  Compile Time Eventlog Verbosity Defines
 *  Use:
 *      V0( ) - Any critical eventlog that will be visible all the time
 *      V1( ) - Any helpful event for general system debugging
 *      V2( ) - Any module specific debugging events (should only really enable VERBOSE_LEVEL 2 for a few
 *              modules at any given time else it will spam the log and fill it too quickly to be useful
 *      V3( ) - Any module specific debugging which will cause extreme levels of events ( should only
 *              enable VERBOSE_LEVEL 3 for 1 module at a time while debugging)
 *      V4( ) - Unused
 */
#ifdef VERBOSE_LEVEL

#if VERBOSE_LEVEL >= 0    // this is the default level
  #define   V0( x )     x
#else
  #define   V0( x )
#endif

#if VERBOSE_LEVEL >= 1
  #define   V1( x )     x
#else
  #define   V1( x )
#endif

#if VERBOSE_LEVEL >= 2
  #define   V2( x )     x
#else
  #define   V2( x )
#endif

#if VERBOSE_LEVEL >= 3
  #define   V3( x )     x
#else
  #define   V3( x )
#endif

#if VERBOSE_LEVEL >= 4
  #define   V4( x )     x
#else
  #define   V4( x )
#endif

#endif // VERBOSE_LEVEL

typedef union {
    BYTE byte_event[ EVENTLOG_MAX_ENTRIES<<1 ];
    WORD event[ EVENTLOG_MAX_ENTRIES ];
} Entry;

typedef struct {
    BYTE head;
    BYTE tail;
    WORD numEntries;
    Entry log;
    WORD cookie;
} EventLog;

/**
 * Adds an event to the current eventlog
 *
 * @param event Event ID for the event being added
 */
void EventLogAdd( WORD event );

/**
 * Adds an event with data to the current eventlog
 *
 * @param event Event ID for the event being added
 * @param data  Data to be paired with event
 */
void EventLogAddN( WORD event, WORD data );

/**
 * Init the current eventlog for use, if a previous log exists in
 * memory, it is backed up to be read later
 */
void EventLogInit( void );

/**
 * Takes the current eventlog and captures it into the saved log
 * buffer to be read later by the host
 */
void EventLogCapture( void );

/**
 * Sequential calls to this function returns the contents of the
 * saved eventlog one byte at a time.
 *
 * The host must make sequential calls to this to reconstruct the
 * saved eventlog for its display. All events are of size WORD so it
 * takes 2 sequential calls to capture one event from the saved log.
 *
 * This function internally tracks/increments through the saved log
 * and will return 0x00 signalling the end of the log.
 *
 * @return Alternating Event High/Low Bytes and 0x00 on end of log
 */
BYTE SavedEventLogRead( void );

/**
 * Resets the internal indexes/pointers of saved log to the oldest
 * event in the saved eventlog.
 *
 * Should be called after capturing a log before trying sequential
 * reads via SavedEventLogRead()
 */
void SavedEventLogRewind( void );

/**
 * Indicates if a saved log is available for reading
 *
 * @return TRUE if log is present, otherwise FALSE
 */
BOOL SavedEventLogExists( void );

#endif


