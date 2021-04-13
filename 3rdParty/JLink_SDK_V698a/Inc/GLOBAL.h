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
File    : GLOBAL.h
Purpose : Global types etc.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GLOBAL_H            // Guard against multiple inclusion
#define GLOBAL_H
//
// We use the enable/disable macros for encapsulating system / 3rd party header includes
// to temporary disable all warnings for the header files as these are warnings we cannot fix
//
// For Microsoft Visual C (MSVC) compiler, this is easy as we can tell it to disable all warnings via pragmas
//
// For GCC / Clang things are more complicated because they do not provide a "disable all" pragma.
// For these, we need to explicitly disable certain warnings.
// We have disabled the ones we encountered so far but we may need to extend the list in the future
//
// Push: Remembers current settings on virtual stack
// Pop: Restores last pushed settings
//
#if defined(_MSC_VER)                         // Microsoft Visual C compiler
  #define GLOBAL_DISABLE_WARNINGS() __pragma(warning(push, 0))
  #define GLOBAL_ENABLE_WARNINGS()  __pragma(warning(pop))     // Restore warning level
#elif defined(__clang__)                      // Clang compiler
  #if defined(__cplusplus)
    #define GLOBAL_DISABLE_WARNINGS() _Pragma("clang diagnostic push") \
                                      _Pragma("clang diagnostic ignored \"-Wpragmas\"") \
                                      _Pragma("clang diagnostic ignored \"-Wclass-memaccess\"") \
                                      _Pragma("clang diagnostic ignored \"-Wdeprecated-copy\"")
  #else
    #define GLOBAL_DISABLE_WARNINGS() _Pragma("clang diagnostic push") \
                                      _Pragma("clang diagnostic ignored \"-Wpragmas\"")
  #endif
  #define GLOBAL_ENABLE_WARNINGS()    _Pragma("clang diagnostic pop")
#elif defined(__GNUC__) || defined(__GNUG__)  // GCC / g++ compiler
  //
  // Generic:
  //   diagnostic ignored -Wpragmas         Disable warnings for unknown pragmas
  //
  // C++ only:
  //   diagnostic ignored -Wclass-memaccess Disable warnings for class members being accessed C-like via memset() etc.
  //   diagnostic ignored -Wdeprecated-copy Thrown by some Qt 4.8.7 headers
  //
  #if defined(__cplusplus)
    #define GLOBAL_DISABLE_WARNINGS() _Pragma("GCC diagnostic push") \
                                      _Pragma("GCC diagnostic ignored \"-Wpragmas\"") \
                                      _Pragma("GCC diagnostic ignored \"-Wclass-memaccess\"") \
                                      _Pragma("GCC diagnostic ignored \"-Wdeprecated-copy\"")
  #else
    #define GLOBAL_DISABLE_WARNINGS() _Pragma("GCC diagnostic push") \
                                      _Pragma("GCC diagnostic ignored \"-Wpragmas\"")
  #endif
  #define GLOBAL_ENABLE_WARNINGS()    _Pragma("GCC diagnostic pop")
#else
  #define GLOBAL_DISABLE_WARNINGS()
  #define GLOBAL_ENABLE_WARNINGS()
#endif

GLOBAL_DISABLE_WARNINGS()
#include <string.h>         // For memset
GLOBAL_ENABLE_WARNINGS()
#include "TYPES.h"          // Defines standard data types

/*********************************************************************
*
*       Defines, function replacements
*
**********************************************************************
*/

//
// Set compiler defines
//
#ifdef _MSC_VER                                // Microsoft Visual C compiler?
  #define IS_MSVC                         (1)
#else
  #define IS_MSVC                         (0)
#endif

#if IS_MSVC
  //
  // Variable initialization macros to keep Visual Studio compiler happy
  // as it gives a lot of false-positives for "potentitally uninitialized variable" warnings
  // because its code analysis logic does not seem to be the best here...
  // For example the following block would create a warning that pBuf is potentially uninitialized (tested with VS2010 and VS2019 with all warnings enabled):
  // [...]
  // NeedFree = 0;
  // if (NumBytesToHandle > xxx) {
  //   pBuf = alloc();
  //   NeedFree = 1;
  // }
  // [...] Lots of code
  // if (NeedFree) {
  //   free(pBuf);
  // }
  //
  #define MSVS_INIT_0(v)      v = 0;
  #define MSVS_INIT_0_STRUCT(v) MEMSET(&v, 0, sizeof(v))
  #define MSVS_INIT_NULL(p)   p = NULL;
  #define MSVS_INIT(Var, Val) Var = Val;
#else
  #define MSVS_INIT_0(v)
  #define MSVS_INIT_0_STRUCT(v)
  #define MSVS_INIT_NULL(p)
  #define MSVS_INIT(Var, Val)
#endif

#ifdef WIN32
  #if _MSC_VER > 1300
    #define IS_MSC2010  1
  #else
    #define IS_MSC2010  0
  #endif
#else
  #define IS_MSC2010  0
#endif

#ifndef   COUNTOF
  #define COUNTOF(a)    (sizeof(a)/sizeof(a[0]))
#endif

#ifndef   ZEROFILL
  #define ZEROFILL(Obj) memset(&Obj, 0, sizeof(Obj))
#endif

#ifndef   LIMIT
  #define LIMIT(a,b)    if (a > b) a = b;
#endif

#ifndef   MIN
  #define MIN(a, b)     (((a) < (b)) ? (a) : (b))
#endif

#ifndef   MAX
  #define MAX(a, b)     (((a) > (b)) ? (a) : (b))
#endif

#ifndef   ABS
  #define ABS(a)        (((a) < 0) ? -(a) : (a))
#endif

/*********************************************************************
*
*       Types, internal
*
**********************************************************************
*/

typedef enum {IS_NOINIT, IS_RUNNING, IS_EXIT} INIT_STATE;

#endif                      // Avoid multiple inclusion

/*************************** End of file ****************************/
