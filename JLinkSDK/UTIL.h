/*********************************************************************
*              SEGGER MICROCONTROLLER SYSTEME GmbH                   *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996-2012 SEGGER Microcontroller Systeme GmbH           *
*                                                                    *
* Internet: www.segger.com Support: support@segger.com               *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : UTIL.h
Purpose : UTIL functions
---------------------------END-OF-HEADER------------------------------
*/

#ifndef UTIL_H    /* Guard against multiple inclusion */
#define UTIL_H

#include "GLOBAL.h"
GLOBAL_DISABLE_WARNINGS()   // Avoid that 3rd party headers throw warnings
#include <stdio.h>
#include <stdarg.h>
GLOBAL_ENABLE_WARNINGS()

#define UTIL_SIZEOF(a)    (int)sizeof(a) // x64 warning suppress: Safe to cast as we are sure to never exceed destination type boundaries here

//
// Defines to cast pointers to addresses and visa versa
//
#ifdef WIN32
  #if (defined(_WIN64) || defined(__LP64__))  // 64-bit symbols used by Visual Studio and GCC, maybe others as well.
    #define UTIL_PTR_ADDR  U64
  #else
    #define UTIL_PTR_ADDR  U32
  #endif
#else
  #define UTIL_PTR_ADDR uintptr_t
#endif

#define UTIL_ADDR2PTR(Type, Addr)  (/*lint -e(923) -e(9078)*/((Type*)((PTR_ADDR)Addr)))  // Allow cast from address to pointer.
#define UTIL_PTR2ADDR(p)           (/*lint -e(923) -e(923) -e(9078)*/((PTR_ADDR)p))      // Allow cast from pointer to address.
#define UTIL_PTR2PTR(Type, p)      (/*lint -e(9079) -e(9087)*/((Type*)p))                // Allow cast from one pointer type to another.

//
// Flags for memory accesses
//

#define FLAG_READ_NO_32BIT                (1uL << 31)  // No 32-bit memory accesses are allowed (only 8- and 16-bit)
#define FLAG_READ_NO_16BIT                (1 << 30)  // No 16-bit memory accesses are allowed (only 8- and 32-bit)
#define FLAG_READ_NO_ADDR_INC             (1 << 29)  // Do not increment address on memory access (Read from the same address multiple times)
// 28 is free for future use
#define MEM_ACCESS_FLAGS_ACCESS_TYPE      (0xF << 24)  // Flags which specify more precisely, how memory shall be accessed

struct FLASH_BLOCK_INFO;      // Defined in FLASHBLOCK.h, which we do not want to include here, as this header is shipped with the SDK as well

typedef struct {
  char acRev[4];
  U32  Major;
  U32  Minor;
} UTIL_VERSION_INFO;

#if defined(__cplusplus)
  extern "C" {                // Make sure we have C-declarations in C++ programs
#endif

U64             UTIL_CalcGCD                  (U64 v, U64 w);
int             UTIL_CalcParity               (U32 Data);
void            UTIL_CopyAPIStruct            (void* pDest, const void* pSrc, const char* sStructName);
void            UTIL_CopyArrayOfItems         (void* paDest, unsigned ItemSizeDest, const void* paSrc, unsigned ItemSizeSrc, unsigned NumItemsToCopy);
void            UTIL_CopyString               (char* pDest, const char* pSrc, U32 MaxLen);
int             UTIL_CountBits                (U32 Mask);
void            UTIL_ClrBitArray              (U8 *p, U32 Off, int NumBits);
int             UTIL_FindDifference           (const void* p0, const void* p1, int NumBytes);
int             UTIL_FindLSB                  (U32 Data);
int             UTIL_FindMSB                  (U32 Data);
int             UTIL_FindMSB64                (U64 Data);
const char*     UTIL_GetCompanyName           (void);
const char*     UTIL_GetCompanyURL            (void);
const char*     UTIL_GetCurrentYear           (void);
U16             UTIL_GetData                  (const U8* pData, unsigned BitPos, unsigned NumBits);
U32             UTIL_GetPtrDistance           (const void* p0, const void* p1);
U32             UTIL_GetDiff                  (int a, int b);
int             UTIL_GetQuartiles             (U64 Min, U64 Max, const U64* pValues, U64 NumValues, U64* paBin, U32 NumBins, U64 MedianResolution, U64* pQ25, U64* pMedian, U64* pQ75);
I32             UTIL_IDiv                     (I32 v, U32 Div);
int             UTIL_SubnetMask2String        (U32 SubnetMask, char* pBuffer, U32 BufferSize);
void            UTIL_IPAddr2String            (U32 IPAddr, char* pBuffer, U32 BufferSize);
void            UTIL_MACAddr2String           (U8* pMACAddr, char ByteDelimiter, char* pBuffer, U32 BufferSize);
int             UTIL_String2IPAddr            (const char* sIPAddr, U32* pIPAddr);
int             UTIL_IsFilledWithChar         (const void* pData, U8 FillChar, U32 NumBytes);
int             UTIL_IsInRange                (U64 Addr, U64 SAddr, U32 Size);
U8              UTIL_Load8                    (const U8* p);
U16             UTIL_Load16BE                 (const U8* p);
U16             UTIL_Load16HE                 (const U8* p);
U16             UTIL_Load16LE                 (const U8* p);
U16             UTIL_Load16TE                 (const U8* p, int IsBigEndian);
U32             UTIL_Load32BE                 (const U8* p);
U32             UTIL_Load32HE                 (const U8* p);
U32             UTIL_Load32LE                 (const U8* p);
U32             UTIL_Load32LEIncPtr           (const U8** pp);
U32             UTIL_Load32TE                 (const U8* p, int IsBigEndian);
U64             UTIL_Load64BE                 (const U8* p);
U64             UTIL_Load64LE                 (const U8* p);
U64             UTIL_Load64TE                 (const U8* p, int IsBigEndian);
void            UTIL_Load128LE                (const U8* p, U64* pLow, U64* pHigh);
void            UTIL_Load128BE                (const U8* p, U64* pLow, U64* pHigh);
U32             UTIL_Load32_ULEB128           (const U8** pp);
int             UTIL_memcmp                   (const void* p0, const void* p1, int NumBytes);
int             UTIL_memicmp                  (const U8 * p0, const U8 * p1, int NumBytes);
int             UTIL_memcmp_C                 (const void* p0, const void* p1, int NumBytes);
int             UTIL_memicmp_C                (const void* p0, const void* p1, int NumBytes);
U16             UTIL_Mirror16                 (U16 Data);
U32             UTIL_Mirror32                 (U32 Data);
void            UTIL_InsertU32                (U8 * p, U32 Data, U32 Shift);
void            UTIL_InsertU32Multiple        (U8 * pDest, const U32 * pData, U32 Shift, unsigned BitOff, unsigned NumItems);
int             UTIL_ParseInt                 (const char* pString, U32* pInt);
int             UTIL_ParseIntEx               (const char** ps, U32* pVal);
int             UTIL_ParseHex32               (const char** ps, U32* pVal);
int             UTIL_ParseHex64               (const char** ps, U64* pVal);
int             UTIL_ParseChar                (const char** ps, char c);
int             UTIL_ParseIPAddr              (const char** ps, U32* pIPAddr, U32* pPort);
int             UTIL_ParseVersionNo           (const char** psVersion, U32* pMajor, U32* pMinor, U32* pRev);
unsigned        UTIL_IsWhiteChar              (char c);
void            UTIL_EatWhite                 (const char** ps);
void            UTIL_TrimString               (char* s, int TrimLeading, int TrimTrailing);
const char*     UTIL_ParseString              (const char** ps, char* pBuffer, unsigned BufferSize);
const char*     UTIL_ParseStringEx            (const char** ps, char* sDest, int MaxLen, char TerminatingChar);
int             UTIL_ParseHostName            (const char** ps, char* pBuffer, unsigned BufferSize);
int             UTIL_ParseCSVEntry            (const char** ps, char* sDest, int DestSize, char TerminatingChar);
int             UTIL_PrintFloat               (char* pBuffer, U32 BufferSize, U32 Value);
void            UTIL_PrintMemSize             (I64 MemSize, char* pBuffer, U32 BufferSize);
void            UTIL_PrintTime                (U64 Time, char* pBuffer, U32 BufferSize);
U32             UTIL_PrintHexString           (char* pBuffer, U32 BufferSize, const U8* pData, U32 NumBytes, int ShowOffset, U32 Offset, U32 BytesPerLine);
void            UTIL_PrintHex32               (char* pBuf, U32 BufSize, U32 Data32);
void            UTIL_PrintHex64               (char* pBuf, U32 BufSize, U64 Data64);
int             UTIL_RangesIntersect          (U64 Addr0, U32 NumBytes0, U64 Addr1, U32 NumBytes1);
void            UTIL_HOST_ReverseEndian       (U8 * pData, U64 Addr, U32 NumBytes, U32 Flags);
void            UTIL_ReverseEndian            (U8 * pDest, U8* pSrc, U32 Addr, U32 NumBytes);
void            UTIL_SetBitArray              (U8 *p, U32 Off, int NumBits);
char*           UTIL_snprintf                 (char * pBuffer, int BufferSize, const char * sFormat, ...);
void            UTIL_Store16BE                (U8* p, U16 Data);
void            UTIL_Store16HE                (U8* p, U16 Data);
void            UTIL_Store16LE                (U8* p, U16 Data);
void            UTIL_Store16TE                (U8* p, U16 Data, int IsBigEndian);
void            UTIL_Store32BE                (U8* p, U32 Data);
void            UTIL_Store32HE                (U8* p, U32 Data);
void            UTIL_Store32LE                (U8* p, U32 Data);
void            UTIL_Store32LEIncPtr          (U8** pp, U32 Data);
void            UTIL_Store32TE                (U8* p, U32 Data, int IsBigEndian);
void            UTIL_Store64BE                (U8* p, U64 Data);
void            UTIL_Store64LE                (U8* p, U64 Data);
void            UTIL_Store64TE                (U8* p, U64 Data, int IsBigEndian);
void            UTIL_Store128BE               (U8* p, U64 DataLow, U64 DataHigh);
void            UTIL_Store128LE               (U8* p, U64 DataLow, U64 DataHigh);
void            UTIL_Store32_ULEB128          (U8** pp, U32 Value);
void            UTIL_strcat                   (char * sDest, const char * sSrc, int BufferSize);
void            UTIL_strncat                  (char * sDest, const char * sSrc, int MaxAppendSize, int BufferSize);
int             UTIL_stricmp                  (const char * dst, const char * src);
int             UTIL_strlen                   (const char* s);
int             UTIL_strnlen                  (const char* s, int maxlen);
int             UTIL_strnicmp                 (const char * first, const char * last, size_t count);
void            UTIL_strncpy                  (char * sDest, const char * sSrc, U32 Count, U32 BufSize);
void            UTIL_strupr                   (char * s);
U16             UTIL_SwapU16                  (U16 v);
void            UTIL_SwapU16Array             (U16* pDest, const U16* pSrc, U32 NumItems);
U32             UTIL_SwapU32                  (U32 v);
void            UTIL_SwapU32Array             (U32* pDest, const U32* pSrc, U32 NumItems);
U64             UTIL_SwapU64                  (U64 v);
void            UTIL_SwapU64Array             (U64* pDest, const U64* pSrc, U32 NumItems);
void            UTIL_SwapArray                (void* pDest, const void* pSrc, U32 ItemSize, U32 NumItems);
void            UTIL_ReverseStream            (void* pStream, U32 NumBytes);
U64             UTIL_U64Scale                 (U64 v, U64 Mul, U64 Div);
U32             UTIL_UDiv                     (U32 v, U32 Div);
U32             UTIL_UDivMod                  (U32 v, U32 Div, U32 * pRemainder);
U32             UTIL_UMod                     (U32 v, U32 Div);
char*           UTIL_vsnprintf                (char * pBuffer, int BufferSize, const char * sFormat, va_list * pParamList);
U32             UTIL_ExtractU32LE             (const U8 * pData, U32 BitPos);
void            UTIL_ToLower                  (char* s);
void            UTIL_NormalizePath            (char* sPath);
void            UTIL_TerminatePathIfNecessary (char* sPath, U32 BufSize);
unsigned        UTIL_IsValidNickName          (const char* s);
unsigned        UTIL_IsValidHostname          (const char* s);
int             UTIL_FindChar                 (const char* s, char c);
void            UTIL_ReplaceChars             (char* s, char cOld, char cNew);
const char*     UTIL_OSType2Str               (U32 OSType);
int             UTIL_log2                     (int Data);
int             UTIL_GetDateDiff              (U32 DateA, U32 DateB);
unsigned        UTIL_IsPrintableASCII         (char c);
char            UTIL_Nibble2HexChar           (char c);
int             UTIL_HexDecode                (char* pDest, const char* pSrc, unsigned NumBytes);
U32             UTIL_PrintFlashBankSizes      (const struct FLASH_BLOCK_INFO** ppaBlockInfo, U32 NumBanks, char* pBuf, U32 BufSize);
void            UTIL_GetVersionInfo           (UTIL_VERSION_INFO* pInfo, U32 Version);
char            UTIL_toupper                  (char c);
char            UTIL_tolower                  (char c);
//
// J-Link specific from UTIL_JLINK.c
//
void            UTIL_JLINK_Core2String        (U32 Core, char* pBuffer, unsigned BufferSize);

#if defined(__cplusplus)
}     /* Make sure we have C-declarations in C++ programs */
#endif

#endif    /* Guard against multiple inclusion */

/************************** end of file *****************************/
