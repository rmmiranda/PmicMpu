/*                      - STDARG.H -
   The ANSI macros for reading variable argument lists (...).

   $Name: V1_13E V1_13D $    

   Copyright 1986 - 1999 IAR Systems. All rights reserved.
*/

#ifndef _STDARG_H_
#define _STDARG_H_

#include "sysmac.h"


typedef void *va_list[1];

__INTRINSIC void __va_start(va_list);

#define va_start(ap, parmN) __va_start(ap)
#define va_end(ap)            ((void) 0)


#if __CORE__==10 /* MAXQ10 */
#define va_arg(ap,type) (*(type*)((*(int **)ap)++))
#else
#if __CORE__==20 || __CORE__==68 /* MAXQ20/68 */
#define va_arg(ap,type) ((sizeof(type)==1)?(*(ap) = ((char*)*(ap)+2), *((type*)*(ap) - 2)):(*(ap) = ((char*)*(ap)+sizeof(type)), *((type*)*(ap) - 1)))
#else
#error Unrecognized __CORE__ 
#endif
#endif


#endif /* _STDARG_H_ */

