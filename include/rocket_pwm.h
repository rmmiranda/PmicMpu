/*****************************************************************************
 * Filename:    rocket_pwm.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     August 17, 2010
 *
 * Description: MSP430 PWM Control
 ****************************************************************************/
#ifndef __ROCKET_PWM_H__
#define __ROCKET_PWM_H__

void PwmInit( void );
void PwmUninit( void );

void PwmConfig( WORD dc );
void PwmEnable( WORD dc, WORD period );
BOOL PwmIsActive( void );

#endif /* __ROCKET_PWM_H__ */
