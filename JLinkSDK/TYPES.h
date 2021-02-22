/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*           (C) 2003    SEGGER Microcontroller Systeme GmbH          *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : Types.h
Purpose : Global data types
---------------------------END-OF-HEADER------------------------------
*/

#ifndef TYPES_H            // Guard against multiple inclusion
#define TYPES_H

/*********************************************************************
*
*       Defines, basic types
*
**********************************************************************
*/

#define   U8    unsigned char
#define   U16   unsigned short
#if (defined (__SES_ARM) || defined (__ICCARM__))
  #define   U32   unsigned long
#else
  #define   U32   unsigned int
#endif
#define   I8    signed char
#define   I16   signed short
#if (defined (__SES_ARM) || defined (__ICCARM__))
  #define   I32   signed long
#else
  #define   I32   signed int
#endif

#ifdef _MSC_VER
  #define U64   unsigned __int64
  #define I64   signed __int64
  #if _MSC_VER <= 1200
    #define U64_C(x) x##UI64
    #define I64_C(x) x##I64
  #else
    #define U64_C(x) x##ULL
    #define I64_C(x) x##LL
  #endif
#else
  #define U64   unsigned long long
  #define I64   signed long long
  #define U64_C(x) x##ULL
  #define I64_C(x) x##LL
#endif

#if (defined(_WIN64) || defined(__LP64__))  // 64-bit symbols used by Visual Studio and GCC, maybe others as well.
  #define PTR_ADDR  U64
#else
  #define PTR_ADDR  U32
#endif

#endif                      // Avoid multiple inclusion

/*************************** End of file ****************************/
