/*****************************************************************************
 * Filename:    lagavulin_gpio_def.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 07, 2010
 *
 * Description: Lagavulin GPIO Pad Definitions
 ****************************************************************************/

#ifndef _LAGAVULIN_GPIO_DEF_H_
#define _LAGAVULIN_GPIO_DEF_H_

// Each Port Controls up to 8 Pads
// PAD   (PortNumber<<3) | PadIndex)
// ex. P1.5 works out as 0x0D

#define PAD_PORT( P, N )    ( (P)<<3 | (N) )
#define PAD_PORT_TOTAL		5
#define PAD_INDEX_TOTAL		8

// MQ31 pads (16)

#define PAD_PORT_0          PAD_PORT( 0, 0 ) // P0.0
#define PAD_PORT_0_0        PAD_PORT( 0, 0 ) // P0.0
#define PAD_PORT_0_1        PAD_PORT( 0, 1 ) // P0.1
#define PAD_PORT_0_2        PAD_PORT( 0, 2 ) // P0.2
#define PAD_PORT_0_3        PAD_PORT( 0, 3 ) // P0.3
#define PAD_PORT_0_4        PAD_PORT( 0, 4 ) // P0.4
#define PAD_PORT_0_5        PAD_PORT( 0, 5 ) // P0.5
#define PAD_PORT_0_6        PAD_PORT( 0, 6 ) // P0.6
#define PAD_PORT_0_7        PAD_PORT( 0, 7 ) // P0.7

#define PAD_PORT_1          PAD_PORT( 1, 0 ) // P1.0
#define PAD_PORT_1_0        PAD_PORT( 1, 0 ) // P1.0
#define PAD_PORT_1_1        PAD_PORT( 1, 1 ) // P1.1
#define PAD_PORT_1_2        PAD_PORT( 1, 2 ) // P1.2
#define PAD_PORT_1_3        PAD_PORT( 1, 3 ) // P1.3
#define PAD_PORT_1_4        PAD_PORT( 1, 4 ) // P1.4
#define PAD_PORT_1_5        PAD_PORT( 1, 5 ) // P1.5
#define PAD_PORT_1_6        PAD_PORT( 1, 6 ) // P1.6
#define PAD_PORT_1_7        PAD_PORT( 1, 7 ) // P1.7

// PR66 pads (12)

#define PAD_PORT_3          PAD_PORT( 3, 0 ) // GPIO0
#define PAD_PORT_3_0        PAD_PORT( 3, 0 ) // GPIO0
#define PAD_PORT_3_1        PAD_PORT( 3, 1 ) // GPIO1
#define PAD_PORT_3_2        PAD_PORT( 3, 2 ) // GPIO2
#define PAD_PORT_3_3        PAD_PORT( 3, 3 ) // GPIO3
#define PAD_PORT_3_4        PAD_PORT( 3, 4 ) // GPIO4
#define PAD_PORT_3_5        PAD_PORT( 3, 5 ) // GPIO5
#define PAD_PORT_3_6        PAD_PORT( 3, 6 ) // GPIO6
#define PAD_PORT_3_7        PAD_PORT( 3, 7 ) // GPIO7

#define PAD_PORT_4          PAD_PORT( 4, 0 ) // GPIO8
#define PAD_PORT_4_0        PAD_PORT( 4, 0 ) // GPIO8
#define PAD_PORT_4_1        PAD_PORT( 4, 1 ) // GPIO9
#define PAD_PORT_4_2        PAD_PORT( 4, 2 ) // GPIO10
#define PAD_PORT_4_3        PAD_PORT( 4, 3 ) // GPIO11

#endif /*_LAGAVULIN_GPIO_DEF_H_*/
