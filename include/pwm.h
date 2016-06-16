/*****************************************************************************
 * Filename:    pwm.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 7, 2011
 *
 * Description: Pmic PWM Control (renamed from rocket_pwm.h which was authored
 * 				by Bryon Hummel and created on August 17, 2010).
 ****************************************************************************/
#ifndef __PWM_H__
#define __PWM_H__

void PwmInit( void );
void PwmUninit( void );

void PwmConfig( WORD dc );
void PwmEnable( WORD dc, WORD period );
BOOL PwmIsActive( void );

#endif /* __PWM_H__ */
