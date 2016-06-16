/*****************************************************************************
 * Filename:    rocket_ft.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     March 2011
 *
 * Description: Rocket FT Definitions
 ****************************************************************************/

#ifndef _ROCKET_FT_H_
#define _ROCKET_FT_H_

#define FT_WATCHDOG_TIMEOUT   (90*32768)        // 90 Seconds
#define FT_END_KEY_POLL       (3276)            // 0.1 seconds

void FtWatchdogInit( void );
void FtWatchdogKick( void );
void FtWatchdogUninit( void );

#endif /*_ROCKET_FT_H_*/
