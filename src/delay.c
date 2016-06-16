/*****************************************************************************
 * Filename:    delay.c
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:		January 20, 2011
 *
 * Description: Hardware delay module.
 ****************************************************************************/

#include "basetype.h"
#include "timer_mux.h"

void HwDelay( DWORD ticksFromNow )
{
	DWORD expire = ReadRtcDword() + ticksFromNow;
	while( ReadRtcDword() < expire )
		; // do nothing

	return;
}
