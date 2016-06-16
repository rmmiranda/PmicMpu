/*****************************************************************************/
/* stdarg.h   v3.2.1                                                         */
/* Copyright (c) 1996-2009 Texas Instruments Incorporated                    */
/*****************************************************************************/

#ifndef _STDARG
#define _STDARG

#ifdef __cplusplus
#define _NS_PREFIX std::
namespace std {
#else
#define _NS_PREFIX
#endif /* __cplusplus */

typedef char *va_list;

#ifdef __cplusplus
} /* namespace std */
#endif

#define _VA_RNDUP(sz) 	(((sz) + 1) & ~1)

/*****************************************************************************/
/* On the MSP430, the stack grows down (towards 0x0), and arguments are      */
/* pushed in reverse order, so later arguments are at higher addresses.      */
/*****************************************************************************/

/*****************************************************************************/
/* VA_START(va_list ap, parmN)                                               */
/*                                                                           */
/*   Set "ap" to point to the address of the next argument past parmN.       */
/*   So add the size of parmN to the address of parmN.                       */
/*                                                                           */
/*   NOTES -must use the address of the level 1 symbol (via va_parmadr).     */
/*         -must account for "chars", which are widened to "ints".           */
/*                                                                           */
/*****************************************************************************/
#if defined(__LARGE_DATA_MODEL__)
#define va_start(ap, parmN)                                                   \
((ap) = (_NS_PREFIX va_list)((long)__va_parmadr(parmN) +                      \
			     (__va_argref(parmN) ?                            \
			      sizeof(&parmN) :                                \
			      (sizeof(parmN) < sizeof(int) ? sizeof(int) :    \
			                                     sizeof(parmN)))))
#else
#define va_start(ap, parmN)						      \
   ((ap) = (__va_argref(parmN) || (sizeof(parmN) < sizeof(int)) 	      \
              ? (_NS_PREFIX va_list)((int)__va_parmadr(parmN) + 2 )	      \
	      : (_NS_PREFIX va_list)((int)__va_parmadr(parmN) + sizeof(parmN))))
#endif
/*****************************************************************************/
/* VA_ARG(va_list, type)                                                     */
/*                                                                           */
/*   Return next argument (currently pointed to by "va_list", and set the    */
/*   argument pointer to point to the next argument after current one.       */
/*                                                                           */
/*   Notes -must handle variables passed by reference (_va_argref())         */
/*         -must round up chars.                                             */
/*                                                                           */
/*****************************************************************************/
#define va_arg(_ap, _type)                                       	      \
        (__va_argref(_type)                         		              \
	 ? ((_ap += sizeof(_type*)), (**(_type**)(_ap - sizeof(_type *))))    \
	 : ((_ap += _VA_RNDUP(sizeof(_type))),				      \
	     (*(_type*)(_ap - _VA_RNDUP(sizeof(_type))))))

#define va_end(ap)

#ifdef __cplusplus

#ifndef _CPP_STYLE_HEADER
using std::va_list;
#endif /* _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#else

#ifdef __cplusplus

#ifndef _CPP_STYLE_HEADER
using std::va_list;
#endif /* _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#endif /* _STDARG */
