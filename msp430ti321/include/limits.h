/*****************************************************************************/
/* limits.h   v3.2.1                                                         */
/* Copyright (c) 1996-2009 Texas Instruments Incorporated                    */
/*****************************************************************************/

#ifndef _LIMITS
#define _LIMITS

#define MB_LEN_MAX               1      /* MAX # BYTES IN MULTI-BYTE        */
#define CHAR_BIT                 8      /* NUMBER OF BITS IN TYPE CHAR      */
#define SCHAR_MIN             -128      /* MIN VALUE FOR SIGNED CHAR        */
#define SCHAR_MAX              127      /* MAX VALUE FOR SIGNED CHAR        */
#define UCHAR_MAX              255      /* MAX VALUE FOR UNSIGNED CHAR      */

#ifdef __signed_chars__
#define CHAR_MIN         SCHAR_MIN      /* MIN VALUE FOR CHAR               */
#define CHAR_MAX         SCHAR_MAX      /* MAX VALUE FOR CHAR               */
#else
#define CHAR_MIN                 0
#define CHAR_MAX         UCHAR_MAX
#endif
 
#define SHRT_MAX             32767      /* MAX VALUE FOR SHORT              */
#define SHRT_MIN     (-SHRT_MAX-1)      /* MIN VALUE FOR SHORT              */
#define USHRT_MAX            65535u     /* MAX VALUE FOR UNSIGNED SHORT     */

#define INT_MAX              32767      /* MAX VALUE FOR INT                */
#define INT_MIN       (-INT_MAX-1)      /* MIN VALUE FOR SHORT              */
#define UINT_MAX             65535u     /* MAX VALUE FOR UNSIGNED INT       */

#define LONG_MAX        2147483647      /* MAX VALUE FOR LONG               */
#define LONG_MIN     (-LONG_MAX-1)      /* MIN VALUE FOR LONG               */
#define ULONG_MAX       4294967295      /* MAX VALUE FOR UNSIGNED LONG      */

#endif
