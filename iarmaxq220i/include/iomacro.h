/*
                         - IOMACRO.H -

  This file defines the Special Function Register Macros for use
  with the IAR MAXQ I/O-files.

  Copyright 2003-2006 IAR Systems. All rights reserved.

  $Revision: 957 $

  $Log$
  Revision 1.6  2006/02/23 08:38:57  IPEH
  *** empty log message ***

  Revision 1.6  2006/02/23 08:38:57Z  IPEH
  Revision 1.5  2003/12/18 17:06:19  bertils
  Revision 1.4  2003/12/18 16:04:10Z  bertils
  Revision 1.3  2003/12/03 10:01:46Z  IPEH
  Revision 1.2  2003/11/26 13:50:34Z  ifrc
  Revision 1.1  2003/07/01 15:09:57Z  ifrc
  Initial revision

*/


#ifndef _IOMACRO_H
#define _IOMACRO_H

#ifdef  __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#endif


/* The assembler uses a special set of macros... */
#if defined( __IAR_SYSTEMS_ASM__ )

;---------------------------------------------------------------------------
;
;  Macro:          __SFR_BIT
;
;  Description:    Defines constants for SFR and BITS
;
;---------------------------------------------------------------------------
;
;  Input:          NAME,ADDR,Bit0,Bit1,...,Bit7
;
;  Usage:          __SFR_BIT WREG,0xF80,,,,,,,
;
;---------------------------------------------------------------------------

__SFR_BIT  MACRO  
\1 DEFINE \2
  IF "\3" <> ""
\3    DEFINE 0
  ENDIF
  IF "\4" <> ""
\4    DEFINE 1
  ENDIF
  IF "\5" <> ""
\5    DEFINE 2
  ENDIF
  IF "\6" <> ""
\6    DEFINE 3
  ENDIF
  IF "\7" <> ""
\7    DEFINE 4
  ENDIF
  IF "\8" <> ""
\8    DEFINE 5
  ENDIF
  IF "\9" <> ""
\9    DEFINE 6
  ENDIF
  IF "\A" <> ""
\A    DEFINE 7
  ENDIF
  ENDM

;---------------------------------------------------------------------------
;
;  Macro:          __SFR_W_BIT
;
;  Description:    Defines constants for word-sized SFR and BITS
;
;---------------------------------------------------------------------------
;
;  Input:          NAME,ADDR,Bit0,Bit1,...,Bit7
;
;  Usage:          __SFR_W_BIT WREG,0xF80,,,,,,,
;
;---------------------------------------------------------------------------

__SFR_W_BIT  MACRO  
\1 DEFINE \2
  IF "\3" <> ""
\3    DEFINE 0
  ENDIF
  IF "\4" <> ""
\4    DEFINE 1
  ENDIF
  IF "\5" <> ""
\5    DEFINE 2
  ENDIF
  IF "\6" <> ""
\6    DEFINE 3
  ENDIF
  IF "\7" <> ""
\7    DEFINE 4
  ENDIF
  IF "\8" <> ""
\8    DEFINE 5
  ENDIF
  IF "\9" <> ""
\9    DEFINE 6
  ENDIF
  IF "\A" <> ""
\A    DEFINE 7
  ENDIF
  IF "\B" <> ""
\B    DEFINE 8
  ENDIF
  IF "\C" <> ""
\C    DEFINE 9
  ENDIF
  IF "\D" <> ""
\D    DEFINE 0x0A
  ENDIF
  IF "\E" <> ""
\E    DEFINE 0x0B
  ENDIF
  IF "\F" <> ""
\F    DEFINE 0x0C
  ENDIF
  IF "\G" <> ""
\G    DEFINE 0x0D
  ENDIF
  IF "\H" <> ""
\H    DEFINE 0x0E
  ENDIF 
  IF "\I" <> ""
\I    DEFINE 0x0F
  ENDIF
  ENDM

__SFR_NAME  MACRO  
\1 DEFINE \2
  ENDM

/* C Preprocessor macros, Non-existing bits cannot be represented by space */
/* Ex:  SFR_BITS(__bank0,STATUS,0x03,C,Z,OV,N,,,,) */

/* Byte sized SFRs */
#define __SFR_BITS(_MEM, _NAME,_ADDR,_A,_B,_C,_D,_E,_F,_G,_H) \
  __SFR_BIT _NAME,_ADDR,_A,_B,_C,_D,_E,_F,_G,_H

/* Word sized SFRs */
#define __SFR_W_BITS(_MEM, _NAME,_ADDR,_A,_B,_C,_D,_E,_F,_G,_H,_I,_J,_K,_L,_M,_N,_O,_P) \
  __SFR_W_BIT _NAME,_ADDR,_A,_B,_C,_D,_E,_F,_G,_H,_I,_J,_K,_L,_M,_N,_O,_P

#define __SFR(_MEM, _NAME,_ADDR) \
__SFR_NAME _NAME,_ADDR 

#define __SFR_W(_MEM, _NAME,_ADDR) \
__SFR_NAME _NAME,_ADDR 

/* Full macros (with bit-names) are used for iccpic18 */
#elif defined( __IAR_SYSTEMS_ICC__ )

#define __SFR_BITS(_MEM, _NAME,_ADDR,_A,_B,_C,_D,_E,_F,_G,_H) \
__root __no_init volatile _MEM union \
{ \
  unsigned char   _NAME; \
  struct \
  { \
    unsigned char _A:1; \
    unsigned char _B:1; \
    unsigned char _C:1; \
    unsigned char _D:1; \
    unsigned char _E:1; \
    unsigned char _F:1; \
    unsigned char _G:1; \
    unsigned char _H:1; \
  } _NAME ## _bit;  \
} @ _ADDR;

#define __SFR_W_BITS(_MEM, _NAME,_ADDR,_A,_B,_C,_D,_E,_F,_G,_H,_I,_J,_K,_L,_M,_N,_O,_P) \
__root __no_init volatile _MEM union \
{ \
  unsigned short   _NAME; \
  struct \
  { \
    unsigned short _A:1; \
    unsigned short _B:1; \
    unsigned short _C:1; \
    unsigned short _D:1; \
    unsigned short _E:1; \
    unsigned short _F:1; \
    unsigned short _G:1; \
    unsigned short _H:1; \
    unsigned short _I:1; \
    unsigned short _J:1; \
    unsigned short _K:1; \
    unsigned short _L:1; \
    unsigned short _M:1; \
    unsigned short _N:1; \
    unsigned short _O:1; \
    unsigned short _P:1; \
  } _NAME ## _bit;  \
} @ _ADDR;


#define __SFR(_MEM, _NAME,_ADDR) \
__root __no_init volatile _MEM unsigned char _NAME @ _ADDR;  

#define __SFR_W(_MEM, _NAME,_ADDR) \
__root __no_init volatile _MEM unsigned short _NAME @ _ADDR;  

#define _M(x,y) (( (x) << 6) |( (y) << 1))

#endif /* __IAR_SYSTEMS_ICC__ */

#endif /* _IOMACRO_H */
