/*****************************************************************************/
/* assert.h   v3.2.1                                                         */
/* Copyright (c) 1993-2009 Texas Instruments Incorporated                    */
/*****************************************************************************/

#ifndef _ASSERT
#define _ASSERT

#include <linkage.h>


#ifdef __cplusplus
//----------------------------------------------------------------------------
// <cassert> IS RECOMMENDED OVER <assert.h>.  <assert.h> IS PROVIDED FOR
// COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++
//----------------------------------------------------------------------------

#define _NAMESPACE_PREFIX std::

extern "C" namespace std
{
#else
#define _NAMESPACE_PREFIX
#endif

/* this #ifndef can go away when C2000 uses extern C builtins */
extern _CODE_ACCESS void _nassert(int);
extern _CODE_ACCESS void _assert(int, const char *);

#ifdef __TI_ELFABI__
extern _CODE_ACCESS void __c6xabi_abort_msg(const char *);
#else
extern _CODE_ACCESS void _abort_msg(const char *);
#endif

#define _STR(x)  __STR(x)
#define __STR(x) #x

#if defined(NDEBUG)
#define assert(_ignore) ((void)0)
#elif defined(NASSERT)
#define assert(_expr)   _NAMESPACE_PREFIX _nassert(_expr)
#else
     #define assert(_expr)   _NAMESPACE_PREFIX _assert((_expr) != 0,      \
                    "Assertion failed, (" _STR(_expr) "), file " __FILE__ \
                    ", line " _STR(__LINE__) "\n")
#endif /* NDEBUG, NASSERT */

#ifdef __cplusplus
} /* extern "C" namespace std */
#endif /* __cplusplus */

#endif /* _ASSERT */

#if defined(__cplusplus) && !defined(_CPP_STYLE_HEADER)
using std::_nassert;


#endif /* _CPP_STYLE_HEADER */

