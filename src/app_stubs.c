/*****************************************************************************
 * Filename:    app_stubs.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     September 2, 1009
 *
 * Description: Rocket APP stubs for partial linking/detection of APP code
 *              used by BOOT code.
 ****************************************************************************/

#include "basetype.h"

BYTE ExtSwLastWrittenValue;

#if defined( RIM_KEYVOL_RESET )
void KeyVolumeResetEnable( BOOL enable )
{
    return;
}
BOOL IsKeyVolumeResetEnabled( void )
{
    return( FALSE );
}
#endif

void AppEntrypoint( void )
{
    return;
}

void AppExitpoint( void )
{
    return;
}

void AppSupercapPowersavingMode( void )
{
    return;
}

void AdcInit( void )
{
    return;
}

void AdcUninit( void )
{
    return;
}

void HostCtrlPaSlumpInit( void )
{
    return;
}

void HostCtrlPaSlumpUninit( void )
{
    return;
}

void UtcInit( void )
{
    return;
}

void UtcBackup( void )
{
    return;
}

void UtcDisable( void )
{
    return;
}

BOOL UtcIsDisabled( void )
{
    return( FALSE );
}

void UtcGetTime( BYTE *result )
{
    return;
}

void UtcSetTime( BYTE *data )
{
    return;
}
