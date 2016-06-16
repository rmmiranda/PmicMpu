/*****************************************************************************
 * Filename:    hostctrl_pwri2c.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Feb, 2010
 *
 * Description: Definitions of PWR I2C interface
 ****************************************************************************/
#ifndef __HOSTCTRL_PWRI2C_H__
#define __HOSTCTRL_PWRI2C_H__

void HostCtrlPwrI2cInit( void );
void HostCtrlPwrI2cUninit( void );

BYTE PwrI2cRegRead( BYTE reg );
void PwrI2cRegWrite( BYTE reg, BYTE value );
//void PwrI2cStopReceived( void );
//void PwrI2cStartReceived( void );

#endif /* __HOSTCTRL_PWRI2C_H__ */

