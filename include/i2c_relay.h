/************************************************************************************
 * Filename:    i2c_relay.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 7, 2011
 *
 * Description: Definitions of I2C Relay interface (renamed from rocket_i2c_relay.h
 * 				which was authored by Bryon Hummel and created on May 31, 2010).
 ***********************************************************************************/
#ifndef __I2C_RELAY_H__
#define __I2C_RELAY_H__

#if defined( PROCESSOR_PMU430 )
#include "rocket_registers.h"
#else
#include "lagavulin_registers.h"
#endif

// Supported Commands       Packed Description
// I2CRLY_WRITE:            [SlaveAddr][CMD][nDataBytes]
// I2CRLY_WRITEREAD:        [SlaveAddr][CMD][NumBytesToRead][nDataBytes]

typedef enum {
    STATE_IDLE      = I2CRLY_STATE_IDLE,
    STATE_BUSY      = I2CRLY_STATE_BUSY,
    STATE_SUCCESS   = I2CRLY_STATE_SUCCESS,
    STATE_FAILURE   = I2CRLY_STATE_FAILURE,
    STATE_CMDERR    = I2CRLY_STATE_CMDERR
}CommandState;

#define CMD_BUFFER_MAX_SIZE    64   // allow 64 bytes of actual data to be transfered/stored

#endif /* __I2C_RELAY_H__ */

