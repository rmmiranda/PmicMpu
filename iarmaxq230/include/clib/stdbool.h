/*                      - STDBOOL.H -

   The "bool" type.

   $Revision: 1.3 $

   Copyright 2003 - 2004 IAR Systems. All rights reserved.
*/

#ifndef _STDBOOL
#define _STDBOOL

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#ifndef __C99_BOOL__
  #error <stdbool.h>  compiled with wrong (version of IAR) compiler
#endif

#ifndef __cplusplus

#define bool _Bool
#define true 1
#define false 0

#define __bool_true_false_are_defined 1

#endif /* !__cplusplus */

#endif /* !_STDBOOL */
