/*****************************************************************************
 * Filename:    registers.h
 *
 * Copyright 2004, Research In Motion Ltd
 *
 * Author:      Graeme Smecher
 *
 * Created:     Mar 4, 2004
 *
 * Description: This file contains helper functions for register accesses
 ****************************************************************************/

#ifndef __REGISTERS_H__
#define __REGISTERS_H__

#include "i_basetype.h"

#define WriteRegByte( REG, VALUE )      ((*((IOBYTE*)(REG)))=(VALUE))
#define WriteRegWord( REG, VALUE )      ((*((IOWORD*)(REG)))=(VALUE))
#define WriteRegDword( REG, VALUE )     ((*((IODWORD*)(REG)))=(VALUE))
#define WriteRegQword( REG, VALUE )     ((*((IOQWORD*)(REG)))=(VALUE))

#define WriteRegByteAnd( REG, VALUE )   ((*((IOBYTE*)(REG)))&=(VALUE))
#define WriteRegWordAnd( REG, VALUE )   ((*((IOWORD*)(REG)))&=(VALUE))
#define WriteRegDwordAnd( REG, VALUE )  ((*((IODWORD*)(REG)))&=(VALUE))

#define WriteRegByteOr( REG, VALUE )    ((*((IOBYTE*)(REG)))|=(VALUE))
#define WriteRegWordOr( REG, VALUE )    ((*((IOWORD*)(REG)))|=(VALUE))
#define WriteRegDwordOr( REG, VALUE )   ((*((IODWORD*)(REG)))|=(VALUE))

#define ReadRegByte( REG ) (*((IOBYTE*)(REG)))
#define ReadRegWord( REG ) (*((IOWORD*)(REG)))
#define ReadRegDword( REG ) (*((IODWORD*)(REG)))

#endif
