/*****************************************************************************/
/* float.h    v3.2.1                                                         */
/* Copyright (c) 1996-2009 Texas Instruments Incorporated                    */
/*****************************************************************************/

/********************************************************************/
/*    KEY:  FLT_     - APPLIES TO TYPE FLOAT                        */
/*          DBL_     - APPLIES TO TYPE DOUBLE                       */
/*          LDBL_    - APPLIES TO TYPE LONG DOUBLE                  */
/********************************************************************/
#ifndef _FLOAT
#define _FLOAT

#define FLT_RADIX                         2   /* RADIX OF EXPONENT         */
#define FLT_ROUNDS                        1   /* TRUE IF ADDITION ROUNDS   */

#define FLT_DIG                           6   /* DECIMAL PRECISION         */
#define FLT_MANT_DIG                     24   /* BITS IN MANTISSA          */
#define FLT_MIN_EXP                    -125   /* SMALLEST EXPONENT         */
#define FLT_MAX_EXP                     128   /* LARGEST EXPONENT          */
#define FLT_MIN_10_EXP                  -37   /* MIN POWER OF 10           */
#define FLT_MAX_10_EXP                   38   /* MAX POWER OF 10           */
#define FLT_EPSILON        1.192092896E-07F   /* SMALLEST X WHERE 1+X != 1 */
#define FLT_MIN            1.175494351E-38F   /* SMALLEST POSITIVE VALUE   */
#define FLT_MAX            3.402823466E+38F   /* LARGEST POSITIVE VALUE    */

#define DBL_DIG         FLT_DIG               /* DECIMAL PRECISION         */
#define DBL_MANT_DIG    FLT_MANT_DIG          /* BITS IN MANTISSA          */
#define DBL_MIN_EXP     FLT_MIN_EXP           /* SMALLEST EXPONENT         */
#define DBL_MAX_EXP     FLT_MAX_EXP           /* LARGEST EXPONENT          */
#define DBL_MIN_10_EXP  FLT_MIN_10_EXP        /* MIN POWER OF 10           */
#define DBL_MAX_10_EXP  FLT_MAX_10_EXP        /* MAX POWER OF 10           */
#define DBL_EPSILON     FLT_EPSILON           /* SMALLEST X WHERE 1+X != 1 */
#define DBL_MIN         FLT_MIN               /* SMALLEST POSITIVE VALUE   */
#define DBL_MAX         FLT_MAX               /* LARGEST POSITIVE VALUE    */

#define LDBL_DIG        FLT_DIG               /* DECIMAL PRECISION         */
#define LDBL_MANT_DIG   FLT_MANT_DIG          /* BITS IN MANTISSA          */
#define LDBL_MIN_EXP    FLT_MIN_EXP           /* SMALLEST EXPONENT         */
#define LDBL_MAX_EXP    FLT_MAX_EXP           /* LARGEST EXPONENT          */
#define LDBL_MIN_10_EXP FLT_MIN_10_EXP        /* MIN POWER OF 10           */
#define LDBL_MAX_10_EXP FLT_MAX_10_EXP        /* MAX POWER OF 10           */
#define LDBL_EPSILON    FLT_EPSILON           /* SMALLEST X WHERE 1+X != 1 */
#define LDBL_MIN        FLT_MIN               /* SMALLEST POSITIVE VALUE   */
#define LDBL_MAX        FLT_MAX               /* LARGEST POSITIVE VALUE    */

#endif
