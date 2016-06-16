/*                 - intrinsics.h -

  Intrinsic functions for the IAR MAXQ C compiler

  Copyright 2003-2006 IAR Systems. All rights reserved.

  $Revision: 1212 $

  $Log$
  Revision 1.5  2006/02/23 08:38:57  IPEH
  *** empty log message ***

  Revision 1.5  2006/02/23 08:38:57Z  IPEH
  Revision 1.4  2003/12/02 14:52:52  ifrc
  Revision 1.3  2003/11/26 13:45:53Z  ifrc
  Revision 1.1  2003/07/01 14:44:48Z  ifrc
  Initial revision

*/

#ifndef INTRINSICS_H
#define INTRINSICS_H

#ifdef  __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#pragma language=extended

typedef void (__interrupt * isr_type)(void);

typedef  unsigned char __istate_t;

__intrinsic void __disable_interrupt(void);
__intrinsic void __enable_interrupt(void);
__intrinsic void __reenable_interrupt(void);
__intrinsic __istate_t __get_interrupt_state(void);
__intrinsic void __set_interrupt_vector(isr_type);
__intrinsic void __set_interrupt_state(__istate_t);
__intrinsic void __no_operation(void);
__intrinsic void __require(void *);
__intrinsic unsigned int __swap_bytes(unsigned int);
__intrinsic void __clear_watchdog_timer(void);

__intrinsic double fabs(double);
__intrinsic float __dnormexp(float);
__intrinsic float __daddexp(float,signed char);
__intrinsic signed char __dgetexp(float);


#pragma language=default

#ifdef __cplusplus
}
#endif

#endif
