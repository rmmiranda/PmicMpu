/* Product-specific DLib configuration. */
/* Copyright (C) 2003 IAR Systems.  All rights reserved. */

#ifndef _DLIB_PRODUCT_H
#define _DLIB_PRODUCT_H

/*
 * This is a template file that must be adopted to each product.
 *
 * This file is included right after the _DLIB_CONFIG_FILE 
 * (like DLib_Config.h) file that the user can use to configure the library.  
 * The file DLib_Defaults.h is then included to set up defaults for all
 * configuration variables that haven't got a value.
 *
 * Typically, this file can contain two things:
 *
 * - Set up defaults that differs from DLib_Defaults.h.  Remember to
 *   honor the values specified in DLib_Defaults, for example:
 *
 *   #ifndef _DLIB_MULTIBYTE
 *   #define _DLIB_MULTIBYTE 1
 *   #endif
 *
 * - Check that configurations not supported by the product isn't used,
 *   for example:
 *
 *   #ifdef _DLIB_FLOAT_ENVIRONMENT
 *   #if _DLIB_FLOAT_ENVIRONMENT
 *   #error "This product does not support a floating point environment"
 *   #endif
 *   #endif
 */

#endif /* _DLIB_PRODUCT_H */
