/*                      - STDIO.H -

   Subset of ANSI standard I/O function declarations.

   $Revision: 1.4 $

   Copyright 1986 - 1999 IAR Systems. All rights reserved.
*/

#ifndef _STDIO_INCLUDED
#define _STDIO_INCLUDED

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include "stdarg.h"

#ifndef NULL
#define NULL    ((void *) 0)
#endif

#ifndef EOF
#define EOF     (-1)
#endif

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

MEMORY_ATTRIBUTE int  puts(const char *);
MEMORY_ATTRIBUTE int  putchar(int);
MEMORY_ATTRIBUTE int  getchar(void);
MEMORY_ATTRIBUTE int  sprintf(char *,const char *,...);
MEMORY_ATTRIBUTE int  vsprintf(char *,const char *,va_list);
MEMORY_ATTRIBUTE int  printf(const char *,...);
MEMORY_ATTRIBUTE int  vprintf(const char *,va_list);
MEMORY_ATTRIBUTE int  scanf(const char *,...);
MEMORY_ATTRIBUTE int  sscanf(const char *, const char *,...);
MEMORY_ATTRIBUTE char *gets(char *);


#endif /* _STDIO_INCLUDED */
