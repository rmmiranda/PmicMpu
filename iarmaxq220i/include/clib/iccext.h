/*                     - ICCEXT.H -

   Some extra declarations for non-ANSI functions used by the C library.

   $Revision: 1.4 $

   Copyright 1986 - 1999 IAR Systems. All rights reserved.
*/


#ifndef _ICCEXT_INCLUDED
#define _ICCEXT_INCLUDED

#include "sysmac.h"

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#if __IAR_SYSTEMS_ICC__ < 2
#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif
#endif

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

__INTRINSIC MEMORY_ATTRIBUTE double exp10(double);

#if __IAR_SYSTEMS_ICC__ < 2
#if __TID__ & 0x8000
#pragma function=default
#endif
#endif

#endif

