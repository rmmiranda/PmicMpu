/*****************************************************************************
 * Filename:    rocket_gpio_def.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     November 25, 2008
 *
 * Description: Rocket GPIO Pad Definitions
 ****************************************************************************/

#ifndef _ROCKET_GPIO_DEF_H_
#define _ROCKET_GPIO_DEF_H_

// Each Port Controls up to 8 Pads
// PAD   ((PortNumber-1)<<3) | PadIndex)
// ex. P5.1 works out as 0x21
#define PAD_PORT( P, N )    ( ((P)-1)<<3 | (N) )

#if defined( PROCESSOR_PMU430 )

#define PAD_PORT_1          PAD_PORT( 1, 0 )
#define PAD_PORT_1_0        PAD_PORT( 1, 0 )
#define PAD_PORT_1_1        PAD_PORT( 1, 1 )
#define PAD_PORT_1_2        PAD_PORT( 1, 2 )
#define PAD_PORT_1_3        PAD_PORT( 1, 3 )
#define PAD_PORT_1_4        PAD_PORT( 1, 4 )
#define PAD_PORT_1_5        PAD_PORT( 1, 5 )
#define PAD_PORT_1_6        PAD_PORT( 1, 6 )
#define PAD_PORT_1_7        PAD_PORT( 1, 7 )

#define PAD_PORT_2          PAD_PORT( 2, 0 )
#define PAD_PORT_2_0        PAD_PORT( 2, 0 )
#define PAD_PORT_2_1        PAD_PORT( 2, 1 )
#define PAD_PORT_2_2        PAD_PORT( 2, 2 )
#define PAD_PORT_2_3        PAD_PORT( 2, 3 )
#define PAD_PORT_2_4        PAD_PORT( 2, 4 )
#define PAD_PORT_2_5        PAD_PORT( 2, 5 )
#define PAD_PORT_2_6        PAD_PORT( 2, 6 )
#define PAD_PORT_2_7        PAD_PORT( 2, 7 )

#define PAD_PORT_3_0        PAD_PORT( 3, 0 )
#define PAD_PORT_3_1        PAD_PORT( 3, 1 )
#define PAD_PORT_3_2        PAD_PORT( 3, 2 )
#define PAD_PORT_3_3        PAD_PORT( 3, 3 )
#define PAD_PORT_3_4        PAD_PORT( 3, 4 )
#define PAD_PORT_3_5        PAD_PORT( 3, 5 )
#define PAD_PORT_3_6        PAD_PORT( 3, 6 )
#define PAD_PORT_3_7        PAD_PORT( 3, 7 )

#define PAD_PORT_4_0        PAD_PORT( 4, 0 )
#define PAD_PORT_4_1        PAD_PORT( 4, 1 )
#define PAD_PORT_4_2        PAD_PORT( 4, 2 )
#define PAD_PORT_4_3        PAD_PORT( 4, 3 )
#define PAD_PORT_4_4        PAD_PORT( 4, 4 )
#define PAD_PORT_4_5        PAD_PORT( 4, 5 )
#define PAD_PORT_4_6        PAD_PORT( 4, 6 )
#define PAD_PORT_4_7        PAD_PORT( 4, 7 )

#define PAD_PORT_5_0        PAD_PORT( 5, 0 )
#define PAD_PORT_5_1        PAD_PORT( 5, 1 )
#define PAD_PORT_5_2        PAD_PORT( 5, 2 )
#define PAD_PORT_5_3        PAD_PORT( 5, 3 )
#define PAD_PORT_5_4        PAD_PORT( 5, 4 )
#define PAD_PORT_5_5        PAD_PORT( 5, 5 )
#define PAD_PORT_5_6        PAD_PORT( 5, 6 )
#define PAD_PORT_5_7        PAD_PORT( 5, 7 )

#define PAD_PORT_J_0        PAD_PORT( 19, 0 )
#define PAD_PORT_J_1        PAD_PORT( 19, 1 )
#define PAD_PORT_J_2        PAD_PORT( 19, 2 )
#define PAD_PORT_J_3        PAD_PORT( 19, 3 )


// Port Mapping Mnemonics
#define PM_NONE             0
#define PM_ACLK             1
#define PM_TA0CLK           1
#define PM_SMCLK            2
#define PM_TA1CLK           2
#define PM_TA0CCR0A         3
#define PM_TA0CCR1A         4
#define PM_TA0CCR2A         5
#define PM_TA0CCR3A         6
#define PM_TA0CCR4A         7
#define PM_TA1CCR0A         8
#define PM_TA1CCR1A         9
#define PM_TA1CCR2A         10
#define PM_UCA0RXD          11
#define PM_UCA0SOMI         11
#define PM_UCA0TXD          12
#define PM_UCA0SIMO         12
#define PM_UCA0CLK          13
#define PM_UCB0STE          13
#define PM_UCB0SOMI         14
#define PM_UCB0SCL          14
#define PM_UCB0SIMO         15
#define PM_UCB0SDA          15
#define PM_UCB0CLK          16
#define PM_UCA0STE          16
#define PM_UCA1RXD          17
#define PM_UCA1SOMI         17
#define PM_UCA1TXD          18
#define PM_UCA1SIMO         18
#define PM_UCA1CLK          19
#define PM_UCB1STE          19
#define PM_UCB1SOMI         20
#define PM_UCB1SCL          20
#define PM_UCB1SIMO         21
#define PM_UCB1SDA          21
#define PM_UCB1CLK          22
#define PM_UCA1STE          22
#define PM_UCA2RXD          23
#define PM_UCA2SOMI         23
#define PM_UCA2TXD          24
#define PM_UCA2SIMO         24
#define PM_UCA2CLK          25
#define PM_UCB2STE          25
#define PM_UCB2SOMI         26
#define PM_UCB2SCL          26
#define PM_UCB2SIMO         27
#define PM_UCB2SDA          27
#define PM_UCB2CLK          28
#define PM_UCA2STE          28
#define PM_TEST0            29
#define PM_TEST1            30
#define PM_ANALOG           0xFF

#endif

#endif /*_ROCKET_GPIO_DEF_H_*/
