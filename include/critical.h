/*****************************************************************************
 * Filename:    critical.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     November 21, 2008
 *
 * Description: Critical Section functions
 ****************************************************************************/

#ifndef __CRITICAL_H__
#define __CRITICAL_H__

#include "basetype.h"

#if defined( PROCESSOR_MQ31 )
#include "maxq31_sfr.h"
#include "intrinsics.h"
#define CoreIntDisable() ReadRegByte( IC_REG8 ); __disable_interrupt()
#define CoreIntEnable(x) WriteRegByte( IC_REG8, x )
#else
#define CoreIntDisable() __get_interrupt_state(); __disable_interrupt()
#define CoreIntEnable(x) __set_interrupt_state(x)
#endif

#endif

