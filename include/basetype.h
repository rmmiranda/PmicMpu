/*****************************************************************************
 * * Filename:    basetype.h
 * *
 * * Copyright 1997-2000, Research In Motion Ltd
 * *
 * * Description: Basic type definitions
 * ****************************************************************************/
#ifndef __BASETYPE_H__
#define __BASETYPE_H__

#ifdef _MSC_VER
#pragma pack( push, 1 )
#endif

/**
 * @name The Basic Types
 */
/*@{*/
typedef unsigned char       BYTE;   /**<  Single byte. */
typedef signed char         SBYTE;  /**<  Single, signed byte. */
typedef unsigned short      WORD;   /**<  Single word. */
typedef signed short        SWORD;  /**<  Single, signed word. */
#if defined( __ADSP218X__ )
typedef unsigned long       DWORD;  /**<  Double word. */
typedef signed long         SDWORD; /**<  Double, signed word. */
#elif defined( __MSP430__ )
typedef unsigned long       DWORD;
typedef signed long         SDWORD;
#elif defined( PROCESSOR_MQ31 )
typedef unsigned long       DWORD;
typedef signed long         SDWORD;
#elif defined( _MSC_VER )
typedef unsigned long       DWORD;  /**<  Double word. */
typedef signed long         SDWORD; /**<  Double, signed word. */
typedef unsigned __int64    QWORD;  /**<  Quadruple word. */
typedef signed __int64      SQWORD; /**<  Quadruple, signed word. */
#define MAKE_QWORD( val )  ((QWORD) val ## ui64) /**<  Create a QWORD constant. */
#define MAKE_SQWORD( val ) ((SQWORD) val ## i64) /**<  Create an SQWORD constant. */
#else
typedef unsigned int        DWORD;  /**<  Double word. */
typedef signed int          SDWORD; /**<  Double, signed word. */
typedef unsigned long long  QWORD;  /**<  Quadruple word. */
typedef signed long long    SQWORD; /**<  Quadruple, signed word. */
#define MAKE_QWORD( val )  ((QWORD) val ## uLL)  /**<  Create a QWORD constant. */
#define MAKE_SQWORD( val ) ((SQWORD) val ## LL)  /**<  Create an SQWORD constant. */
#endif
typedef long                LONG;   /**<  Long. */
typedef unsigned int        UINT;   /**<  Unsigned integer. */

typedef int                 BOOL;   /**<  Boolean type. */

#if ( defined(_SC100_) && !defined(_CW_X_SLLD_) ) || defined(__CCC__)
typedef QWORD               DOUBLE; /**<  IEEE 754 doubles not supported on MSA or Freescale when 64-bit doubles disabled. */
#else
typedef double              DOUBLE; /**<  Software emulated, or FPU support for IEEE 754 double precision (64-bit). */
#endif
typedef float               FLOAT;  /**<  Software emulated, or FPU support for IEEE 754 single precision (32-bit). */

typedef BYTE                TASK;   /**<  Task type. */

typedef void                (*FUNCTION)(void); /**<  Void function. */
typedef void                (*FUNCTION1)(WORD); /**< WORD function */

#ifdef _MSC_VER
    // Define ARM compiler modifiers to make sense to MSVC
    #ifndef __align
    #define __align(x) __declspec( align(x) )
    #endif
    #ifndef __packed
    #define __packed
    #endif
#endif

#ifndef APIFUNC
/**
 * A marker used by thunkgen.pl to identify functions to expose through
 * the thunk layer.
 */
#define APIFUNC
#endif
#ifndef IPIFUNC
/**
 * A marker used by the simulator for internal functions that are exported to
 * OS-level modules.
 */
#define IPIFUNC
#endif

#ifndef TRUE
  #ifdef __cplusplus
    const BOOL              TRUE  = 1;   /**<  Boolean true value. */
    const BOOL              FALSE = 0;   /**<  Boolean false value. */
  #else
// NOTE: The BugBuild parser does not recognise compiler switches, so the
//       following BUGBUILD_ENUM definitions should be parsed by BugBuild,
//       regardless of the states of the above compiler switches.
#define BUGBUILD_ENUM "BOOL"
    #define                 TRUE    1    /**<  Boolean true value. */
    #define                 FALSE   0    /**<  Boolean false value. */
#undef BUGBUILD_ENUM
  #endif
#endif

#ifndef NULL
  #ifdef __cplusplus
    #define NULL 0            /**<  Null pointer value. */
  #else
    #define NULL ((void *) 0) /**<  Null pointer value. */
  #endif
#endif
/*@}*/


/**
 * @name Bitmap Masks
 */
/*@{*/
#define BITMAP_ORIENTATION_MASK             0x80 /**<  Orientation mask. */
#define BITMAP_COLOUR_MASK                  0x60 /**<  Colour mask. */
#define BITMAP_DEPTH_MASK                   0x07 /**<  Depth mask. */
/*@}*/


/**
 * @name Bitmap orientation types
 */
/*@{*/
#define COLUMNWISE_BITMAP                   0x00 /**<  Columnwise bitmap. */
#define ROWWISE_BITMAP                      0x80 /**<  Rowwise bitmap. */
/*@}*/


/**
 * @name Bitmap Colour types
 */
/*@{*/
#define GREYSCALE_BITMAP                    0x00 /**<  Greyscale bitmap. */
#define COLOUR_BITMAP                       0x40 /**<  Colour bitmap. */
#define COLOUR_YUV_BITMAP                   0x20 /**<  Colour YUV bitmap. */
#define JPEG_ENCODED_BITMAP                0x100 /**<  JPEG encoded bitmap */
/*@}*/

/**
 * @name Bitmap Bit Ordering
 */
/*@{*/
#define BITMAP_BYTES_SWAPPED               0x200 /**<  Bytes are in a byte-swapped non-native format,
                                                       and require the ByteSwap version of raster copy. */
/*@}*/

/**
 * @name Bitmap composite types
 *
 * Lower 3 bits represent bitmap depth; bit depth of bitmap is
 * <code>2<sup>(<em>x</em>-1)</sup></code>, where <code><em>x</em></code> is the
 * lower 3 bits.
 */
/*@{*/
#define COLUMNWISE_MONOCHROME_BITMAP            0x01 /**<  Columnwise, monochrome bitmap. */
#define ROWWISE_MONOCHROME_BITMAP               0x81 /**<  Rowwise, monochrome bitmap. */
#define COLUMNWISE_2BIT_GREYSCALE_BITMAP        0x02 /**<  Columnwise, two-bit greyscale bitmap. */
#define ROWWISE_2BIT_GREYSCALE_BITMAP           0x82 /**<  Rowwise, two-bit greyscale bitmap. */
#define COLUMNWISE_4BIT_GREYSCALE_BITMAP        0x03 /**<  Columnwise, four-bit greyscale bitmap. */
#define ROWWISE_4BIT_GREYSCALE_BITMAP           0x83 /**<  Rowwise, four-bit greyscale bitmap. */
#define COLUMNWISE_16BIT_COLOUR_BITMAP          0x45 /**<  Columnwise, sixteen-bit colour bitmap. */
#define ROWWISE_16BIT_COLOUR_BITMAP             0xC5 /**<  Rowwise, sixteen-bit colour bitmap. */
#define COLUMNWISE_16BIT_COLOUR_YUV422_BITMAP   0x25 /**<  Columnwise, sixteen-bit colour YUV422 bitmap. */
#define ROWWISE_16BIT_COLOUR_YUV422_BITMAP      0xA5 /**<  Rowwise, sixteen-bit colour YUV422 bitmap. */
/*@}*/


/**
 * Bitmap structure.
 *
 * Structure to encapsulate all bitmaps supported on the device. The first two
 * fields specify the (a) type of the bitmap, which should be one of the static
 * "bitmap composite types", and (b) the width and height of the image in
 * pixels.
 *
 * The third field in the structure holds a pointer to the acutal image data.
 */
typedef struct
{
  /**
   * Bitmap type (one of the bitmap composite types).
   *
   * This field only requires a BYTE, but is a DWORD to force natural alignment.
   */
  DWORD   bType;
  WORD    wide;          /**<  Width of image, in pixels. */
  WORD    high;          /**<  Height of image, in pixels. */
  BYTE    *data;         /**<  Pointer to image data. */
} BitMap;


/**
 * Rectangle structure.
 *
 * Structure to encapsulate an abstract rectangular region.
 */
typedef struct
{
  SDWORD      x;         /**<  Left edge of region. */
  SDWORD      y;         /**<  Top edge of region. */
  SDWORD      width;     /**<  Width of region. */
  SDWORD      height;    /**<  Height of region. */
} Rect;

#ifdef _MSC_VER
#pragma pack( pop )
#endif

#endif


/**
 * @file
 *
 * Provides defintions of basic data types.
 */
