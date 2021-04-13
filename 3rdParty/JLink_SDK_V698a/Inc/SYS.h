/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*           (C) 2008    SEGGER Microcontroller Systeme GmbH          *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : SYS.h
Purpose : Interface definition for system specific routines
---------------------------END-OF-HEADER------------------------------
*/

#ifndef SYS_H                    // Avoid multiple inclusion
#define SYS_H

#include "GLOBAL.h"

#if defined(__cplusplus)         // Allow usage of this module from C++ files (disable name mangling)
  extern "C" {
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// While the drawing of the message box is app specific and there is no SYS function for that
// we define some common basics for the flags anyhow, so they do not differ from app to app
//
#define SYS_MSGBOX_BUTTON0                    (0x00000000uL)
#define SYS_MSGBOX_BUTTON1                    (0x00000001uL)
#define SYS_MSGBOX_ICON_NONE                  (0x00000000uL)
#define SYS_MSGBOX_ICON_QUESTION              (0x00000200uL)
#define SYS_MSGBOX_ICON_EXCLAMATION           (0x00000300uL)
#define SYS_MSGBOX_ICON_WARNING               SYS_MSGBOX_ICON_EXCLAMATION
#define SYS_MSGBOX_ICON_ERROR                 (0x00000100uL)
#define SYS_MSGBOX_ICON_INFORMATION           (0x00000400uL)
#define SYS_MSGBOX_TIMEOUT_INFINITE           (-1)

#define SYS_MSGBOX_FLAG_SETFOREGROUND         (0x00001000uL)
#define SYS_MSGBOX_FLAG_TOPMOST               (0x00002000uL)
#define SYS_MSGBOX_FLAG_CHECKBOX              (0x01000000uL)        // Creates a message box that contains a check box "Do not show this message again"

#define SYS_MSGBOX_BUTTON_MASK                (0x0000000FuL)
#define SYS_MSGBOX_DEFAULT_MASK               SYS_MSGBOX_BUTTON_MASK
#define SYS_MSGBOX_TYPE_MASK                  (0x000000F0uL)
#define SYS_MSGBOX_ICON_MASK                  (0x00000F00uL)
#define SYS_MSGBOX_FLAG_MASK                  (0xFFFFF000uL)

#define SYS_MSGBOX_BUTTON_SHIFT               (0)
#define SYS_MSGBOX_DEFAULT_SHIFT              SYS_MSGBOX_BUTTON_SHIFT
#define SYS_MSGBOX_TYPE_SHIFT                 (4)
#define SYS_MSGBOX_ICON_SHIFT                 (8)
#define SYS_MSGBOX_FLAG_SHIFT                 (12)

#define SYS_MSGBOX_TYPE_CLOSE                 (0x00000080uL)        // Enables the close button in the upper right corner of the message box

#ifndef SYS_WINDOWS_USE_SEGGER_REGISTRY                  // Windows specific: Uses the XML-based SEGGER registry instead of the Windows registry (Under Linux / macOS the SEGGER registry is the only available option and therefore enabled by default)
  #define SYS_WINDOWS_USE_SEGGER_REGISTRY 0
#endif

#define SYS_THREAD_PRIO_HIGHEST         (2)                         // Same value as THREAD_PRIORITY_HIGHEST
#define SYS_THREAD_PRIO_ABOVE_NORMAL    (1)                         // Same value as THREAD_PRIORITY_ABOVE_NORMAL
#define SYS_THREAD_PRIO_NORMAL          (0)                         // Same value as THREAD_PRIORITY_NORMAL
#define SYS_THREAD_PRIO_BELOW_NORMAL    (-1)                        // Same value as THREAD_PRIORITY_BELOW_NORMAL
#define SYS_THREAD_PRIO_LOWEST          (-2)                        // Same value as THREAD_PRIORITY_LOWEST

#define SYS_THREAD_CREATE_SUSPENDED     (1)

#define SYS_REG_HKEY_CLASSES_ROOT       ((SYS_REG_HKEY)0x80000000)  // Same value as HKEY_CLASSES_ROOT
#define SYS_REG_HKEY_CURRENT_USER       ((SYS_REG_HKEY)0x80000001)  // Same value as HKEY_CURRENT_USER
#define SYS_REG_HKEY_LOCAL_MACHINE      ((SYS_REG_HKEY)0x80000002)  // Same value as HKEY_LOCAL_MACHINE
#define SYS_REG_HKEY_USERS              ((SYS_REG_HKEY)0x80000003)  // Same value as HKEY_USERS
#define SYS_REG_HKEY_PERFORMANCE_DATA   ((SYS_REG_HKEY)0x80000004)  // Same value as HKEY_PERFORMANCE_DATA
#define SYS_REG_HKEY_CURRENT_CONFIG     ((SYS_REG_HKEY)0x80000005)  // Same value as HKEY_CURRENT_CONFIG
#define SYS_REG_HKEY_DYN_DATA           ((SYS_REG_HKEY)0x80000006)  // Same value as HKEY_DYN_DATA

#define SYS_REG_TYPE_INVALID             (0)
#define SYS_REG_TYPE_SZ                  (1)                         // Same value as REG_SZ
#define SYS_REG_TYPE_EXPAND_SZ           (2)                         // Same value as REG_EXPAND_SZ
#define SYS_REG_TYPE_BINARY              (3)                         // Same value as REG_BINARY
#define SYS_REG_TYPE_DWORD               (4)                         // Same value as REG_DWORD
#define SYS_REG_TYPE_DWORD_LITTLE_ENDIAN (4)
#define SYS_REG_TYPE_MULTI_SZ            (7)                         // Multi-string entry
#define SYS_REG_TYPE_QWORD               (11)

#define SYS_REG_FILENAME_HKCR           "SEGGER_REG_HKEY_CLASSES_ROOT.xml"
#define SYS_REG_FILENAME_HKCU           "SEGGER_REG_HKEY_CURRENT_USER.xml"
#define SYS_REG_FILENAME_HKLM           "SEGGER_REG_HKEY_LOCAL_MACHINE.xml"

#define SYS_REG_ERROR_ENTRY_NOT_FOUND   (2)                         // Same value as ERROR_FILE_NOT_FOUND under Windows
#define SYS_REG_ERROR_MORE_DATA         (234)                       // Same value as ERROR_MORE_DATA under Windows

#define SYS_REG_FLAG_READ               (1 << 0)                    // Open registry key for reading
#define SYS_REG_FLAG_WRITE              (1 << 1)                    // Open registry key for writing

#define SYS_FILE_FLAG_READ              (1 << 0)                    // Open the file for reading.
#define SYS_FILE_FLAG_WRITE             (1 << 1)                    // Open the file for writing.
#define SYS_FILE_FLAG_SHARE_READ        (1 << 2)                    // Allow other processes to open the same file for reading.
#define SYS_FILE_FLAG_SHARE_WRITE       (1 << 3)                    // Allow other processes to open the same file for writing.
#define SYS_FILE_FLAG_SHARE_DELETE      (1 << 8)                    // Allow other processes to open the same file for deleting.
#define SYS_FILE_FLAG_SHARE             (SYS_FILE_FLAG_SHARE_READ | SYS_FILE_FLAG_SHARE_WRITE)
#define SYS_FILE_FLAG_CREATE            (1 << 4)                    // Create new file. If the file already exist, open it.
#define SYS_FILE_FLAG_TRUNC             (1 << 5)                    // Truncate the file to zero length.
#define SYS_FILE_FLAG_APPEND            (1 << 6)                    // Set file pointer to end of file.
#define SYS_FILE_FLAG_EXCL              (1 << 7)                    // Can be specified in combination with SYS_FILE_FLAG_CREATE. Open fails if the specified file already exists.
#define SYS_FILE_FLAG_OVERLAPPED        (1 << 9)                    // Enables asynchronous I/O operations on file.
#define SYS_FILE_FLAG_ISDIR             (1 << 10)                   // The requested "file" is a directory

#define SYS_FILE_SEEK_BEGIN             (0)
#define SYS_FILE_SEEK_CURRENT           (1)
#define SYS_FILE_SEEK_END               (2)

#define SYS_INVALID_FILE_ATT            (0xFFFFFFFF)                // Same value as INVALID_FILE_ATTRIBUTES
#define SYS_FILE_ATT_READONLY           (1)                         // Same value as FILE_ATTRIBUTE_READONLY
#define SYS_FILE_ATT_ISDIR              (16)                        // Same value as FILE_ATTRIBUTE_DIRECTORY

#define SYS_FILE_NUM_RETRIES_DEFAULT   3
#define SYS_FILE_SLEEP_RETRIES_DEFAULT 5  // [ms]

#define SYS_INVALID_HANDLE              (SYS_HANDLE)((size_t)-1)
#define SYS_INVALID_SOCKET              (0xFFFFFFFF)
#define SYS_INFINITE_TIMEOUT            (0xFFFFFFFF)                // Same value as INFINITE
#define SYS_STILL_ACTIVE                ((U32)0x00000103)           // Same value as STILL_ACTIVE
//
// SYS_WAIT_ values are identical to Windows ones for WAIT_
//
#define SYS_WAIT_FAILED                 ((U32)0xFFFFFFFF)           // Same value as WAIT_FAILED
#define SYS_WAIT_OK                     ((U32)0x00000000)           // Same value as WAIT_OBJECT_0
#define SYS_WAIT_TIMEOUT                ((U32)0x00000102)           // Same value as WAIT_TIMEOUT

#define SYS_ERROR_FILE_NOT_FOUND        (2)
#define SYS_ERROR_PATH_NOT_FOUND        (3)
#define SYS_ERROR_ACCESS_DENIED         (5)
#define SYS_ERROR_INVALID_DRIVE         (15)
#define SYS_ERROR_SHARING_VIOLATION     (32)
#define SYS_ERROR_INVALID_NAME          (123)
#ifdef APP_JFLASH_ARM
  #define SYS_MAX_PATH                  (512)
#else
  #ifdef WIN32
    #define SYS_MAX_PATH                (260)                       // Max. length of full pathname
  #else
    #define SYS_MAX_PATH                (512)
  #endif
#endif
#define SYS_MAX_DRIVE                   (3)                         // Max. length of drive component
#define SYS_MAX_DIR                     (256)                       // Max. length of path component
#define SYS_MAX_FNAME                   (256)                       // Max. length of file name component
#define SYS_MAX_EXT                     (256)                       // Max. length of extension component

#define SYS_SOCKET_INVALID_HANDLE       (-1)
#define SYS_SOCKET_IP_ADDR_ANY          0
#define SYS_SOCKET_IP_ADDR_LOCALHOST    0x7F000001                  // 127.0.0.1 (localhost)

#define SYS_SOCKET_PORT_ANY             0

#define SYS_SOCKET_ERR_UNSPECIFIED      -1
#define SYS_SOCKET_ERR_WOULDBLOCK       -2
#define SYS_SOCKET_ERR_TIMEDOUT         -3
#define SYS_SOCKET_ERR_CONNRESET        -4
#define SYS_SOCKET_ERR_INTERRUPT        -5

#define SYS_SOCKET_SHUT_RD              0
#define SYS_SOCKET_SHUT_WR              1
#define SYS_SOCKET_SHUT_RDWR            2

#define SYS_COM_DEFAULT_TIMEOUT         100
#define SYS_COM_INVALID_HANDLE          NULL
//
// Note:
//    The SYS layer assumes the DB9 Male pinout:
//   *  -------------------  *
//   * / 1   2   3   4   5 \ *
//   * \                   / *
//   *  \  6   7   8   9  /  *
//   *   \_______________/   *
//     
//    Pin  Signal   I/O   Description
//    ---------------------------------
//    [1]  CD       IN     Carrier Detect
//    [2]  RxD*     IN     Receive Data
//    [3]  TxD*     OUT    Transmit Data
//    [4]  DTR      OUT    Data Terminal Ready
//    [5]  GND      ---    Ground
//    [6]  DSR      IN     Data Set Ready
//    [7]  RTS      OUT    Request To Send
//    [8]  CTS      IN     Clear To Send
//    [9]  RI       IN     Ring Indicator
// 
//  (*) Controlled/ Affected by SYS_COM_Read() and SYS_COM_Write(), not by SYS_COM_ pin functions.
//
#define SYS_COM_PIN_LE                  (1 << 0)         // DSR (data set ready/ line enable)
#define SYS_COM_PIN_DTR                 (1 << 1)         // DTR (data terminal ready)
#define SYS_COM_PIN_RTS                 (1 << 2)         // RTS (request to send)
#define SYS_COM_PIN_CTS                 (1 << 3)         // CTS (clear to send)
#define SYS_COM_PIN_CAR                 (1 << 4)         // DCD (data carrier detect)
#define SYS_COM_PIN_CD                  SYS_COM_PIN_CAR  // See SYS_COM_PIN_CAR
#define SYS_COM_PIN_RNG                 (1 << 5)         // RNG (ring)
#define SYS_COM_PIN_RI                  SYS_COM_PIN_RNG  // See SYS_COM_PIN_RNG
#define SYS_COM_PIN_DSR                 (1 << 6)         // DSR (data set ready)

#define SYS_IP_FLAG_GLOBALLY_UNIQUE     (1 << 0)  // Filter non-unique MACs
#define SYS_IP_FLAG_ETH_ONLY            (1 << 1)  // Get Ethernet adapters only

#define SYS_HOST_OS_TYPE_UNKNOWN               (0x0000)
#define SYS_HOST_OS_TYPE_WINDOWS               (0x1000)
#define SYS_HOST_OS_TYPE_LINUX                 (0x2000)
#define SYS_HOST_OS_TYPE_MAC                   (0x3000)
//
// Unknown OS
//
#define SYS_HOST_OS_VERSION_UNKNOWN_OS_UNKNOWN (SYS_HOST_OS_TYPE_UNKNOWN | (0 << 0))
//
// Windows OS
//
#define SYS_HOST_OS_VERSION_WINDOWS_UNKNOWN    (SYS_HOST_OS_TYPE_WINDOWS | (0 << 0))
#define SYS_HOST_OS_VERSION_WINDOWS_2K         (SYS_HOST_OS_TYPE_WINDOWS | (1 << 0))
#define SYS_HOST_OS_VERSION_WINDOWS_XP         (SYS_HOST_OS_TYPE_WINDOWS | (2 << 0))
#define SYS_HOST_OS_VERSION_WINDOWS_XP64       (SYS_HOST_OS_TYPE_WINDOWS | (3 << 0))
#define SYS_HOST_OS_VERSION_WINDOWS_VISTA      (SYS_HOST_OS_TYPE_WINDOWS | (4 << 0))
#define SYS_HOST_OS_VERSION_WINDOWS_7          (SYS_HOST_OS_TYPE_WINDOWS | (5 << 0))
#define SYS_HOST_OS_VERSION_WINDOWS_8          (SYS_HOST_OS_TYPE_WINDOWS | (6 << 0))
#define SYS_HOST_OS_VERSION_WINDOWS_8_1        (SYS_HOST_OS_TYPE_WINDOWS | (7 << 0))
#define SYS_HOST_OS_VERSION_WINDOWS_10         (SYS_HOST_OS_TYPE_WINDOWS | (8 << 0))
//
// Linux OS
//
#define SYS_HOST_OS_VERSION_LINUX_UNKNOWN      (SYS_HOST_OS_TYPE_LINUX | (0 << 0))
//
// macOS
//
#define SYS_HOST_OS_VERSION_MACOS_UNKNOWN      (SYS_HOST_OS_TYPE_MAC | (0 << 0))

#ifndef NULL
  #define NULL  ((void*)0)
#endif
//
// Implement spinlock as SYS-macros, not SYS-subroutines, because it is a lot faster
//
#define SYS_SPINLOCK_CREATE(pInfoSpinLock) { pInfoSpinLock = (SYS_SPINLOCK_INFO*)SYS_MEM_AllocEx(sizeof(SYS_SPINLOCK_INFO), "Spinlock info"); memset(pInfoSpinLock, 0, sizeof(SYS_SPINLOCK_INFO)); }
#define SYS_SPINLOCK_DELETE(pInfoSpinLock) { SYS_MEM_Free(pInfoSpinLock); pInfoSpinLock = NULL; }
#ifdef _MSC_VER  // Keep GCC happy, as it does not know the MSVC specific intrinsics but has its own ones
  //
  // Acc. to MSDN InterlockedExchange() is inlined by the compiler, where possible.
  // Lock
  //   Variable was 0? => Got the lock
  //   Variable was 1? => Did not get the lock
  // Unlock
  //   Reset variable to 0
  //
  #define SYS_SPINLOCK_LOCK(pInfoSpinLock)   { U32 LockStat; do { LockStat = _InterlockedExchange(&pInfoSpinLock->Lock, 1); } while (LockStat); }
  #define SYS_SPINLOCK_UNLOCK(pInfoSpinLock) { _InterlockedExchange(&pInfoSpinLock->Lock, 0); }
#else
  //
  // According to https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html a __sync_lock_test_and_set()
  // is not a full barrier. This means that after this builtin, some global memory stores are not visible yet
  // and some loads may not be satisfied yet.
  // Therefore, we use
  // type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)
  //
  #define SYS_SPINLOCK_LOCK(pInfoSpinLock)        { U32 LockStat; do { LockStat = __sync_val_compare_and_swap(&pInfoSpinLock->Lock, 0, 1); } while (LockStat); }
  #define SYS_SPINLOCK_UNLOCK(pInfoSpinLock)      { __sync_val_compare_and_swap(&pInfoSpinLock->Lock, 1, 0); }
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

#ifdef WIN32
  #define SYS_THREAD_PROC_EX_TYPE U32 __stdcall
  #define SYS_THREAD_PROC_EX_R_TYPE U32
#else
  #define SYS_THREAD_PROC_EX_TYPE void*
  #define SYS_THREAD_PROC_EX_R_TYPE void*
#endif

typedef void*        SYS_FILE_HANDLE;
typedef void*        SYS_REG_HKEY;
typedef void*        SYS_WIN_HANDLE;
typedef void*        SYS_HANDLE;
typedef int          SYS_SOCKET_HANDLE;               // Do not change to something else as some routines use < 0 comparisons for error detection
typedef void*        SYS_COM_HANDLE;

typedef struct {
  char acName[64];
} SYS_COM_INFO;

typedef struct {
  U8  addr[16];
} SYS_SOCKET_ADDR6;

typedef struct {
  U16 Year;
  U16 Month;
  U16 Day;
  U16 Hour;
  U16 Minute;
  U16 Second;
  U16 Milliseconds;
} SYS_TIME;

typedef struct {
  U32   IPAddr;
  U32   SubnetMask;
  U32   GWAddr;
  U8    IsDHCPEnabled;
  U8    abMAC[6];
  char  acName[64];
} SYS_IP_ADAPTER_INFO;

typedef struct {
  U32 BlockSize;
  const char* sName;    // Optional. May be NULL.
} SYS_ALLOC_ENTRY;

struct SYS_ALLOC_INFO;

typedef struct SYS_ALLOC_INFO {
  U32 Size;                 // Size of <DataBlock> requested by caller on alloc()
  U32 Signature;            // Signature of block, to detect if we are pointing to a valid one
  const char* sName;        // Name of block. Must be constant, so available throughout the whole session
  struct SYS_ALLOC_INFO* pNext; // Next alloc block (used for memory allocation visualization in control panel)
  struct SYS_ALLOC_INFO* pPrev; // Previous alloc block
} SYS_ALLOC_INFO;

typedef struct {
  volatile U32 Lock;    // Set to 1 by Lock() and to 0 by Unlock(). Default state after Create() is == 0 
} SYS_SPINLOCK_INFO;

typedef struct {
  char* pBuf;
  U32   BufSize;
  U32   NumBytesInBuf;                // Number of bytes in buffer
  U32   Off;                          // Offset into buffer to read data from / write data to
} SYS_SOCKET_BUF_INFO;

typedef struct {
  SYS_SOCKET_HANDLE hSocket;            // Socket handle for client connection
  SYS_SOCKET_BUF_INFO Rd;
  SYS_SOCKET_BUF_INFO Wr;
} SYS_SOCKET_INFO;

typedef struct {
  SYS_TIME    TimeLastWrite;
  const char* sFile;
  const char* sPath;
  U64         FileSize;
  U32         Attr;
} SYS_FILE_FOUND_INFO;

typedef void SYS_CB_ON_FILE_FOUND       (SYS_FILE_FOUND_INFO* pInfo, void* pPara);
typedef int  SYS_CB_ON_CONNECT_INTERVAL (void);
typedef int  SYS_THREAD_PROC            (void);
typedef SYS_THREAD_PROC_EX_TYPE SYS_THREAD_PROC_EX(void* pPara);

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/

extern int              SYS_MEM_NumBlocks;
extern int              SYS_MEM_NumBytes;
extern SYS_SOCKET_ADDR6 SYS_SOCKET_IP_ADDR6_ANY;

/*********************************************************************
*
*       API, terminal I/O
*
**********************************************************************
*/

int               SYS_TERM_KbHit                  (void);

/*********************************************************************
*
*       API, file I/O
*
**********************************************************************
*/

void              SYS_FILE_Close                  (SYS_FILE_HANDLE hFile);
int               SYS_FILE_Delete                 (const char* sFile);
U32               SYS_FILE_GetAttributes          (const char* sFile);
U32               SYS_FILE_GetSize                (SYS_FILE_HANDLE hFile);
char              SYS_FILE_IsValid                (const char* sFile);
SYS_FILE_HANDLE   SYS_FILE_Open                   (const char* sFile, U32 Flags);
SYS_FILE_HANDLE   SYS_FILE_OpenEx                 (const char* sFile, U32 Flags, U32 NumRetries, U32 SleepMs);
int               SYS_FILE_Read                   (SYS_FILE_HANDLE hFile, void* pData, U32 NumBytes);
int               SYS_FILE_ReadLine               (SYS_FILE_HANDLE hFile, char* pData, U32 NumBytes);
int               SYS_FILE_Write                  (SYS_FILE_HANDLE hFile, const void* pData, U32 NumBytes);
SYS_FILE_HANDLE   SYS_FILE_CreateFile             (const char* sFile, U32 Flags);
int               SYS_FILE_Seek                   (SYS_FILE_HANDLE hFile, int offset, U32 fromPos);
int               SYS_FILE_Truncate               (SYS_FILE_HANDLE hFile, int length);
int               SYS_FILE_GetDates               (SYS_FILE_HANDLE hFile, SYS_TIME* pTimeAccess, SYS_TIME* pTimeModified);

SYS_FILE_HANDLE   SYS_FILE_CreateMapping          (SYS_FILE_HANDLE hFile, U64 MappedSize);
void              SYS_FILE_DeleteMapping          (SYS_FILE_HANDLE hMapFile);
void*             SYS_FILE_MapView                (SYS_FILE_HANDLE hMapFile, U64 Offset, U32 Size);
void              SYS_FILE_UnmapView              (void* pView, U64 Offset, U32 Size);
void              SYS_FILE_FlushView              (void* pView, U32 NumBytes);
int               SYS_FILE_Move                   (const char* sPathSrc, const char* sPathDest);

char              SYS_DIR_IsValid                 (const char* sDir);
int               SYS_DIR_RemovePath              (const char* sPath);
int               SYS_DIR_CreatePath              (const char* sPath);
int               SYS_DIR_GetConfigPath           (char* sPath, U32 NumBytes);
int               SYS_DIR_Iterate                 (const char* sPath, void* pPara, SYS_CB_ON_FILE_FOUND* pfCB);

/*********************************************************************
*
*       API, memory functions
*
**********************************************************************
*/

void              SYS_MEM_Free                   (void*  p);
void              SYS_MEM_FreePtr                (void** pp);
void*             SYS_MEM_Alloc                  (unsigned Size);
void*             SYS_MEM_AllocEx                (unsigned Size, const char* sName);
void*             SYS_MEM_AllocShared            (unsigned Size, const char* sName);
void*             SYS_MEM_Realloc                (void* p, unsigned Size);
U32               SYS_MEM_GetListAllocatedBlocks (SYS_ALLOC_ENTRY* pList, U32 MaxNumEntries);

/*********************************************************************
*
*       API, registry
*
**********************************************************************
*/

SYS_REG_HKEY      SYS_REG_OpenKeyEx               (SYS_REG_HKEY hKey, const char* sSubKey, U32 Flags);
SYS_REG_HKEY      SYS_REG_OpenKey                 (SYS_REG_HKEY hKey, const char* sSubKey);
SYS_REG_HKEY      SYS_REG_CreateKey               (SYS_REG_HKEY hKey, const char* sSubKey);
void              SYS_REG_CloseKey                (SYS_REG_HKEY hKey);
U32               SYS_REG_DeleteValue             (SYS_REG_HKEY hKey, const char* sRegValueName);
U32               SYS_REG_DeleteKey               (SYS_REG_HKEY hKey, const char* sRegKey);
U32               SYS_REG_SetValue                (SYS_REG_HKEY hKey, const char* sRegValueName, U32 Type, const void* pData, U32 NumBytes);
U32               SYS_REG_GetValue                (SYS_REG_HKEY hKey, const char* sRegValueName, U32* pType, void* pData, U32* pNumBytes);
int               SYS_REG_GetChildKey             (SYS_REG_HKEY hKey, U32 iChildKey, char* pBuf, U32 BufSize);

/*********************************************************************
*
*       API, critical sections
*
**********************************************************************
*/

SYS_HANDLE        SYS_CreateCriticalSection       (U32 SpinCount);
void              SYS_DeleteCriticalSection       (SYS_HANDLE hSection);
void              SYS_EnterCriticalSection        (SYS_HANDLE hSection);
void              SYS_LeaveCriticalSection        (SYS_HANDLE hSection);

/*********************************************************************
*
*       API, misc. functions
*
**********************************************************************
*/

U64               SYS_GetCurrentThreadId          (void);
U32               SYS_GetCurrentProcessId         (void);
int               SYS_GetListProcessId            (U64* paList, U32 MaxNumEntries);
U64               SYS_ConvertHPTime2us            (U64 t);
U64               SYS_GetHPTimeStamp              (void);
U64               SYS_GetHPTimeDiff_us            (U64 t0);
int               SYS_GetTickCount                (void);
int               SYS_GetNonNormalizedTickCount   (void);
U32               SYS_GetNumLogCores              (void);
void              SYS_GetLocalTime                (SYS_TIME* pTime);
void              SYS_GetSystemTime               (SYS_TIME* pTime);
int               SYS_GetDateFromString           (const char* sDate, U32* pDate, U32* pTime);
void              SYS_OutputDebugString           (const char *s);
void              SYS_OutputDebugStringf          (char *sFormat, ...);
void              SYS_Sleep                       (int ms);
SYS_HANDLE        SYS_CreateMutex                 (const char* sName);
void              SYS_CloseMutex                  (SYS_HANDLE* phMutex);
char              SYS_ReleaseMutex                (SYS_HANDLE hMutex);
U32               SYS_WaitForMutex                (SYS_HANDLE hObj, U32 TimeOut);
void              SYS_CloseHandle                 (SYS_HANDLE hObj);
void              SYS_timeBeginPeriod             (unsigned Period);
void              SYS_timeEndPeriod               (unsigned Period);
U32               SYS_GetLastError                (void);
SYS_HANDLE        SYS_CreateThread                (SYS_THREAD_PROC* pfThreadProc, U64* pThreadId, const char* sName);
SYS_HANDLE        SYS_CreateThreadEx              (SYS_THREAD_PROC_EX* pfThreadProc, void* pPara, U64* pThreadId, const char* sName, U32 Flags);
void              SYS_CloseThreadHandle           (SYS_HANDLE hObj);
int               SYS_ResumeThread                (SYS_HANDLE hThread);
void              SYS_SetThreadPriority           (SYS_HANDLE hThread, int Priority);
void              SYS_TerminateThread             (SYS_HANDLE hThread, int ExitCode);
int               SYS_GetExitCodeThread           (SYS_HANDLE hThread, int* pExitCode);
SYS_HANDLE        SYS_CreateEvent                 (int ManualReset, int InitialState);
void              SYS_SetEvent                    (SYS_HANDLE hEvent);
U32               SYS_WaitForEvent                (SYS_HANDLE hObj, U32 TimeOut);
void              SYS_CloseEvent                  (SYS_HANDLE hEvent);
SYS_HANDLE        SYS_CreateWaitableTimer         (const char* sName, int ManualReset);
void              SYS_SetWaitableTimer            (SYS_HANDLE hTimer, I64 DueTime, U32 Period, int Resume);
char              SYS_GetChar                     (void);
void              SYS_splitpath                   (const char* sPath, char* sDrive, U32 BufSizeDrive, char* sDir, U32 BufSizeDir, char* sName, U32 BufSizeName, char* sExt, U32 BufSizeExt);
void              SYS_makepath                    (char* sPath, U32 BufSize, const char* sDrive, const char* sDir, const char* sName, const char* sExt);
void              SYS_MakeAbsFilePath             (const char* sHomeDir, const char* sFileDir, char* pBuffer, int BufferSize);
void              SYS_SetModule                   (SYS_HANDLE hModule);
SYS_HANDLE        SYS_GetModule                   (void);
void              SYS_GetModuleFileName           (char* pBuffer, U32 BufferSize);
void              SYS_GetModulePath               (char* pBuffer, U32 BufferSize);
void              SYS_GetProcessFileName          (char* pBuffer, U32 BufferSize);
void              SYS_GetProcessPath              (char* pBuffer, U32 BufferSize);
U32               SYS_IsValidPath                 (const char* sPath);
void              SYS_ExtractExtension            (const char* sFileName, char* sBuffer, U32 BufferSize);
void              SYS_ExtractFilename             (const char* sFileName, char* sBuffer, U32 BufferSize);
void              SYS_GetDataPath                 (char* pBuffer, U32 BufferSize);
char*             SYS_ConsoleGetString            (const char * pPrompt, char * pBuffer, U32 BufferSize);
void              SYS_PreventSleep                (void);
void              SYS_AllowSleep                  (void);
void              SYS_GetRandStream               (void* pBuf, U32 NumBytes);
U32               SYS_GetOSVersion                (void);
int               SYS_CreateCLIProcess            (char* sName, char* sPath, char* sCmdLine, char* sWorkingDir, unsigned ShowWindow);
double            SYS_pow                         (double a, double b);
double            SYS_log10                       (double a);
double            SYS_fabs                        (double a);

void              SYS_Init                        (U32 CloseFDs);
void              SYS_Exit                        (void);
//
// Socket API (Requires SYS_Windows/Linux/MAC.c and UTIL.c)
//
SYS_SOCKET_HANDLE     SYS_SOCKET_OpenUDP              (void);
SYS_SOCKET_HANDLE     SYS_SOCKET_IPV6_OpenUDP         (char DualStack);
SYS_SOCKET_HANDLE     SYS_SOCKET_OpenTCP              (void);
SYS_SOCKET_HANDLE     SYS_SOCKET_IPV6_OpenTCP         (char DualStack);
void                  SYS_SOCKET_Close                (SYS_SOCKET_HANDLE hSocket);
void                  SYS_SOCKET_Shutdown             (SYS_SOCKET_HANDLE hSocket, int How);
void                  SYS_SOCKET_SetTimeouts          (SYS_SOCKET_HANDLE hSocket, int TimeoutSend, int TimeoutReceive);
int                   SYS_SOCKET_DisableLinger        (SYS_SOCKET_HANDLE hSocket);
int                   SYS_SOCKET_EnableLinger         (SYS_SOCKET_HANDLE hSocket, int TimeoutLinger);
int                   SYS_SOCKET_IsReadable           (SYS_SOCKET_HANDLE hSocket, int TimeoutMs);
int                   SYS_SOCKET_IsWriteable          (SYS_SOCKET_HANDLE hSocket, int TimeoutMs);
unsigned              SYS_SOCKET_IsReady              (SYS_SOCKET_HANDLE hSocket);
void                  SYS_SOCKET_EnableKeepalive      (SYS_SOCKET_HANDLE hSocket);
void                  SYS_SOCKET_SetBlocking          (SYS_SOCKET_HANDLE hSocket);
void                  SYS_SOCKET_SetNonBlocking       (SYS_SOCKET_HANDLE hSocket);
int                   SYS_SOCKET_Connect              (SYS_SOCKET_HANDLE hSocket, U32 IPAddr, U32 Port);
SYS_SOCKET_HANDLE     SYS_SOCKET_ConnectTCPWithTimeout(U32 IPAddr, U32 Port, int Timeout, SYS_CB_ON_CONNECT_INTERVAL* pfCheckForCancel);
int                   SYS_SOCKET_IPV6_Connect         (SYS_SOCKET_HANDLE hSocket, SYS_SOCKET_ADDR6 IPAddr, U32 Port);
int                   SYS_SOCKET_ListenAtTCPAddr      (SYS_SOCKET_HANDLE hSocket, U32 IPAddr, U32 Port, unsigned NumConnectionsQueued);
int                   SYS_SOCKET_IPV6_ListenAtTCPAddr (SYS_SOCKET_HANDLE hSocket, SYS_SOCKET_ADDR6 IPAddr, U32 Port, unsigned NumConnectionsQueued);
int                   SYS_SOCKET_Bind                 (SYS_SOCKET_HANDLE hSocket, U32 IPAddr, U32 LocalPort);
int                   SYS_SOCKET_IPV6_Bind            (SYS_SOCKET_HANDLE hSocket, SYS_SOCKET_ADDR6 IPAddr, U32 LocalPort);
SYS_SOCKET_HANDLE     SYS_SOCKET_Accept               (SYS_SOCKET_HANDLE hSocket);
SYS_SOCKET_HANDLE     SYS_SOCKET_AcceptEx             (SYS_SOCKET_HANDLE hSocket, int TimeoutMs);
int                   SYS_SOCKET_EnableBroadcast      (SYS_SOCKET_HANDLE hSocket);
int                   SYS_SOCKET_SetReceiveBufSize    (SYS_SOCKET_HANDLE hSocket, U32 BufSize);
int                   SYS_SOCKET_Send                 (SYS_SOCKET_HANDLE hSocket, const void* pData, U32 NumBytes);
int                   SYS_SOCKET_SendTo               (SYS_SOCKET_HANDLE hSocket, const void* pData, U32 NumBytes, U32 IPAddr, U32 DestPort);
int                   SYS_SOCKET_IPV6_SendTo          (SYS_SOCKET_HANDLE hSocket, const void* pData, U32 NumBytes, SYS_SOCKET_ADDR6 IPAddr, U32 DestPort);
int                   SYS_SOCKET_Receive              (SYS_SOCKET_HANDLE hSocket, void* pData, U32 MaxNumBytes);
int                   SYS_SOCKET_ReceiveAll           (SYS_SOCKET_HANDLE hSocket, void* pData, U32 NumBytes);
int                   SYS_SOCKET_UDPReceiveFrom       (SYS_SOCKET_HANDLE hSocket, void* pData, U32 MaxNumBytes, U32* pIPAddr, U32* pPort);
int                   SYS_SOCKET_IPV6_UDPReceiveFrom  (SYS_SOCKET_HANDLE hSocket, void* pData, U32 MaxNumBytes, SYS_SOCKET_ADDR6* IPAddr, U32* pPort);
int                   SYS_SOCKET_GetPeerName          (SYS_SOCKET_HANDLE hSocket, U32* pIPAddr, U32* pPort);
int                   SYS_SOCKET_IPV6_GetPeerName     (SYS_SOCKET_HANDLE hSocket, SYS_SOCKET_ADDR6* pIPAddr, U32* pPort);
int                   SYS_SOCKET_GetAddrInfo          (const char* sHostname, const char* sService, U32* pIPAddr);
int                   SYS_SOCKET_IPV6_GetAddrInfo     (const char* sHostname, const char* sService, SYS_SOCKET_ADDR6* pIPAddr, char AcceptMapped);
int                   SYS_SOCKET_GetNameInfo          (U32 IPAddr, char* sHostname, U32 HostnameLen, char* sService, U32 ServiceLen);
int                   SYS_SOCKET_IPV6_GetNameInfo     (SYS_SOCKET_ADDR6 IPAddr, char* sHostname, U32 HostnameLen, char* sService, U32 ServiceLen);
int                   SYS_SOCKET_GetLocalPort         (SYS_SOCKET_HANDLE hSocket, U32* pPort);
int                   SYS_SOCKET_SetReuseAddr         (SYS_SOCKET_HANDLE hSock);
int                   SYS_SOCKET_GetErrorStatus       (SYS_SOCKET_HANDLE hSock);
int                   SYS_SOCKET_GetSockName          (SYS_SOCKET_HANDLE hSocket, U32* pIPAddr);
//
// From SYS_SOCKET_UTIL.c (Requires SYS_Windows/Linux/MAC.c and UTIL.c)
//
int                   SYS_SOCKET_UTIL_FlushSendBuf    (SYS_SOCKET_INFO* pSocketInfo);
int                   SYS_SOCKET_UTIL_StoreSendData   (SYS_SOCKET_INFO* pSocketInfo, const void* pSrc, U32 NumBytesToWrite);
int                   SYS_SOCKET_UTIL_StoreSendData32 (SYS_SOCKET_INFO* pSocketInfo, U32 Data);
int                   SYS_SOCKET_UTIL_Read            (SYS_SOCKET_INFO* pSocketInfo, void* pDest, U32 NumBytesRem);
//
// SYS_IP API
//
int                   SYS_IP_GetAdaptersInfo          (SYS_IP_ADAPTER_INFO* paInfo, int MaxNumAdapters);
int                   SYS_IP_GetAdaptersInfoEx        (SYS_IP_ADAPTER_INFO* paInfo, int MaxNumAdapters, U32 Flags);
//
// COM API
//
U32                   SYS_COM_GetList                 (SYS_COM_INFO* paInfo, U32 MaxNumInfo);
SYS_COM_HANDLE        SYS_COM_Open                    (SYS_COM_INFO* pInfo, U32 Baudrate);
int                   SYS_COM_SetPins                 (SYS_COM_HANDLE hCOM, U32 Pins);
int                   SYS_COM_ClrPins                 (SYS_COM_HANDLE hCOM, U32 Pins);
int                   SYS_COM_GetPinStates            (SYS_COM_HANDLE hCOM);
int                   SYS_COM_SetPinStates            (SYS_COM_HANDLE hCOM, U32 Mask);
void                  SYS_COM_SetBaudrate             (SYS_COM_HANDLE hCOM, U32 Baudrate);
int                   SYS_COM_Read                    (SYS_COM_HANDLE hCOM, void* pData, U32 NumBytes, U32 Timeout);
int                   SYS_COM_Write                   (SYS_COM_HANDLE hCOM, const void* pData, U32 NumBytes, U32 Timeout);
void                  SYS_COM_Close                   (SYS_COM_HANDLE hCOM);

#if defined(__cplusplus)          // Allow usage of this module from C++ files (disable name mangling)
  }
#endif

#endif                            // Avoid multiple inclusion

/*************************** End of file ****************************/
