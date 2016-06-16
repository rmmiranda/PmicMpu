/****************************************************************************
 * Filename:    msgque.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     January 19, 2011
 *
 * Description: Message queue defines (copied from rocket_msgque.h
 * 				developed by Bryon Hummel).
 ****************************************************************************/
#ifndef __MSGQUE_H__
#define __MSGQUE_H__

#include <basetype.h>

#if defined( PROCESSOR_PMU430 )
#include "rocket_sfr.h"
#endif

// Maximum call queue size
#define CALLQUE_QSIZE        ( 16 )

typedef struct {
    FUNCTION1 call;
    WORD param;
} QueuedCall;


/**
 * Sets up message queue for use
 */
void CallQueueInit( void );


#define QueueCall( f, d )       QueueCallInt((f),(d),FALSE)
#define QueueCallPrio( f, d )   QueueCallInt((f),(d),TRUE)

/**
 * Queues a function to be called in the main loop
 * If prio is TRUE, the call is placed at the front of the queue, not the end
 */
void QueueCallInt( FUNCTION1 func, WORD data, BOOL prio );

/**
 * Removes MSP430 from Idle/Sleep mode on ISR return
 *
 * Macro must be called within an ISR which has posted a message
 * to allow the message to be serviced when the ISR returns.
 */
#if defined( PROCESSOR_PMU430 )
#define MessageQueued()  __bic_SR_register_on_exit( LPM4_SR_BITS )
#endif

/**
 * Executes the next call to be run, returns FALSE if
 * nothing left
 */
BOOL ExecuteCall( void );

/**
 * Retrieves the current message queue count
 *
 * @return Message queue count
 */
BYTE GetQueueCount( void );


#endif


