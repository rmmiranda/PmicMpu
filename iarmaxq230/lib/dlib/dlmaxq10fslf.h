/* Customer-specific DLib configuration. */
/* Copyright (C) 2006 IAR Systems.  All rights reserved. */

#ifndef _DLIB_CONFIG_H
#define _DLIB_CONFIG_H

/* Turn on locale support. */
#define _DLIB_FULL_LOCALE_SUPPORT 1

/* Turn on FILE descriptor support. */
#define _DLIB_FILE_DESCRIPTOR 1

/* Turn on multibyte formatting. */
#define _DLIB_FORMATTED_MULTIBYTE 1

/* Turn on support for hex-floats in strtod. */
#define _DLIB_STRTOD_HEX_FLOAT 1

/* Turn off long long support in printf */
#define _DLIB_PRINTF_LONG_LONG    0

/* Turn off C99 %a support in printf */
#define _DLIB_PRINTF_SPECIFIER_A  0

/* Turn off long long support in scanf */
#define _DLIB_SCANF_LONG_LONG    0


#endif /* _DLIB_CONFIG_H */
