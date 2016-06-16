/*****************************************************************************
 * Filename:    i_basetype.h
 *
 * Copyright 2003, Research In Motion Ltd
 *
 * Author:      Jerry Mailloux
 *
 * Created:     Dec 08 2003
 *
 * Description: Declares basetypes that shouldn't be exposed to the apps
 ****************************************************************************/
#ifndef __I_BASETYPE_H__
#define __I_BASETYPE_H__

#include <basetype.h>

typedef volatile BYTE   IOBYTE;
typedef volatile WORD   IOWORD;
typedef volatile DWORD  IODWORD;
#if !defined( __MSP430__ ) && !defined( PROCESSOR_MQ31 )
typedef volatile QWORD  IOQWORD;
#endif

// Mask for enabling byte-reversal in bitmaps
#define BYTE_REVERSED_MASK          0x08

#define ROWWISE_16BIT_REVERSED_COLOUR_YUV422_BITMAP     0xAD /***< Rowwise, sixteen-bit colour YUV422 bitmap with YUYV in reversed order. */
#define ROWWISE_12BIT_COLOUR_YUV420_BITMAP              0xA4 /***< Rowwise, twelve-bit colour YUV420 bitmap in dense planar format. */
#endif

