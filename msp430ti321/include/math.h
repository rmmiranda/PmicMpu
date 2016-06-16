/*****************************************************************************/
/* math.h     v3.2.1                                                         */
/* Copyright (c) 1996-2009 Texas Instruments Incorporated                    */
/*****************************************************************************/

#ifndef _MATH
#define _MATH

#include <linkage.h>

#include <float.h>
#define HUGE_VAL   DBL_MAX
#define HUGE_VALL  DBL_MAX

#ifdef __cplusplus
//----------------------------------------------------------------------------
// <cmath> IS RECOMMENDED OVER <math.h>.  <math.h> IS PROVIDED FOR 
// COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++
//----------------------------------------------------------------------------
extern "C" namespace std {
#endif

/***************************************************************/
/* FUNCTION DEFINITIONS.                                       */
/***************************************************************/
        double modf(double x, double *y); 
        double asin(double x);
        double acos(double x);
        double atan(double x);
        double atan2(double y, double x);
 _IDECL double ceil(double x);
        double cos(double x);
        double cosh(double x);
        double exp(double x);
        double fabs(double x);
  _IDECL double floor(double x);
         double fmod(double x, double y);
         double frexp(double x, int *exp);
         double ldexp(double x, int exp);
         double log(double x);
         double log10(double x);
         double pow(double x, double y);
         double sin(double x);
         double sinh(double x);
         double tan(double x);
         double tanh(double x);
         double sqrt(double x);

#if defined(_CEIL) ||						\
    (defined(_INLINE) && 					\
     !(defined(_OPTIMIZE_FOR_SPACE) && defined(__TMS470__)))
_IDEFN double ceil(double x)
{
     double y; 
     return (modf(x, &y) > 0 ? y + 1.0 : y);
}
#endif /* (_CEIL || (_INLINE && !(_OPTIMIZE_FOR_SPACE && __TMS470))) */

#if defined(_FLOOR) ||						\
    (defined(_INLINE) && 					\
     !(defined(_OPTIMIZE_FOR_SPACE) && defined(__TMS470__)))
_IDEFN double floor(double x)
{
     double y; 
     return (modf(x, &y) < 0 ? y - 1.0 : y);
}
#endif /* (_FLOOR || (_INLINE && !(_OPTIMIZE_FOR_SPACE && __TMS470))) */

#ifdef __cplusplus
} /* extern "C" namespace std */

#ifndef _CPP_STYLE_HEADER
using std::modf;
using std::asin;
using std::acos;
using std::atan;
using std::atan2;
using std::ceil;
using std::cos;
using std::cosh;
using std::exp;
using std::fabs;
using std::floor;
using std::fmod;
using std::frexp;
using std::ldexp;
using std::log;
using std::log10;
using std::pow;
using std::sin;
using std::sinh;
using std::tan;
using std::tanh;
using std::sqrt;
#endif /* _CPP_STYLE_HEADER */
#endif /* __cplusplus */

/*******************************************************************************/
/* Overloaded version of math functions for float and long double in C++       */
/*******************************************************************************/
#include <cpp_inline_math.h>

#endif /* _MATH */
