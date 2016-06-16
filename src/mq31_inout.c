/*****************************************************************************
 * Filename:    maxq31_inout.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 21, 2010
 *
 * Description: MAXQ31 interrupt handlers
 ****************************************************************************/

#include "basetype.h"

extern FUNCTION InterruptHandlerActual[];

#pragma vector=0
__interrupt void maxq31isr0(void) { InterruptHandlerActual[0](); }

#pragma vector=1
__interrupt void maxq31isr1(void) { InterruptHandlerActual[1](); }

#pragma vector=2
__interrupt void maxq31isr2(void) { InterruptHandlerActual[2](); }

#pragma vector=3
__interrupt void maxq31isr3(void) { InterruptHandlerActual[3](); }

#pragma vector=4
__interrupt void maxq31isr4(void) { InterruptHandlerActual[4](); }

#pragma vector=5
__interrupt void maxq31isr5(void) { InterruptHandlerActual[5](); }

#pragma vector=6
__interrupt void maxq31isr6(void) { InterruptHandlerActual[6](); }

#pragma vector=7
__interrupt void maxq31isr7(void) { InterruptHandlerActual[7](); }

#pragma vector=8
__interrupt void maxq31isr8(void) { InterruptHandlerActual[8](); }

#pragma vector=9
__interrupt void maxq31isr9(void) { InterruptHandlerActual[9](); }

#pragma vector=10
__interrupt void maxq31isr10(void) { InterruptHandlerActual[10](); }

#pragma vector=11
__interrupt void maxq31isr11(void) { InterruptHandlerActual[11](); }

#pragma vector=12
__interrupt void maxq31isr12(void) { InterruptHandlerActual[12](); }

#pragma vector=13
__interrupt void maxq31isr13(void) { InterruptHandlerActual[13](); }

#pragma vector=14
__interrupt void maxq31isr14(void) { InterruptHandlerActual[14](); }

#pragma vector=15
__interrupt void maxq31isr15(void) { InterruptHandlerActual[15](); }
