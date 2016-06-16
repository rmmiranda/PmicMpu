/*                      - ASSERT.H -

   assert macro facility.

   NOTE: _Assert should report problems to 'stderr' for hosted
   implementations according to ANSI.
   May be implemented if the target supports it.

   $Revision: 1.5 $

   Copyright 1986 - 1999 IAR Systems. All rights reserved.
*/

#ifndef _ASSERT_INCLUDED
#define _ASSERT_INCLUDED

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#ifdef NDEBUG

#define assert(ignore)  ( (void) 0)

#else

void _Assert(const char *, const char *, int); /* forward declaration */

#define assert(arg)   ((arg) ? (void)0 : (void) _Assert ( \
                        #arg,__FILE__, __LINE__ ) )

#endif  /* NDEBUG */

#endif  /* _ASSERT_INCLUDED */

