/*****************************************************************************
 * Filename:    version.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Description: Rocket Version Information
 * ****************************************************************************/

#ifndef _VERSION_H_
#define _VERSION_H_

#define ROCKET_VERSION_MAJOR    1
#define ROCKET_VERSION_MINOR    0
#define ROCKET_VERSION        ( ( ROCKET_VERSION_MAJOR << 8 ) | \
                                ( ROCKET_VERSION_MINOR ) )

#define ROCKET_RELEASE          1       // 1==Developer build    0==SCM build
#define ROCKET_BUILD            0       // minor feature (used by SCM only)

#define ROCKET_BUILDINFO      ( ( ROCKET_RELEASE << 8 ) | \
                                ( ROCKET_BUILD ) )

#endif
