/*****************************************************************************
 * Filename:    slowclock.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 1, 2011
 *
 * Description: Slow clock and reset related defines (copied from
 *              rocket_slowclock.h which was authored by Bryon Hummel).
 ****************************************************************************/
#ifndef __SLOWCLOCK_H__
#define __SLOWCLOCK_H__

WORD VCoreLevel( void );
WORD VCoreDecrease( void );
WORD VCoreIncrease( void );
void SetVCoreLevel( BYTE level, BOOL fullPerformance );

void ConfigureFll( void );
void ConfigureXt1External( WORD unused );

WORD HostPowerUpSequence( void );
void HostPowerDownSequence( void );

void PreEnterDeepSleep( void );
void SCSleep( void );
void SCSleepEnable(int module_);
void SCSleepDisable(int module_);

#endif  /* __SLOWCLOCK_H__ */

