/*****************************************************************************
 * Filename:    rocket_msgque.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     April 3, 2009
 *
 * Description: Rocket message queue routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "eventlog.h"
#include "msgque.h"
#include "watchdog.h"
#include "gpio.h"

static QueuedCall qCalls[ CALLQUE_QSIZE ];
static BYTE    qHead;
static BYTE    qTail;
static BYTE    qCount;
FUNCTION1 LastMsgQCall;

void CallQueueInit( void )
{
    qHead = 0;
    qTail = 0;
    qCount = 0;
}

void QueueCallInt( FUNCTION1 func, WORD data, BOOL prio )
{
    QueuedCall *qc;
    WORD wason = CoreIntDisable();

    if( qCount >= CALLQUE_QSIZE ) {
        // TODO: some error handling, catfail?
        EventLogAddN( EVENTLOG_MSGQUE_FULL, (WORD)LastMsgQCall );
        RimInitiateReset( RESET_COOKIE_INITIATED_ERROR );
    }

    if( prio ) {
        if( qTail == 0 ) {
            qTail = CALLQUE_QSIZE;
        }
        qc = &qCalls[ --qTail ];
    } else {
        qc = &qCalls[ qHead++ ];
        if( qHead == CALLQUE_QSIZE ) {
            qHead = 0;
        }
    }
    qc->call = func;
    qc->param = data;
    qCount++;
    EventLogAdd( EVENTLOG_MSGQUE_POST );

    CoreIntEnable( wason );
}

BOOL ExecuteCall( void )
{
    FUNCTION1 func;
    WORD data;
    WORD wason;

    if( qCount == 0 ) {
        return( FALSE );
    }

    wason = CoreIntDisable();

    func = qCalls[ qTail ].call;
    data = qCalls[ qTail ].param;
    qCount--;

    qTail++;
    if( qTail == CALLQUE_QSIZE ) {
        qTail = 0;
    }
    CoreIntEnable( wason );

    LastMsgQCall = func;
    EventLogAdd( EVENTLOG_MSGQUE_GET );
    func( data );
    LastMsgQCall = NULL;

    return( TRUE );
}

BYTE GetQueueCount( void )
{
    return( qCount );
}
