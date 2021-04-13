/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (C) 2004 - 2005    SEGGER Microcontroller Systeme GmbH      *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : JLinkARMDLL.h
Purpose : "C"- level interface of <JLinkARM.dll>
---------------------------END-OF-HEADER------------------------------
*/

#ifndef JLINKARM_CONST_H            //  Avoid multiple inclusion
#define JLINKARM_CONST_H

#ifdef _WIN32
  #ifndef WIN32
    #define WIN32
  #endif
#endif

#include "TYPES.h"

#ifdef WIN32
  #define STDCALL __stdcall
#else
  #define STDCALL
#endif

/*********************************************************************
*
*       Defines required for API
*
**********************************************************************
*/

//
// SiLabs EFM8 (8051) virtual addresses
//
// J-Link virtually maps the different memory types of the SiLabs EFM8 (8051) to different 16 MB areas
// Do NOT change after C2 support has been released, since DLL + Firmware rely on this layout
// Layout:
//   Area0: CODE
//   Area1: IDATA
//   Area2: DDATA
//   Area3: XDATA
//   Area4: DSR
//
#define JLINK_EFM8_VIRTUAL_AREA_SIZE         (0x1000000)

#define JLINK_EFM8_START_ADDR_CODE           (0x0000000)                                                   // 64KB. First 32KB are fixed, the other 32KB are the current bank
#define JLINK_EFM8_START_ADDR_IDATA          (JLINK_EFM8_START_ADDR_CODE + JLINK_EFM8_VIRTUAL_AREA_SIZE)   // 256 bytes
#define JLINK_EFM8_START_ADDR_DDATA          (JLINK_EFM8_START_ADDR_IDATA + JLINK_EFM8_VIRTUAL_AREA_SIZE)  // 256 bytes  00-7F is RAM, with register banks as usual. 80-FF is the sfr area of the currently selected bank
#define JLINK_EFM8_START_ADDR_XDATA          (JLINK_EFM8_START_ADDR_DDATA + JLINK_EFM8_VIRTUAL_AREA_SIZE)
#define JLINK_EFM8_START_ADDR_DSR            (JLINK_EFM8_START_ADDR_XDATA + JLINK_EFM8_VIRTUAL_AREA_SIZE)  // Virtually 64 KB. Maps non-memory mapped DSR registers to virtual memory, to allow easy access to them
#define JLINK_EFM8_START_ADDR_C2             (JLINK_EFM8_START_ADDR_DSR + JLINK_EFM8_VIRTUAL_AREA_SIZE)    // 256 bytes. Allows memory accesses directly via C2
//
// Explicit access to different DDATA pages
//
#define JLINK_EFM8_START_ADDR_DDATA_PAGE0    (JLINK_EFM8_START_ADDR_DDATA + 0x00100)                       // 256 bytes. Same as DDATA, only sfrs are page-0 sfrs (no matter what SFRPAGE register currently selects)
#define JLINK_EFM8_START_ADDR_DDATA_PAGE1    (JLINK_EFM8_START_ADDR_DDATA + 0x00200)                       // 256 bytes. Same as DDATA, only sfrs are page-1 sfrs (no matter what SFRPAGE register currently selects)
#define JLINK_EFM8_START_ADDR_DDATA_PAGE2    (JLINK_EFM8_START_ADDR_DDATA + 0x00300)                       // 256 bytes. Same as DDATA, only sfrs are page-2 sfrs (no matter what SFRPAGE register currently selects)
// [...] DDATA_PAGE2-xxx
//
// Explicit access to different flash areas
// Allows debugger to perform a continuous flash download via a single memory-write
//
#define JLINK_EFM8_START_ADDR_CODE_BANK0     (JLINK_EFM8_START_ADDR_CODE + 0x10000)                        // 32KB, Always visible at 0x30000. Also be visible at 0x38000 if PSBANK == 0x00
#define JLINK_EFM8_START_ADDR_CODE_BANK1     (JLINK_EFM8_START_ADDR_CODE + 0x18000)                        // 32KB, Also be visible at 0x38000 if PSBANK == 0x11
#define JLINK_EFM8_START_ADDR_CODE_BANK2     (JLINK_EFM8_START_ADDR_CODE + 0x20000)                        // 32KB, Also visible at 0x38000 if PSBANK register == 0x22
//[...] CODE_BANK3-xxx

//
// Virtual offsets of non-memory mapped DSR registers, into DSR zone
//
#define JLINK_EFM8_OFF_REG_DSR_VERSION       (0x00)
#define JLINK_EFM8_OFF_REG_DSR_DERIVATIVE    (0x01)

#define JLINK_EFM8_FLACL_FORMAT_LOCKBYTE_COMPLEMENT                     (0)
#define JLINK_EFM8_FLACL_FORMAT_LOCKBYTE                                (1)

#define JLINK_EFM8_MEM_ACC_TYPE_D     (0)
#define JLINK_EFM8_MEM_ACC_TYPE_I     (1)
#define JLINK_EFM8_MEM_ACC_TYPE_C2    (2)


#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_DSRCMD                 ( 0)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BPENA                  ( 1)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP0L                   ( 2)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP0H                   ( 3)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP1L                   ( 4)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP1H                   ( 5)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP2L                   ( 6)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP2H                   ( 7)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP3L                   ( 8)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP3H                   ( 9)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP4L                   (10)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP4H                   (11)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP5L                   (12)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP5H                   (13)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP6L                   (14)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP6H                   (15)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP7L                   (16)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP7H                   (17)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_R0B0             (18)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_R1B0             (19)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_R2B0             (20)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_DPTR_L           (21)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_DPTR_H           (22)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_PSW              (23)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_A                (24)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_SECTOR_SIZE_SHIFT           (25)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_PSBANK                 (26)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_SFRPAGE                (27)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_XRAM_ACC_MODE               (28)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_XADRH                  (29)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_XADRL                  (30)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_XDATA                  (31)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_FLSCL                  (32)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_RSTSRC                 (33)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_VDM0CN                 (34)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_NUM_SECTORS                 (35)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_FLACL                  (36)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_FLACL_SFRPAGE               (37)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_FLACL_FORMAT                (38)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDRL_DFLASH_START          (39)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDRH_DFLASH_START          (40)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DFLASH_SECTOR_SIZE_SHIFT    (41)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_SFRPAGE          (42)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_PSCTL                  (43)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_PSCTL            (44)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_CLKSEL                 (45)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_CLKSEL           (46)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_FLKEY                  (47)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_FLKEY            (48)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR0_ADDR             (49)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR0_DEBUG_ADDR       (50)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR0_ACC_TYPE         (51)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR1_ADDR             (52)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR1_DEBUG_ADDR       (53)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR1_ACC_TYPE         (54)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR2_ADDR             (55)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR2_DEBUG_ADDR       (56)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR2_ACC_TYPE         (57)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR3_ADDR             (58)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR3_DEBUG_ADDR       (59)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR3_ACC_TYPE         (60)
//
// Microchip BT5511 (8051) virtual addresses
//
// J-Link virtually maps the different memory types different 16 MB areas
// Do NOT change after BT5511 support has been released, since DLL + Firmware rely on this layout
// Layout:
//   Area0: CODE
//   Area1: IDATA
//   Area2: DDATA
//   Area3: XDATA
//
#define JLINK_BT5511_VIRTUAL_AREA_SIZE       (0x1000000)
#define JLINK_BT5511_START_ADDR_CODE         (0x0000000)                                                       // 16 MB
#define JLINK_BT5511_START_ADDR_IDATA        (JLINK_BT5511_START_ADDR_CODE + JLINK_BT5511_VIRTUAL_AREA_SIZE)   // 512 bytes (upper 256 bytes are available through SP accesses only) (higher addresses will be aliased to 0x000 - 0x1FF)
#define JLINK_BT5511_START_ADDR_DDATA        (JLINK_BT5511_START_ADDR_IDATA + JLINK_BT5511_VIRTUAL_AREA_SIZE)  // 256 bytes (higher addresses will be aliased to 0x000 - 0x0FF)
#define JLINK_BT5511_START_ADDR_XDATA        (JLINK_BT5511_START_ADDR_DDATA + JLINK_BT5511_VIRTUAL_AREA_SIZE)  // 16 MB (24-bit DPTR)
//
// Memory flags (32-bit, passed to ReadMemEx(), WriteMemEx(), ...)
//
// Flags  [31:30] Security      See JLINK_MEM_ACC_SEC_
//        [29:27] Cacheability  See JLINK_MEM_ACC_CACHE_
//        [26:25] Privilege     See JLINK_MEM_ACC_PRIV_
//        [24:24] Addr. inc.    See JLINK_MEM_ACC_INC_
//        [23:21] Type          See JLINK_MEM_ACC_TYPE_
//        [20:13] Mode parameters Actual meaning depends on mode + connected CPU. See JLINK_MEM_ACC_TYPE_ for specific CPUs for more information
//        [12:8]  AddFlags      CPU specific
//        [7:0]   AccessWidth   See JLINK_MEM_ACC_WIDTH_  => Have been there all the time
//
//
// Security related memory access attributes
//
#define JLINK_MEM_ACC_SEC_DEFAULT           (0) // Default
#define JLINK_MEM_ACC_SEC_FORCE_SECURE      (1) // Force secure access
#define JLINK_MEM_ACC_SEC_FORCE_NON_SECURE  (2) // Force unsecure access
//
// Cache related memory access attributes
//
#define JLINK_MEM_ACC_CACHE_DEFAULT         (0) // Default
#define JLINK_MEM_ACC_CACHE_FORCE_WC        (1) // Indicates access to write-cacheable memory
#define JLINK_MEM_ACC_CACHE_FORCE_RC        (2) // Indicates access to read-cacheable memory
#define JLINK_MEM_ACC_CACHE_FORCE_RWC       (3) // Indicates access to write-read-cacheable memory
#define JLINK_MEM_ACC_CACHE_FORCE_NC        (4) // Indicates access to non-cacheable memory
//
// Privilege related memory access attributes
//
#define JLINK_MEM_ACC_PRIV_DEFAULT          (0)  // Default
#define JLINK_MEM_ACC_PRIV_PRIVILEDGED      (1)  // Force privileged
#define JLINK_MEM_ACC_PRIV_UNPRIVILEDGE     (2)  // Force unprivileged
//
// Addr increment related memory access attributes
//
#define JLINK_MEM_ACC_INC_DEFAULT           (0)  // Default
#define JLINK_MEM_ACC_INC_NOADDRINC         (1)  // No Auto-increment (Same addr. is read for all requested items)
//
// Memory access type
// 0 & 1 are generic, 2-7 are core specific
//
#define JLINK_MEM_ACC_TYPE_DEFAULT          (0)
#define JLINK_MEM_ACC_TYPE_0                (0)
#define JLINK_MEM_ACC_TYPE_CPU              (1)
#define JLINK_MEM_ACC_TYPE_1                (1)
#define JLINK_MEM_ACC_TYPE_2                (2)
#define JLINK_MEM_ACC_TYPE_3                (3)
#define JLINK_MEM_ACC_TYPE_4                (4)
#define JLINK_MEM_ACC_TYPE_5                (5)
#define JLINK_MEM_ACC_TYPE_6                (6)
#define JLINK_MEM_ACC_TYPE_7                (7)
//
// Cortex specific modes
//
#define JLINK_MEM_ACC_TYPE_CORTEX_PRECONFIG_AHBAP   (2)
#define JLINK_MEM_ACC_TYPE_CORTEX_PRECONFIG_APBAP   (3)
#define JLINK_MEM_ACC_TYPE_CORTEX_MEMAP             (4)   // Access via MEM-AP. Index of AP to be used is specified by Flags[20:13]
//
// Access width
//
#define JLINK_MEM_ACC_WIDTH_DEFAULT         (0) // Whatever works best
#define JLINK_MEM_ACC_WIDTH_8BIT            (1) // Force 8-bit accesses
#define JLINK_MEM_ACC_WIDTH_16BIT           (2) // Force 16-bit accesses
#define JLINK_MEM_ACC_WIDTH_32BIT           (4) // Force 32-bit accesses
#define JLINK_MEM_ACC_WIDTH_64BIT           (8) // Force 64-bit accesses
//
// Position of memory attribute groups inside 32-bit <Flags> parameter
//
#define JLINK_MEM_ACC_SEC_POS              (30)
#define JLINK_MEM_ACC_CACHE_POS            (27)
#define JLINK_MEM_ACC_PRIV_POS             (25)
#define JLINK_MEM_ACC_INC_POS              (24)
#define JLINK_MEM_ACC_TYPE_POS             (21)
#define JLINK_MEM_ACC_MODEPARAM_POS        (13)
#define JLINK_MEM_ACC_WIDTH_POS             (0)
//
// Mask for memory attribute groups
//
#define JLINK_MEM_ACC_SEC_MASK              (0x3)
#define JLINK_MEM_ACC_CACHE_MASK            (0x7)
#define JLINK_MEM_ACC_PRIV_MASK             (0x3)
#define JLINK_MEM_ACC_INC_MASK              (0x1)
#define JLINK_MEM_ACC_TYPE_MASK             (0x7)
#define JLINK_MEM_ACC_MODEPARAM_MASK        (0xFF)
#define JLINK_MEM_ACC_WIDTH_MASK            (0xFF)

/*********************************************************************
*
*       Emulator product IDs
*
*/
#define JLINK_EMU_PRODUCT_ID_UNKNOWN            (0x00)
#define JLINK_EMU_PRODUCT_ID_JLINK              (0x01)
#define JLINK_EMU_PRODUCT_ID_JLINK_CF           (0x02)
#define JLINK_EMU_PRODUCT_ID_JLINK_CE           (0x03)
#define JLINK_EMU_PRODUCT_ID_JLINK_KS           (0x04)
#define JLINK_EMU_PRODUCT_ID_DIGI_LINK          (0x05)
#define JLINK_EMU_PRODUCT_ID_MIDAS              (0x06)
#define JLINK_EMU_PRODUCT_ID_SAMICE             (0x07)
#define JLINK_EMU_PRODUCT_ID_JTRACE             (0x08)
#define JLINK_EMU_PRODUCT_ID_JTRACE_CS          (0x09)
#define JLINK_EMU_PRODUCT_ID_FLASHER_ARM        (0x0A)
#define JLINK_EMU_PRODUCT_ID_JLINK_PRO          (0x0B)
#define JLINK_EMU_PRODUCT_ID_JLINK_EDU          (0x0C)
#define JLINK_EMU_PRODUCT_ID_JLINK_ULTRA        (0x0D)
#define JLINK_EMU_PRODUCT_ID_FLASHER_PPC        (0x0E)
#define JLINK_EMU_PRODUCT_ID_FLASHER_RX         (0x0F)
#define JLINK_EMU_PRODUCT_ID_JLINK_OB_RX200     (0x10)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE         (0x11)
#define JLINK_EMU_PRODUCT_ID_JLINK_OB_SAM3U128  (0x12)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_CORTEXM (0x13)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_LPC     (0x14)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_STM32   (0x15)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_FSL     (0x16)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_ADI     (0x17)
#define JLINK_EMU_PRODUCT_ID_ENERGY_MICRO_EFM32 (0x18)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_XMC4000 (0x19)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_XMC4200 (0x20)
#define JLINK_EMU_PRODUCT_ID_JLINK_LPC_LINK2    (0x21)
#define JLINK_EMU_PRODUCT_ID_FLASHER_PRO        (0x22)
#define JLINK_EMU_PRODUCT_ID_FLASHER_COMPACT    (0x28)
#define JLINK_EMU_PRODUCT_ID_JTRACE_PRO         (0xFE)

/*********************************************************************
*
*       Global error codes
*
*  The error codes which are listed here can be returned by
*  any DLL API-function which has a return value.
*/
#define JLINK_ERR_EMU_NO_CONNECTION            -256 // (0xFFFFFF00) No connection to emulator / Connection to emulator lost
#define JLINK_ERR_EMU_COMM_ERROR               -257 // (0xFFFFFEFF) Emulator communication error (host-interface module reproted error)
#define JLINK_ERR_DLL_NOT_OPEN                 -258 // (0xFFFFFEFE) DLL has not been opened but needs to be (JLINKARM_Open() needs to be called first)
#define JLINK_ERR_VCC_FAILURE                  -259 // (0xFFFFFEFD) Target system has no power (Measured VTref < 1V)
#define JLINK_ERR_INVALID_HANDLE               -260 // (0xFFFFFEFC) File handle / memory area handle needed for operation, but given handle is not valid
#define JLINK_ERR_NO_CPU_FOUND                 -261 // (0xFFFFFEFB) Could not find supported CPU
#define JLINK_ERR_EMU_FEATURE_NOT_SUPPORTED    -262 // (0xFFFFFEFA) Emulator does not support the selected feature (Usually returned by functions which need specific emulator capabilities)
#define JLINK_ERR_EMU_NO_MEMORY                -263 // (0xFFFFFEF9) Emulator does not have enough memory to perform the requested operation
#define JLINK_ERR_TIF_STATUS_ERROR             -264 // (0xFFFFFEF8) Things such as "TCK is low but should be high"
#define JLINK_ERR_FLASH_PROG_COMPARE_FAILED    -265
#define JLINK_ERR_FLASH_PROG_PROGRAM_FAILED    -266
#define JLINK_ERR_FLASH_PROG_VERIFY_FAILED     -267
#define JLINK_ERR_OPEN_FILE_FAILED             -268
#define JLINK_ERR_UNKNOWN_FILE_FORMAT          -269
#define JLINK_ERR_WRITE_TARGET_MEMORY_FAILED   -270
#define JLINK_ERR_DEVICE_FEATURE_NOT_SUPPORTED -271 // Connected device does not support specific feature
#define JLINK_ERR_WRONG_USER_CONFIG            -272 // User configured DLL parameters incorrectly
#define JLINK_ERR_NO_TARGET_DEVICE_SELECTED    -273 // User did not specify the core / device J-Link shall connect to
#define JLINK_ERR_CPU_IN_LOW_POWER_MODE        -274

/*********************************************************************
*
*       Memory access errors
*/
#define JLINKARM_ERR_CPU_NO_CLOCK  0x80000001
#define JLINKARM_ERR_CPU_NO_POWER  0x80000002

/*********************************************************************
*
*       JLINKARM_FLASH_ API error codes
*/
#define JLINK_FLASH_ERR_BLOCK_VERIFICATION_ERROR 1
#define JLINK_FLASH_ERR_ITEM_VERIFICATION_ERROR  2
#define JLINK_FLASH_ERR_TIMEOUT                  3
#define JLINK_FLASH_ERR_PROGRAM_ERROR            4
#define JLINK_FLASH_ERR_PROGRAM_1_OVER_0         5
#define JLINK_FLASH_ERR_SECTOR_IS_LOCKED         6
#define JLINK_FLASH_ERR_ERASE_ERROR              7
#define JLINK_FLASH_ERR_NO_FLASH_MEMORY          8
#define JLINK_FLASH_ERR_GENERIC_ERROR            9
#define JLINK_FLASH_ERR_ALGO_SPECIFIC_ERROR     -2
#define JLINK_FLASH_ERR_NO_FLASH_BANK           -3
#define JLINK_FLASH_ERR_PROGRAM_DOES_NOT_FIT    -4

/*********************************************************************
*
*       Pin status
*/
#define JLINKARM_PIN_STATUS_LOW       0
#define JLINKARM_PIN_STATUS_HIGH      1
#define JLINKARM_PIN_STATUS_UNKNOWN   255

/*********************************************************************
*
*       PIN override functionalities
*/
#define JLINK_PIN_OVERRIDE_MODE_RELEASE         0  // Release pin from override mode
#define JLINK_PIN_OVERRIDE_MODE_PIO_IN          1
#define JLINK_PIN_OVERRIDE_MODE_PIO_OUT_LOW     2
#define JLINK_PIN_OVERRIDE_MODE_PIO_OUT_HIGH    3
#define JLINK_PIN_OVERRIDE_MODE_UART_TX         4
#define JLINK_PIN_OVERRIDE_MODE_UART_RX         5
#define JLINK_PIN_OVERRIDE_MODE_UART_RXTX       6  // Half-duplex UART where Rx and Tx are on the same pin

/*********************************************************************
*
*       Interfaces
*/
#define JLINKARM_TIF_JTAG              0
#define JLINKARM_TIF_SWD               1
#define JLINKARM_TIF_BDM3              2  // Do NOT use. Not supported anymore. Only there for backward compatbility inside the DLL
#define JLINKARM_TIF_FINE              3
#define JLINKARM_TIF_ICSP              4  // Microchip 2-wire JTAG via TCK + TMS (e.g. PIC32)
#define JLINKARM_TIF_SPI               5
#define JLINKARM_TIF_C2                6
#define JLINKARM_TIF_CJTAG             7
#define JLINKARM_TIF_SWIM              8   // Only used Flasher PRO/ATE internally. J-Link does not support SWIM interface (yet)
#define JLINKARM_TIF_PDI               9   // Only used Flasher PRO/ATE internally. J-Link does not support PDI interface (yet)
#define JLINKARM_TIF_MC2WJTAG_TDI      10  // Microchip 2-wire JTAG via TCK + TDI (e.g. BT5511 8051 core)
#define JLINKARM_TIF_SPI_IDLE_CLK_LOW  11  // Microchip 2-wire JTAG via TCK + TDI (e.g. ATMega)
#define JLINKARM_TIF_I2C               12  // Only used Flasher PRO/ATE internally. J-Link does not support I2C interface (yet)
#define JLINKARM_TIF_SPI2FE            13  // Only used Flasher PRO/ATE internally. J-Link does not support SPI2FE interface (yet)
#define JLINKARM_TIF_QSPI              14  // Currently, only supported by Flasher PRO
#define JLINKARM_TIF_NUMTIFS           15  // Increment when adding a new interface
//
// Options for command string "SetcJTAGInitMode"
// Note: When adding new ones here, also add them to JLINKARM_Exec to be accepted as symbolic values
//
#define JLINK_CJTAG_INIT_LONG_ACT_SEQ   (0)  // Long-form activation sequence with JScan0 boot & OScan1 enter afterwards (default)
#define JLINK_CJTAG_INIT_SHORT_ACT_SEQ  (1)  // Short-form activation sequence with OScan1 boot (Needed for SiFive RISC-V targets)
#define JLINK_CJTAG_INIT_WILIOT_ACT_SEQ (2)  // Short-form activation sequence with OScan1 boot + special for Wiliot chips

/*********************************************************************
*
*       HSS flags that can be passed on Start()
*       Max. 32 Flags
*/
#define JLINK_HSS_FLAG_TIMESTAMP_US           (1uL << 0)

/*********************************************************************
*
*       PIN defines
*/
#define JLINK_PIN_MAX_NUM_PINS                    (8)

/*********************************************************************
*
*       SPI flags
*/
#define JLINK_SPI_FLAG_MASK_CS_START_STATE        (3 << 0)
#define JLINK_SPI_FLAG_MASK_CS_END_STATE          (3 << 2)
#define JLINK_SPI_FLAG_MASK_START_MODE            (3 << 8)
#define JLINK_SPI_FLAG_MASK_END_MODE              (3 << 10)
#define JLINK_SPI_FLAG_MASK_HANDLE_CS_ON_REPEATS  (1 << 4)
#define JLINK_SPI_FLAG_MASK_CS_INDEX              (3 << 5)
#define JLINK_SPI_FLAG_SHIFT_CS_INDEX             (5)
#define JLINK_SPI_FLAG_CS_START_STATE_U           (0 << 0)  // nCS not touched at start of transfer
#define JLINK_SPI_FLAG_CS_START_STATE_0           (2 << 0)  // nCS == LOW at start of transfer
#define JLINK_SPI_FLAG_CS_START_STATE_1           (3 << 0)  // nCS == HIGH at start of transfer
#define JLINK_SPI_FLAG_CS_END_STATE_U             (0 << 2)  // nCS not touched at end of transfer
#define JLINK_SPI_FLAG_CS_END_STATE_0             (2 << 2)  // nCS == LOW at end of transfer
#define JLINK_SPI_FLAG_CS_END_STATE_1             (3 << 2)  // nCS == HIGH at end of transfer
#define JLINK_SPI_FLAG_HANDLE_CS_ON_REPEATS       (1 << 4)  // Start/End state of nCS applied on each retry when using TransferUntil()
#define JLINK_SPI_FLAG_CS_INDEX_0                 (0 << 5)  // [7:5] == Index of nCS to drive
#define JLINK_SPI_FLAG_CS_INDEX_1                 (1 << 5)  // [7:5] == Index of nCS to drive
#define JLINK_SPI_FLAG_CS_INDEX_2                 (2 << 5)  // [7:5] == Index of nCS to drive
#define JLINK_SPI_FLAG_CS_INDEX_3                 (3 << 5)  // [7:5] == Index of nCS to drive
#define JLINK_SPI_FLAG_START_MODE_U               (0 << 8)  // [9:8] Do not change SPI mode at start of transfer
#define JLINK_SPI_FLAG_START_MODE_S               (1 << 8)  // [9:8] Set mode to single at start of transfer
#define JLINK_SPI_FLAG_START_MODE_Q               (2 << 8)  // [9:8] Set mode to quad at start of transfer
#define JLINK_SPI_FLAG_END_MODE_U                 (0 << 10) // [11:10] Do not change SPI mode at end of transfer
#define JLINK_SPI_FLAG_END_MODE_S                 (1 << 10) // [11:10] Set mode to single at end of transfer
#define JLINK_SPI_FLAG_END_MODE_Q                 (2 << 10) // [11:10] Set mode to quad at end of transfer

#define JLINK_SPI_CMD_TRANSFER                    (1)
#define JLINK_SPI_CMD_TRANSFER_UNTIL              (2)

/*********************************************************************
*
*       STRACE commands
*/
#define JLINK_STRACE_CMD_SET_TRACE_EVENT          (0)
#define JLINK_STRACE_CMD_CLR_TRACE_EVENT          (1)
#define JLINK_STRACE_CMD_CLR_ALL_TRACE_EVENTS     (2)
#define JLINK_STRACE_CMD_SET_BUFF_SIZE            (3)

/*********************************************************************
*
*       STRACE event and operation specifiers
*/
#define JLINK_STRACE_EVENT_TYPE_CODE_FETCH  (0)
#define JLINK_STRACE_EVENT_TYPE_DATA_ACC    (1)
#define JLINK_STRACE_EVENT_TYPE_DATA_LOAD   (2)
#define JLINK_STRACE_EVENT_TYPE_DATA_STORE  (3)

#define JLINK_STRACE_OP_TRACE_START         (0)
#define JLINK_STRACE_OP_TRACE_STOP          (1)
#define JLINK_STRACE_OP_TRACE_INCLUDE_RANGE (2)
#define JLINK_STRACE_OP_TRACE_EXCLUDE_RANGE (3)

/*********************************************************************
*
*       Device families
*/
#define JLINKARM_DEV_FAMILY_AUTO          0
#define JLINKARM_DEV_FAMILY_CM1           1     // Cortex-M1
#define JLINKARM_DEV_FAMILY_CF            2     // ColdFire
#define JLINKARM_DEV_FAMILY_CM3           3     // Cortex-M3
#define JLINKARM_DEV_FAMILY_SIM           4     // Simulator
#define JLINKARM_DEV_FAMILY_XSCALE        5     // XScale
#define JLINKARM_DEV_FAMILY_CM0           6     // Cortex-M0
#define JLINKARM_DEV_FAMILY_ARM7          7     // ARM7
#define JLINKARM_DEV_FAMILY_CORTEX_A8     8     // Cortex-A8 / Cortex-A9
#define JLINKARM_DEV_FAMILY_ARM9          9     // ARM9
#define JLINKARM_DEV_FAMILY_ARM10        10     // ARM10
#define JLINKARM_DEV_FAMILY_ARM11        11     // ARM11
#define JLINKARM_DEV_FAMILY_CORTEX_R4    12     // Cortex-R4
#define JLINKARM_DEV_FAMILY_RX           13     // Renesas RX core
#define JLINKARM_DEV_FAMILY_CM4          14     // Cortex-M4
#define JLINKARM_DEV_FAMILY_CORTEX_A5    15     // Cortex-A5
#define JLINKARM_DEV_FAMILY_POWER_PC     16     // PowerPC
#define JLINK_DEV_FAMILY_MIPS            17     // MIPS architecture
#define JLINK_DEV_FAMILY_EFM8            18     // SiLabs EFM8 (8-bit 8051 derivates)
#define JLINK_DEV_FAMILY_RISC_V          19     // RISC-V
#define JLINK_DEV_FAMILY_CORTEX_AR_ARMV8 0x14   // ARMv8A/R (A53, A57, A72, ...)
#define JLINK_DEV_FAMILY_BT5511          0x15   // Microchip BT5511 8051 related cores
#define JLINKARM_DEV_FAMILY_ANY         255

/*********************************************************************
*
*       Cores
*/
#define JLINK_CORE_NONE             0x00000000
#define JLINK_CORE_ANY              0xFFFFFFFF
#define JLINK_CORE_CORTEX_M1        0x010000FF
#define JLINK_CORE_COLDFIRE         0x02FFFFFF
#define JLINK_CORE_CORTEX_M3        0x030000FF
#define JLINK_CORE_CORTEX_M3_R1P0   0x03000010
#define JLINK_CORE_CORTEX_M3_R1P1   0x03000011
#define JLINK_CORE_CORTEX_M3_R2P0   0x03000020
#define JLINK_CORE_CORTEX_M3_R2P1   0x03000021
#define JLINK_CORE_SIM              0x04FFFFFF
#define JLINK_CORE_XSCALE           0x05FFFFFF
#define JLINK_CORE_CORTEX_M0        0x060000FF
#define JLINK_CORE_CORTEX_M23       0x060100FF
#define JLINK_CORE_ARM7             0x07FFFFFF
#define JLINK_CORE_ARM7TDMI         0x070000FF
#define JLINK_CORE_ARM7TDMI_R3      0x0700003F
#define JLINK_CORE_ARM7TDMI_R4      0x0700004F
#define JLINK_CORE_ARM7TDMI_S       0x070001FF
#define JLINK_CORE_ARM7TDMI_S_R3    0x0700013F
#define JLINK_CORE_ARM7TDMI_S_R4    0x0700014F
//
// Order of Cortex-A cores looks unfortunate but we cannot change it anymore
// since we need to stay binary compatible
//
#define JLINK_CORE_CORTEX_A8        0x080000FF
#define JLINK_CORE_CORTEX_A7        0x080800FF  // Same family as Cortex-A9. Only low-level differences
#define JLINK_CORE_CORTEX_A9        0x080900FF  // Cortex-A9. Cortex-A8 compatible only small differences for future multi-core debugging support.
#define JLINK_CORE_CORTEX_A12       0x080A00FF  // Same family as Cortex-A9. Only low-level differences
#define JLINK_CORE_CORTEX_A15       0x080B00FF  // Same family as Cortex-A9. Only low-level differences
#define JLINK_CORE_CORTEX_A17       0x080C00FF  // Same family as Cortex-A9. Only low-level differences
#define JLINK_CORE_ARM9             0x09FFFFFF
#define JLINK_CORE_ARM9TDMI_S       0x090001FF
#define JLINK_CORE_ARM920T          0x092000FF
#define JLINK_CORE_ARM922T          0x092200FF
#define JLINK_CORE_ARM926EJ_S       0x092601FF
#define JLINK_CORE_ARM946E_S        0x094601FF
#define JLINK_CORE_ARM966E_S        0x096601FF
#define JLINK_CORE_ARM968E_S        0x096801FF
#define JLINK_CORE_ARM11            0x0BFFFFFF
#define JLINK_CORE_ARM1136          0x0B36FFFF
#define JLINK_CORE_ARM1136J         0x0B3602FF
#define JLINK_CORE_ARM1136J_S       0x0B3603FF
#define JLINK_CORE_ARM1136JF        0x0B3606FF
#define JLINK_CORE_ARM1136JF_S      0x0B3607FF
#define JLINK_CORE_ARM1156          0x0B56FFFF
#define JLINK_CORE_ARM1176          0x0B76FFFF
#define JLINK_CORE_ARM1176J         0x0B7602FF
#define JLINK_CORE_ARM1176J_S       0x0B7603FF
#define JLINK_CORE_ARM1176JF        0x0B7606FF
#define JLINK_CORE_ARM1176JF_S      0x0B7607FF
#define JLINK_CORE_CORTEX_R4        0x0C0000FF  // Device family: 0x0D => Cortex-R, sub-family 0x00: Cortex-R4
#define JLINK_CORE_CORTEX_R5        0x0C0100FF  // Device family: 0x0D => Cortex-R, sub-family 0x01: Cortex-R5
#define JLINK_CORE_CORTEX_R8        0x0C0200FF  // Device family: 0x0D => Cortex-R, sub-family 0x02: Cortex-R8
//
// RX is somewhat difficult
// We need to distinguish between them on a low-level-core basis because handling is different for some families
// This is why there is not a single "JLINK_CORE_RX" define
//
#define JLINK_CORE_RX               0x0DFFFFFF  // Device family: 0x0D, sub family not specified, revision not specified
// RX61x, RX62x, RX63x sub family
#define JLINK_CORE_RX610            0x0D00FFFF  // Device family: 0x0D, sub family 0x00, revision not specified
#define JLINK_CORE_RX62N            0x0D01FFFF  // Device family: 0x0D, sub family 0x01, revision not specified
#define JLINK_CORE_RX62T            0x0D02FFFF
#define JLINK_CORE_RX63N            0x0D03FFFF
#define JLINK_CORE_RX630            0x0D04FFFF
#define JLINK_CORE_RX63T            0x0D05FFFF
#define JLINK_CORE_RX621            0x0D06FFFF
#define JLINK_CORE_RX62G            0x0D07FFFF
#define JLINK_CORE_RX631            0x0D08FFFF
#define JLINK_CORE_RX65N            0x0D09FFFF
#define JLINK_CORE_RX66T            0x0D0AFFFF
#define JLINK_CORE_RX72T            0x0D0BFFFF
#define JLINK_CORE_RX66N            0x0D0CFFFF  // According to the documentation the ID of 66N should be 0x0D0CFFFF. However a customer error suggests it has the same Core ID as RX72M (0x0D0DFFFF). Unfortuneatly we cannot verify this yet since we do not have any hardware at hand.
#define JLINK_CORE_RX72M            0x0D0DFFFF
#define JLINK_CORE_RX72N            0x0D0EFFFF
// RX2xx sub family
#define JLINK_CORE_RX210            0x0D10FFFF
#define JLINK_CORE_RX21A            0x0D11FFFF
#define JLINK_CORE_RX220            0x0D12FFFF
#define JLINK_CORE_RX230            0x0D13FFFF
#define JLINK_CORE_RX231            0x0D14FFFF
#define JLINK_CORE_RX23T            0x0D15FFFF
#define JLINK_CORE_RX24T            0x0D16FFFF
// RX1xx sub family
#define JLINK_CORE_RX111            0x0D20FFFF  // Device family: 0x0D, sub family 0x20, revision not specified
#define JLINK_CORE_RX110            0x0D21FFFF
#define JLINK_CORE_RX113            0x0D22FFFF
#define JLINK_CORE_RX130            0x0D23FFFF
// RX64x sub family
#define JLINK_CORE_RX64M            0x0D30FFFF
#define JLINK_CORE_RX71M            0x0D31FFFF

#define JLINK_CORE_CORTEX_M4        0x0E0000FF
#define JLINK_CORE_CORTEX_M7        0x0E0100FF  // Device family: 0x0E (M4), sub family: 0x01
#define JLINK_CORE_CORTEX_M33       0x0E0200FF
#define JLINK_CORE_CORTEX_M55       0x0E0300FF
#define JLINK_CORE_STAR             0x0E0400FF  // ARM China STAR core. Backward compatible to Cortex-M33
#define JLINK_CORE_CORTEX_A5        0x0F0000FF
#define JLINK_CORE_POWER_PC         0x10FFFFFF
#define JLINK_CORE_POWER_PC_N1      0x10FF00FF  // Core with Nexus-1  support
#define JLINK_CORE_POWER_PC_N2      0x10FF01FF  // Core with Nexus-2+ support
#define JLINK_CORE_MIPS             0x11FFFFFF  // Dev family: MIPS, sub family not specified
#define JLINK_CORE_MIPS_M4K         0x1100FFFF  // Dev family: MIPS, sub family: 0x00 (M4K core)
#define JLINK_CORE_MIPS_MICROAPTIV  0x1101FFFF  // Dev family: MIPS, sub family: 0x01 (microAptiv core)
#define JLINK_CORE_MIPS_M14K        0x1102FFFF  // Dev family: MIPS, sub family: 0x02 (M14K core)
#define JLINK_CORE_EFM8_UNSPEC      0x12FFFFFF  // Dev family: SiLabs EFM8, sub family 0xFF (exact core not specified)
#define JLINK_CORE_CIP51            0x1200FFFF  // Dev family: SiLabs EFM8, sub family 0x00 (CIP51 core)
#define JLINK_CORE_RV32             ((JLINK_DEV_FAMILY_RISC_V << 24) | 0x00FFFFFF)  // RISC-V 32-bit
#define JLINK_CORE_RV64             ((JLINK_DEV_FAMILY_RISC_V << 24) | 0x0000FFFF)  // RISC-V 64-bit
//
// ARMv8A family related cores
//
#define JLINK_CORE_CORTEX_A53       0x1400FFFF
#define JLINK_CORE_CORTEX_A57       0x1401FFFF
#define JLINK_CORE_CORTEX_A72       0x1402FFFF
#define JLINK_CORE_CORTEX_A35       0x1403FFFF
//
// Microchip BT5511 8051 related cores
//
#define JLINK_CORE_BT5511           ((JLINK_DEV_FAMILY_BT5511 << 24) | 0x00FFFF)

/*********************************************************************
*
*       CPU capabilities
*/
#define JLINKARM_EMU_CPU_CAP_RESERVED               (1 <<  0)         // Always 1
#define JLINKARM_EMU_CPU_CAP_READ_MEM               (1 <<  1)         // Supports command "CPU_CMD_READ_MEM"
#define JLINKARM_EMU_CPU_CAP_WRITE_MEM              (1 <<  2)         // Supports command "CPU_CMD_WRITE_MEM"
#define JLINKARM_EMU_CPU_CAP_READ_REGS              (1 <<  3)         // Supports command "CPU_CMD_READ_REGS"
#define JLINKARM_EMU_CPU_CAP_WRITE_REGS             (1 <<  4)         // Supports command "CPU_CMD_WRITE_REGS"
#define JLINKARM_EMU_CPU_CAP_GO                     (1 <<  5)         // Supports command "CPU_CMD_GO"
#define JLINKARM_EMU_CPU_CAP_STEP                   (1 <<  6)         // Supports command "CPU_CMD_STEP"
#define JLINKARM_EMU_CPU_CAP_HALT                   (1 <<  7)         // Supports command "CPU_CMD_HALT"
#define JLINKARM_EMU_CPU_CAP_IS_HALTED              (1 <<  8)         // Supports command "CPU_CMD_IS_HALTED"
#define JLINKARM_EMU_CPU_CAP_RESET                  (1 <<  9)         // Supports command "CPU_CMD_RESET"
#define JLINKARM_EMU_CPU_CAP_RUN_STOP               (1 << 10)         // Supports command "CPU_CMD_RUN_STOP"
#define JLINKARM_EMU_CPU_CAP_TERMINAL               (1 << 11)         // Supports command "CPU2_CMD_READ_TERMINAL". Do not use. Use "EMU_CPU_CAP_DCC" instead for checking if JLINKARM_ReadTerminal() can be used.
                                                 // (1 << 12)         // Used internally only
                                                 // (1 << 13)         // Used internally only
#define JLINKARM_EMU_CPU_CAP_DCC                    (1 << 14)         // Supports command "CPU2_CMD_DCC"
#define JLINKARM_EMU_CPU_CAP_HSS                    (1 << 15)         // Supports command "CPU2_CMD_HSS"

/*********************************************************************
*
*       Emulator Capabilities (Query with JLINKARM_GetEmuCaps())
*/
#define JLINKARM_EMU_CAP_RESERVED                   (1 <<  0)       // Always 1
#define JLINKARM_EMU_CAP_GET_HW_VERSION             (1 <<  1)       // Supports command "EMU_CMD_GET_HARDWARE_VERSION"
#define JLINKARM_EMU_CAP_WRITE_DCC                  (1 <<  2)       // Supports command "EMU_CMD_WRITE_DCC"
#define JLINKARM_EMU_CAP_ADAPTIVE_CLOCKING          (1 <<  3)       // Supports adaptive clocking
#define JLINKARM_EMU_CAP_READ_CONFIG                (1 <<  4)       // Supports command "EMU_CMD_READ_CONFIG"
#define JLINKARM_EMU_CAP_WRITE_CONFIG               (1 <<  5)       // Supports command "EMU_CMD_WRITE_CONFIG"
#define JLINKARM_EMU_CAP_TRACE_ARM79                (1 <<  6)       // Supports trace commands
#define JLINKARM_EMU_CAP_WRITE_MEM                  (1 <<  7)       // Supports command "EMU_CMD_WRITE_MEM"
#define JLINKARM_EMU_CAP_READ_MEM                   (1 <<  8)       // Supports command "EMU_CMD_READ_MEM"
#define JLINKARM_EMU_CAP_SPEED_INFO                 (1 <<  9)       // Supports command "EMU_CMD_GET_SPEED"
#define JLINKARM_EMU_CAP_EXEC_CODE                  (1 << 10)       // Supports command "EMU_CMD_CODE_..."
#define JLINKARM_EMU_CAP_GET_MAX_BLOCK_SIZE         (1 << 11)       // Supports command "EMU_CMD_CODE_..."
#define JLINKARM_EMU_CAP_GET_HW_INFO                (1 << 12)       // Supports command "EMU_CMD_GET_HW_INFO"
#define JLINKARM_EMU_CAP_SET_KS_POWER               (1 << 13)       // Supports command "EMU_CMD_SET_KS_POWER"
#define JLINKARM_EMU_CAP_RESET_STOP_TIMED           (1 << 14)       // Supports command "EMU_CMD_HW_RELEASE_RESET_STOP_TIMED"
#define JLINKARM_EMU_CAP_GET_LICENSE_INFO           (1 << 15)       // Supports command "EMU_CMD_GET_LICENSE_INFO"
#define JLINKARM_EMU_CAP_MEASURE_RTCK_REACT         (1 << 16)       // Supports command "EMU_CMD_MEASURE_RTCK_REACT"
#define JLINKARM_EMU_CAP_SELECT_IF                  (1 << 17)       // Supports command "EMU_CMD_HW_SELECT_IF"
#define JLINKARM_EMU_CAP_RW_MEM_ARM79               (1 << 18)       // Supports command "EMU_CMD_WRITE_MEM_ARM79", "CMD_READ_MEM_ARM79"
#define JLINKARM_EMU_CAP_GET_COUNTERS               (1 << 19)       // Supports command "EMU_CMD_GET_COUNTERS"
#define JLINKARM_EMU_CAP_READ_DCC                   (1 << 20)       // Supports command "EMU_CMD_READ_DCC"
#define JLINKARM_EMU_CAP_GET_CPU_CAPS               (1 << 21)       // Supports command "EMU_CMD_GET_CPU_CAPS"
#define JLINKARM_EMU_CAP_EXEC_CPU_CMD               (1 << 22)       // Supports command "EMU_CMD_EXEC_CPU_CMD"
#define JLINKARM_EMU_CAP_SWO                        (1 << 23)       // Supports command "EMU_CMD_SWO"
#define JLINKARM_EMU_CAP_WRITE_DCC_EX               (1 << 24)       // Supports command "EMU_CMD_WRITE_DCC_EX"
#define JLINKARM_EMU_CAP_UPDATE_FIRMWARE_EX         (1 << 25)       // Supports command "EMU_CMD_UPDATE_FIRMWARE_EX"
#define JLINKARM_EMU_CAP_FILE_IO                    (1 << 26)       // Supports command "EMU_CMD_FILE_IO"
#define JLINKARM_EMU_CAP_REGISTER                   (1 << 27)       // Supports command "EMU_CMD_REGISTER"
#define JLINKARM_EMU_CAP_INDICATORS                 (1 << 28)       // Supports command "EMU_CMD_INDICATORS"
#define JLINKARM_EMU_CAP_TEST_NET_SPEED             (1 << 29)       // Supports command "EMU_CMD_TEST_NET_SPEED"
#define JLINKARM_EMU_CAP_RAWTRACE                   (1 << 30)       // Supports command "EMU_CMD_RAWTRACE"
#define JLINKARM_EMU_CAP_GET_CAPS_EX                (1 << 31)       // Supports command "EMU_CMD_GET_CAPS_EX"

/*********************************************************************
*
*       Extended Emulator Capabilities (Query with JLINKARM_GetEmuCapsEx())
*/
#define JLINKARM_EMU_CAP_EX_RESERVED                (0)             // Always 1
#define JLINKARM_EMU_CAP_EX_GET_HW_VERSION          (1)             // Supports command "EMU_CMD_GET_HARDWARE_VERSION"
#define JLINKARM_EMU_CAP_EX_WRITE_DCC               (2)             // Supports command "EMU_CMD_WRITE_DCC"
#define JLINKARM_EMU_CAP_EX_ADAPTIVE_CLOCKING       (3)             // Supports adaptive clocking
#define JLINKARM_EMU_CAP_EX_READ_CONFIG             (4)             // Supports command "EMU_CMD_READ_CONFIG"
#define JLINKARM_EMU_CAP_EX_WRITE_CONFIG            (5)             // Supports command "EMU_CMD_WRITE_CONFIG"
#define JLINKARM_EMU_CAP_EX_TRACE_ARM79             (6)             // Supports trace commands
#define JLINKARM_EMU_CAP_EX_WRITE_MEM               (7)             // Supports command "EMU_CMD_WRITE_MEM"
#define JLINKARM_EMU_CAP_EX_READ_MEM                (8)             // Supports command "EMU_CMD_READ_MEM"
#define JLINKARM_EMU_CAP_EX_SPEED_INFO              (9)             // Supports command "EMU_CMD_GET_SPEED"
#define JLINKARM_EMU_CAP_EX_EXEC_CODE               (10)            // Supports command "EMU_CMD_CODE_..."
#define JLINKARM_EMU_CAP_EX_GET_MAX_BLOCK_SIZE      (11)            // Supports command "EMU_CMD_CODE_..."
#define JLINKARM_EMU_CAP_EX_GET_HW_INFO             (12)            // Supports command "EMU_CMD_GET_HW_INFO"
#define JLINKARM_EMU_CAP_EX_SET_KS_POWER            (13)            // Supports command "EMU_CMD_SET_KS_POWER"
#define JLINKARM_EMU_CAP_EX_RESET_STOP_TIMED        (14)            // Supports command "EMU_CMD_HW_RELEASE_RESET_STOP_TIMED"
#define JLINKARM_EMU_CAP_EX_GET_LICENSE_INFO        (15)            // Supports command "EMU_CMD_GET_LICENSE_INFO"
#define JLINKARM_EMU_CAP_EX_MEASURE_RTCK_REACT      (16)            // Supports command "EMU_CMD_MEASURE_RTCK_REACT"
#define JLINKARM_EMU_CAP_EX_SELECT_IF               (17)            // Supports command "EMU_CMD_HW_SELECT_IF"
#define JLINKARM_EMU_CAP_EX_RW_MEM_ARM79            (18)            // Supports command "EMU_CMD_WRITE_MEM_ARM79", "CMD_READ_MEM_ARM79"
#define JLINKARM_EMU_CAP_EX_GET_COUNTERS            (19)            // Supports command "EMU_CMD_GET_COUNTERS"
#define JLINKARM_EMU_CAP_EX_READ_DCC                (20)            // Supports command "EMU_CMD_READ_DCC"
#define JLINKARM_EMU_CAP_EX_GET_CPU_CAPS            (21)            // Supports command "EMU_CMD_GET_CPU_CAPS"
#define JLINKARM_EMU_CAP_EX_EXEC_CPU_CMD            (22)            // Supports command "EMU_CMD_EXEC_CPU_CMD"
#define JLINKARM_EMU_CAP_EX_SWO                     (23)            // Supports command "EMU_CMD_SWO"
#define JLINKARM_EMU_CAP_EX_WRITE_DCC_EX            (24)            // Supports command "EMU_CMD_WRITE_DCC_EX"
#define JLINKARM_EMU_CAP_EX_UPDATE_FIRMWARE_EX      (25)            // Supports command "EMU_CMD_UPDATE_FIRMWARE_EX"
#define JLINKARM_EMU_CAP_EX_FILE_IO                 (26)            // Supports command "EMU_CMD_FILE_IO"
#define JLINKARM_EMU_CAP_EX_REGISTER                (27)            // Supports command "EMU_CMD_REGISTER"
#define JLINKARM_EMU_CAP_EX_INDICATORS              (28)            // Supports command "EMU_CMD_INDICATORS"
#define JLINKARM_EMU_CAP_EX_TEST_NET_SPEED          (29)            // Supports command "EMU_CMD_TEST_NET_SPEED"
#define JLINKARM_EMU_CAP_EX_RAWTRACE                (30)            // Supports command "EMU_CMD_RAWTRACE"
#define JLINKARM_EMU_CAP_EX_GET_CAPS_EX             (31)            // Supports command "EMU_CMD_GET_CAPS_EX"
#define JLINKARM_EMU_CAP_EX_HW_JTAG_WRITE           (32)            // Supports command "EMU_CMD_HW_JTAG_WRITE"
#define JLINKARM_EMU_CAP_EX_EMUCOM                  (33)            // Supports command "EMU_CMD_EMUCOM"
#define JLINKARM_EMU_CAP_EX_EXEC_CPU2_CMD           (34)            // Supports command "EMU_CMD_EXEC_CPU2_CMD"
#define JLINKARM_EMU_CAP_EX_GET_CPU2_CAPS           (35)            // Supports command "EMU_CMD_GET_CPU2_CAPS"
#define JLINKARM_EMU_CAP_EX_TEST_NET                (36)            // Supports command "EMU_CMD_TEST_NET"
#define JLINKARM_EMU_CAP_EX_POWERTRACE              (37)            // Supports command "EMU_CMD_POWERTRACE"
#define JLINKARM_EMU_CAP_EX_ETHERNET                (38)            // Supports communication via ethernet
#define JLINKARM_EMU_CAP_EX_HW_SET_SWDIO_DIR        (39)            // Supports commands "EMU_CMD_HW_SWD_SET_DIR_OUT",  "EMU_CMD_HW_SWD_SET_DIR_IN"
#define JLINKARM_EMU_CAP_EX_HW_ENABLE_DISABLE_SWCLK (40)            // Supports commands "EMU_CMD_HW_SWD_ENABLE_SWCLK", "EMU_CMD_HW_SWD_DISABLE_SWCLK"
#define JLINKARM_EMU_CAP_EX_HW_ENABLE_DISABLE_JTAG_IF (41)          // Supports commands "EMU_CMD_HW_JTAG_ENABLE_IF",   "EMU_CMD_HW_JTAG_DISABLE_IF"
#define JLINKARM_EMU_CAP_EX_HW_SET_CLEAR_TCK        (42)            // Supports commands "EMU_CMD_HW_TCK0",             "EMU_CMD_HW_TCK1"
// (43) is for internal use only
#define JLINKARM_EMU_CAP_EX_PCODE_EXEC              (44)            // Supports command "EMU_CMD_PCODE_EXEC"
// (45) is for internal use only
// (46) is for internal use only
// (47) is for internal use only
#define JLINKARM_EMU_CAP_EX_CDC_EXEC                (48)            // Supports command "EMU_CMD_CDC_EXEC"
#define JLINKARM_EMU_CAP_EX_CDC_SET_HOOK_FUNCS      (49)            // Supports command "EMU_CMD_CDC_SET_HOOK_FUNCS"
#define JLINKARM_EMU_CAP_EX_HANDLE_BMI              (50)            // Supports JLINKARM_SetBMI() / JLINKARM_GetBMI()
#define JLINKARM_EMU_CAP_EX_HANDLE_GPIO             (51)            // Supports command "EMU_CMD_HANDLE_GPIO"
// (52) is for internal use only
#define JLINKARM_EMU_CAP_EX_HSS                     (53)            // Supports command "EMU_CMD_HSS"
#define JLINKARM_EMU_CAP_EX_FILE_IO_EX              (54)            // Supports extended command set of FILE IO commands
// (55) is for internal use only
#define JLINKARM_EMU_CAP_EX_SPI                     (56)            // Supports command "EMU_CMD_SPI"
#define JLINKARM_EMU_CAP_EX_SUPPORT_OTS_CONFIG_X    (57)            // Supports command "EMU_CMD_HANDLE_OTSX_CONFIGX"
#define JLINKARM_EMU_CAP_EX_HANDLE_SECURITY         (58)            // Supports command "EMU_SECURE"
#define JLINKARM_EMU_CAP_EX_PIN_OVERRIDE            (59)            // Supports command "EMU_CMD_PIN_OVERRIDE"
// (60) is for internal use only
#define JLINKARM_EMU_CAP_EX_WINUSB                  (61)            // Supports configuring J-Link to use WinUSB instead of SEGGER driver
// (62) is for internal use only
// (63) is for internal use only
// (64) is for internal use only
// (65) is for internal use only
#define JLINKARM_EMU_CAP_EX_USB_IP                  (66)            // Supports "IP over USB" feature
#define JLINKARM_EMU_CAP_EX_WIFI                    (67)            // Supports WiFi feature
#define JLINKARM_EMU_CAP_EX_FIXED_VTREF             (68)            // Supports fixed VTref feature
// (69) is for internal use only
#define JLINKARM_EMU_CAP_EX_MULTI_VCOM              (70)            // 1 == Supports multiple VCOM ports
// Internal use only                                (71)
#define JLINKARM_EMU_CAP_EX_SPI_STAND_ALONE         (72)

#define JLINKARM_EMUCOM_CHANNEL_TIME                (0x0000)        // Always returns 4 bytes: OS_Time
#define JLINKARM_EMUCOM_CHANNEL_USER                (0x10000)       // The first 64k channels are reserved

#define JLINKARM_EMUCOM_ERR                         (0x80000000UL)  // Generic error
#define JLINKARM_EMUCOM_ERR_CHANNEL_NOT_SUPPORTED   (0x80000001UL)  // Selected channel is not supported by emulator
#define JLINKARM_EMUCOM_ERR_BUFFER_TOO_SMALL        (0x81000000UL)  // The lower 24 bytes contain required buffer size

/*********************************************************************
*
*       Supported host interfaces for JLINKARM_EMU_GetList()
*/
#define JLINKARM_HOSTIF_USB   (1 << 0)
#define JLINKARM_HOSTIF_IP    (1 << 1)

//
// Flags for JLINKARM_BP_
//
#define JLINKARM_BP_MODE0                 (0 << 0)      // Meaning depends on CPU type
#define JLINKARM_BP_MODE1                 (1 << 0)      // Meaning depends on CPU type
#define JLINKARM_BP_MODE2                 (2 << 0)      // Meaning depends on CPU type
#define JLINKARM_BP_MODE3                 (3 << 0)      // Meaning depends on CPU type
#define JLINKARM_BP_MODE_MASK             (0x0000000F)

#define JLINKARM_BP_IMP_SW_RAM            (1 << 4)
#define JLINKARM_BP_IMP_SW_FLASH          (1 << 5)
#define JLINKARM_BP_IMP_SW                (0x000000F0)
#define JLINKARM_BP_IMP_HW                (0xFFFFFF00)
#define JLINKARM_BP_IMP_ANY               (JLINKARM_BP_IMP_HW | JLINKARM_BP_IMP_SW)
#define JLINKARM_BP_IMP_MASK              (JLINKARM_BP_IMP_ANY)

#define JLINKARM_BP_HANDLE_ALL            (0xFFFFFFFF)
#define JLINKARM_WP_HANDLE_ALL            (0xFFFFFFFF)

//
// Flags for ARM7/9, Cortex-A and Cortex-A cores
//
#define JLINKARM_BP_TYPE_ARM              JLINKARM_BP_MODE1
#define JLINKARM_BP_TYPE_THUMB            JLINKARM_BP_MODE2
//
// For Cortex-M, BP_MODE_ / BP_TYPE_ does not matter
//
//
// For SiLabs EFM8, BP_MODE_ / BP_TYPE_ does not matter
//


//
// Flags for JLINKARM_BP_INFO (ImpFlags member)
//

#define JLINKARM_BP_IMP_FLAG_HARD         (1 << 0)      // Hardware breakpoint ?
#define JLINKARM_BP_IMP_FLAG_SOFT         (1 << 1)      // Software breakpoint ?
#define JLINKARM_BP_IMP_FLAG_PENDING      (1 << 2)      // Breakpoint not yet written to memory (SWBPs only)
#define JLINKARM_BP_IMP_FLAG_FLASH        (1 << 4)      // FlashBP ?

/*********************************************************************
*
*       PCODE caps
*/

#define JLINK_PCODE_CAP_RESERVED          (1 <<  0)
#define JLINK_PCODE_CAP_GET_S32_VERSION   (1 <<  1)
#define JLINK_PCODE_CAP_EXEC              (1 <<  2)
#define JLINK_PCODE_CAP_CREATE_MODULE     (1 <<  3)
#define JLINK_PCODE_CAP_DELETE_MODULE     (1 <<  4)
#define JLINK_PCODE_CAP_DOWNLOAD          (1 <<  5)
#define JLINK_PCODE_CAP_FREE              (1 <<  6)
#define JLINK_PCODE_CAP_GO                (1 <<  7)
#define JLINK_PCODE_CAP_STEP              (1 <<  8)
#define JLINK_PCODE_CAP_READ_MEM          (1 <<  9)
#define JLINK_PCODE_CAP_WRITE_MEM         (1 << 10)
#define JLINK_PCODE_CAP_SUPPORT_V2        (1 << 11)

/*********************************************************************
*
*       Flags for JLINKARM_SetDataEvent()
*/

#define JLINKARM_EVENT_TYPE_DATA_BP             (1 << 0)

#define JLINKARM_EVENT_ERR_UNKNOWN              (0x80000000)
#define JLINKARM_EVENT_ERR_NO_MORE_EVENTS       (0x80000001)
#define JLINKARM_EVENT_ERR_NO_MORE_ADDR_COMP    (0x80000002)
#define JLINKARM_EVENT_ERR_NO_MORE_DATA_COMP    (0x80000004)
#define JLINKARM_EVENT_ERR_INVALID_ADDR_MASK    (0x80000020)
#define JLINKARM_EVENT_ERR_INVALID_DATA_MASK    (0x80000040)
#define JLINKARM_EVENT_ERR_INVALID_ACCESS_MASK  (0x80000080)

#define JLINK_EVENT_HANDLE_ALL                  (0xFFFFFFFF)

#define JLINK_EVENT_DATA_BP_DIR_RD              (0 << 0)
#define JLINK_EVENT_DATA_BP_DIR_WR              (1 << 0)
#define JLINK_EVENT_DATA_BP_PRIV                (1 << 4)
#define JLINK_EVENT_DATA_BP_SIZE_8BIT           (0 << 1)
#define JLINK_EVENT_DATA_BP_SIZE_16BIT          (1 << 1)
#define JLINK_EVENT_DATA_BP_SIZE_32BIT          (2 << 1)
//
// Flags for the parameter TypeMask
//
#define JLINK_EVENT_DATA_BP_MASK_SIZE           (3 << 1)
#define JLINK_EVENT_DATA_BP_MASK_DIR            (1 << 0)
#define JLINK_EVENT_DATA_BP_MASK_PRIV           (1 << 4)

/*********************************************************************
*
*       Flags for JLINKARM_SetWP()
*/
//
// Flags for the parameter AccessType
//
#define JLINKARM_WP_DIR_RD      (0 << 0)
#define JLINKARM_WP_DIR_WR      (1 << 0)
#define JLINKARM_WP_PRIV        (1 << 4)
#define JLINKARM_WP_SIZE_8BIT   (0 << 1)
#define JLINKARM_WP_SIZE_16BIT  (1 << 1)
#define JLINKARM_WP_SIZE_32BIT  (2 << 1)
//
// Flags for the parameter TypeMask
//
#define JLINKARM_WP_MASK_SIZE   (3 << 1)
#define JLINKARM_WP_MASK_DIR    (1 << 0)
#define JLINKARM_WP_MASK_PRIV   (1 << 4)

#define JLINKARM_SPEED_AUTO         0
#define JLINKARM_SPEED_INVALID      0xFFFE
#define JLINKARM_SPEED_ADAPTIVE     0xFFFF

#define JLINKARM_INDICATOR_ID_LED_GREEN  0
#define JLINKARM_INDICATOR_ID_LED_RED    1
#define JLINKARM_INDICATOR_ID_BUSY       2
#define JLINKARM_INDICATOR_ID_OK         3

//
// Flags for JLINKARM_GoEx()
//
#define JLINKARM_GO_FLAG_OVERSTEP_BP        (1 << 0)    // Overstep the current instruction if it is breakpointed
#define JLINKARM_GO_FLAG_DISABLE_INT        (1 << 1)    // Disable/Block interrupts for this Go()

#define JLINKARM_GO_MAX_EMUL_INSTS_DEFAULT  (-1)        // Use the same settings as JLINKARM_Go()

//
// Flags for JLINKARM_BeginDownload()
//
#define JLINKARM_ALLOW_FLASH_DOWNLOAD           (1 << 0)
#define JLINKARM_ALLOW_BUFFERED_RAM_DOWNLOAD    (1 << 1)

/*********************************************************************
*
*       Flags for JLINKARM_WriteVectorCatch()
*/
#define JLINKARM_CORTEX_R4_VCATCH_RESET    (1 << 0)
#define JLINKARM_CORTEX_R4_VCATCH_UNDEF    (1 << 1)
#define JLINKARM_CORTEX_R4_VCATCH_SVC      (1 << 2)
#define JLINKARM_CORTEX_R4_VCATCH_SWI      JLINKARM_CORTEX_R4_VCATCH_SVC
#define JLINKARM_CORTEX_R4_VCATCH_PREFETCH (1 << 3)
#define JLINKARM_CORTEX_R4_VCATCH_ABORT    (1 << 4)
#define JLINKARM_CORTEX_R4_VCATCH_IRQ      (1 << 6)
#define JLINKARM_CORTEX_R4_VCATCH_FIQ      (1 << 7)

//
// ARM DDI0337G (Cortex-M3 TRM): 10.2.4 Debug Exception and Monitor Control Register (DEMCR)
//
// [0]   Read/write     VC_CORERESET Reset Vector Catch. Halt running system if Core reset occurs.
// [3:1] -              -            Reserved, SBZP
// [4]   Read/write     VC_MMERR     Debug trap on Memory Management faults.
// [5]   Read/write     VC_NOCPERR   Debug trap on Usage Fault access to Coprocessor that is not present or marked as not present in CAR register.
// [6]   Read/write     VC_CHKERR    Debug trap on Usage Fault enabled checking errors.
// [7]   Read/write     VC_STATERR   Debug trap on Usage Fault state errors.
// [8]   Read/write     VC_BUSERR    Debug Trap on normal Bus error.
// [9]   Read/write     VC_INTERR    Debug Trap on interrupt/exception service errors. These are a subset of other faults and catches before BUSERR or HARDERR.
// [10]  Read/write     VC_HARDERR   Debug trap on Hard Fault.
//
#define JLINKARM_CORTEX_M3_VCATCH_CORERESET (1 <<  0)
#define JLINKARM_CORTEX_M3_VCATCH_MMERR     (1 <<  4)
#define JLINKARM_CORTEX_M3_VCATCH_NOCPERR   (1 <<  5)
#define JLINKARM_CORTEX_M3_VCATCH_CHKERR    (1 <<  6)
#define JLINKARM_CORTEX_M3_VCATCH_STATERR   (1 <<  7)
#define JLINKARM_CORTEX_M3_VCATCH_BUSERR    (1 <<  8)
#define JLINKARM_CORTEX_M3_VCATCH_INTERR    (1 <<  9)
#define JLINKARM_CORTEX_M3_VCATCH_HARDERR   (1 << 10)

#define JLINKARM_MAX_NUM_CPU_REGS           (256)

/*********************************************************************
*
*       Flags for JLINK_EMU_GPIO_SetState(), parameter *paState
*/

#define JLINK_EMU_GPIO_STATE_UNCHANGE                   0
#define JLINK_EMU_GPIO_STATE_INPUT                      1   // Set to input
#define JLINK_EMU_GPIO_STATE_CLR                        2   // Set to output + LOW level
#define JLINK_EMU_GPIO_STATE_SET                        3   // Set to output + HIGH level

/*********************************************************************
*
*       Group Ids for JLINK_MRU_GetList()
*/
#define JLINK_MRU_GROUP_DEVICE          0
#define JLINK_MRU_GROUP_SETTINGSFILE    1
#define JLINK_MRU_GROUP_SCRIPTFILE      2

/*********************************************************************
*
*       Flags for external dialogs that are called via hook functions
*/
#define JLINK_DLG_BUTTON_YES                  (1 << 0)
#define JLINK_DLG_BUTTON_NO                   (1 << 1)
#define JLINK_DLG_BUTTON_OK                   (1 << 2)
#define JLINK_DLG_BUTTON_CANCEL               (1 << 3)

#define JLINK_DLG_TYPE_PROT_READ              (1 << 28)
#define JLINK_DLG_TYPE_PROT_WRITE             (2 << 28)


/*********************************************************************
*
*       Types required for API
*
**********************************************************************
*/

//
// ARM_REG
//
typedef enum {
  ARM_REG_R0,                         // Index  0
  ARM_REG_R1,                         // Index  1
  ARM_REG_R2,                         // Index  2
  ARM_REG_R3,                         // Index  3
  ARM_REG_R4,                         // Index  4
  ARM_REG_R5,                         // Index  5
  ARM_REG_R6,                         // Index  6
  ARM_REG_R7,                         // Index  7
  ARM_REG_CPSR,                       // Index  8
  ARM_REG_R15,                        // Index  9
  ARM_REG_R8_USR,                     // Index 10
  ARM_REG_R9_USR,                     // Index 11
  ARM_REG_R10_USR,                    // Index 12
  ARM_REG_R11_USR,                    // Index 13
  ARM_REG_R12_USR,                    // Index 14
  ARM_REG_R13_USR,                    // Index 15
  ARM_REG_R14_USR,                    // Index 16
  ARM_REG_SPSR_FIQ,                   // Index 17
  ARM_REG_R8_FIQ,                     // Index 18
  ARM_REG_R9_FIQ,                     // Index 19
  ARM_REG_R10_FIQ,                    // Index 20
  ARM_REG_R11_FIQ,                    // Index 21
  ARM_REG_R12_FIQ,                    // Index 22
  ARM_REG_R13_FIQ,                    // Index 23
  ARM_REG_R14_FIQ,                    // Index 24
  ARM_REG_SPSR_SVC,                   // Index 25
  ARM_REG_R13_SVC,                    // Index 26
  ARM_REG_R14_SVC,                    // Index 27
  ARM_REG_SPSR_ABT,                   // Index 28
  ARM_REG_R13_ABT,                    // Index 29
  ARM_REG_R14_ABT,                    // Index 30
  ARM_REG_SPSR_IRQ,                   // Index 31
  ARM_REG_R13_IRQ,                    // Index 32
  ARM_REG_R14_IRQ,                    // Index 33
  ARM_REG_SPSR_UND,                   // Index 34
  ARM_REG_R13_UND,                    // Index 35
  ARM_REG_R14_UND,                    // Index 36
  ARM_REG_FPSID,                      // Index 37
  ARM_REG_FPSCR,                      // Index 38
  ARM_REG_FPEXC,                      // Index 39
  ARM_REG_FPS0,                       // Index 40
  ARM_REG_FPS1,                       // Index 41
  ARM_REG_FPS2,                       // Index 42
  ARM_REG_FPS3,                       // Index 43
  ARM_REG_FPS4,                       // Index 44
  ARM_REG_FPS5,                       // Index 45
  ARM_REG_FPS6,                       // Index 46
  ARM_REG_FPS7,                       // Index 47
  ARM_REG_FPS8,                       // Index 48
  ARM_REG_FPS9,                       // Index 49
  ARM_REG_FPS10,                      // Index 50
  ARM_REG_FPS11,                      // Index 51
  ARM_REG_FPS12,                      // Index 52
  ARM_REG_FPS13,                      // Index 53
  ARM_REG_FPS14,                      // Index 54
  ARM_REG_FPS15,                      // Index 55
  ARM_REG_FPS16,                      // Index 56
  ARM_REG_FPS17,                      // Index 57
  ARM_REG_FPS18,                      // Index 58
  ARM_REG_FPS19,                      // Index 59
  ARM_REG_FPS20,                      // Index 60
  ARM_REG_FPS21,                      // Index 61
  ARM_REG_FPS22,                      // Index 62
  ARM_REG_FPS23,                      // Index 63
  ARM_REG_FPS24,                      // Index 64
  ARM_REG_FPS25,                      // Index 65
  ARM_REG_FPS26,                      // Index 66
  ARM_REG_FPS27,                      // Index 67
  ARM_REG_FPS28,                      // Index 68
  ARM_REG_FPS29,                      // Index 69
  ARM_REG_FPS30,                      // Index 70
  ARM_REG_FPS31,                      // Index 71
  ARM_REG_R8,                         // Index 72
  ARM_REG_R9,                         // Index 73
  ARM_REG_R10,                        // Index 74
  ARM_REG_R11,                        // Index 75
  ARM_REG_R12,                        // Index 76
  ARM_REG_R13,                        // Index 77
  ARM_REG_R14,                        // Index 78
  ARM_REG_SPSR,                       // Index 79
  ARM_NUM_REGS
} ARM_REG;

//
// JLINKARM_CM3_REG
//
typedef enum {
  JLINKARM_CM3_REG_R0,                // Index  0: HW-Reg
  JLINKARM_CM3_REG_R1,                // Index  1: HW-Reg
  JLINKARM_CM3_REG_R2,                // Index  2: HW-Reg
  JLINKARM_CM3_REG_R3,                // Index  3: HW-Reg
  JLINKARM_CM3_REG_R4,                // Index  4: HW-Reg
  JLINKARM_CM3_REG_R5,                // Index  5: HW-Reg
  JLINKARM_CM3_REG_R6,                // Index  6: HW-Reg
  JLINKARM_CM3_REG_R7,                // Index  7: HW-Reg
  JLINKARM_CM3_REG_R8,                // Index  8: HW-Reg
  JLINKARM_CM3_REG_R9,                // Index  9: HW-Reg
  JLINKARM_CM3_REG_R10,               // Index 10: HW-Reg
  JLINKARM_CM3_REG_R11,               // Index 11: HW-Reg
  JLINKARM_CM3_REG_R12,               // Index 12: HW-Reg
  JLINKARM_CM3_REG_R13,               // Index 13: Pseudo reg! It needs to be mapped to SP_MSP or SP_PSP, depending on current Controlregister:
  JLINKARM_CM3_REG_R14,               // Index 14: HW-Reg
  JLINKARM_CM3_REG_R15,               // Index 15: HW-Reg
  JLINKARM_CM3_REG_XPSR,              // Index 16: HW-Reg
  JLINKARM_CM3_REG_MSP,               // Index 17: HW-Reg  Main stack pointer
  JLINKARM_CM3_REG_PSP,               // Index 18: HW-Reg  Process stack pointer
  JLINKARM_CM3_REG_RAZ,               // Index 19: HW-Reg, Unused/Reserved
  JLINKARM_CM3_REG_CFBP,              // Index 20: HW-Reg, CONTROL/FAULTMASK/BASEPRI/PRIMASK (packed into 4 bytes of word. CONTROL = CFBP[31:24], FAULTMASK = CFBP[16:23], BASEPRI = CFBP[15:8], PRIMASK = CFBP[7:0]
  JLINKARM_CM3_REG_APSR,              // Index 21: Pseudo reg. (Part of XPSR)
  JLINKARM_CM3_REG_EPSR,              // Index 22: Pseudo reg. (Part of XPSR)
  JLINKARM_CM3_REG_IPSR,              // Index 23: Pseudo reg. (Part of XPSR)
  //
  // Data for CFBP pseudo regs is expected & returned
  // in same byte lane as when reading/writing the CFBP.
  // This means when reading
  //  PRIMASK,     the data of PRIMASK is expected/returned at     bits [7:0]
  //  BASEPRI,     the data of BASEPRI is expected/returned at     bits [15:8]
  //  BASEPRI_MAX, the data of BASEPRI_MAX is expected/returned at bits [15:8]
  //  FAULTMASK,   the data of FAULTMASK is expected/returned at   bits [23:16]
  //  CONTROL,     the data of CONTROL is expected/returned at     bits [31:24]
  //
  // Sample CFBP = 0xx11223344
  // Read PRIMASK   will return 0x00000044
  // Read BASEPRI   will return 0x00003300
  // Read FAULTMASK will return 0x00220000
  // Read CONTROL   will return 0x11000000
  // When writing, data is expected at the same position
  //
  // Note:
  // For BASEPRI, BASEPRI_MAX, FAULTMASK, CONTROL base 0 (always shifted to byte-lane starting at bit 0), please use:
  // BASEPRI_BASE0, BASEPRI_MAX_BASE0, FAULTMASK_BASE0, CONTROL_BASE0
  //
  //
  JLINKARM_CM3_REG_PRIMASK,           // Index 24: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI,           // Index 25: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_FAULTMASK,         // Index 26: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_CONTROL,           // Index 27: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_MAX,       // Index 28: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_IAPSR,             // Index 29: Pseudo reg. (Part of XPSR)
  JLINKARM_CM3_REG_EAPSR,             // Index 30: Pseudo reg. (Part of XPSR)
  JLINKARM_CM3_REG_IEPSR,             // Index 31: Pseudo reg. (Part of XPSR)
  //
  // Pseudo registers (not really CPU registers but handled as such ones
  //
  JLINKARM_CM3_REG_DWT_CYCCNT=65,     // Index 65
  //
  // New regs introduced with ARMv8M architecture
  //
  JLINKARM_CM3_REG_MSP_NS,
  JLINKARM_CM3_REG_PSP_NS,
  JLINKARM_CM3_REG_MSP_S,
  JLINKARM_CM3_REG_PSP_S,
  JLINKARM_CM3_REG_MSPLIM_S,
  JLINKARM_CM3_REG_PSPLIM_S,
  JLINKARM_CM3_REG_MSPLIM_NS,
  JLINKARM_CM3_REG_PSPLIM_NS,
  JLINKARM_CM3_REG_CFBP_S,
  JLINKARM_CM3_REG_CFBP_NS,
  //
  // Data for CFBP pseudo regs is expected & returned
  // in same byte lane as when reading/writing the CFBP.
  // This means when reading
  //  PRIMASK,     the data of PRIMASK is expected/returned at     bits [7:0]
  //  BASEPRI,     the data of BASEPRI is expected/returned at     bits [15:8]
  //  BASEPRI_MAX, the data of BASEPRI_MAX is expected/returned at bits [15:8]
  //  FAULTMASK,   the data of FAULTMASK is expected/returned at   bits [23:16]
  //  CONTROL,     the data of CONTROL is expected/returned at     bits [31:24]
  //
  // Sample CFBP = 0xx11223344
  // Read PRIMASK   will return 0x00000044
  // Read BASEPRI   will return 0x00003300
  // Read FAULTMASK will return 0x00220000
  // Read CONTROL   will return 0x11000000
  // When writing, data is expected at the same position
  //
  JLINKARM_CM3_REG_PRIMASK_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_FAULTMASK_NS,      // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_CONTROL_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_MAX_NS,    // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_PRIMASK_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_FAULTMASK_S,       // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_CONTROL_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_MAX_S,     // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_MSPLIM,
  JLINKARM_CM3_REG_PSPLIM,
  JLINKARM_CM3_REG_BASEPRI_BASE0,           // (CFBP[15:8]  >>  8) & 0xFF
  JLINKARM_CM3_REG_FAULTMASK_BASE0,         // (CFBP[23:16] >> 16) & 0xFF
  JLINKARM_CM3_REG_CONTROL_BASE0,           // (CFBP[31:24] >> 24) & 0xFF
  JLINKARM_CM3_REG_BASEPRI_MAX_BASE0,       // (CFBP[15:8]  >>  8) & 0xFF
  JLINKARM_CM3_NUM_REGS
} JLINKARM_CM3_REG;

//
// JLINKARM_CM4_REG
//
typedef enum {
  JLINKARM_CM4_REG_R0,                // Index  0
  JLINKARM_CM4_REG_R1,                // Index  1
  JLINKARM_CM4_REG_R2,                // Index  2
  JLINKARM_CM4_REG_R3,                // Index  3
  JLINKARM_CM4_REG_R4,                // Index  4
  JLINKARM_CM4_REG_R5,                // Index  5
  JLINKARM_CM4_REG_R6,                // Index  6
  JLINKARM_CM4_REG_R7,                // Index  7
  JLINKARM_CM4_REG_R8,                // Index  8
  JLINKARM_CM4_REG_R9,                // Index  9
  JLINKARM_CM4_REG_R10,               // Index 10
  JLINKARM_CM4_REG_R11,               // Index 11
  JLINKARM_CM4_REG_R12,               // Index 12
  JLINKARM_CM4_REG_R13,               // Index 13: Pseudo reg! It needs to be mapped to SP_MSP or SP_PSP, depending on current Controlregister:
  JLINKARM_CM4_REG_R14,               // Index 14
  JLINKARM_CM4_REG_R15,               // Index 15
  JLINKARM_CM4_REG_XPSR,              // Index 16
  JLINKARM_CM4_REG_MSP,               // Index 17
  JLINKARM_CM4_REG_PSP,               // Index 18
  JLINKARM_CM4_REG_RAZ,               // Index 19: Reserved
  JLINKARM_CM4_REG_CFBP,              // Index 20: CONTROL/FAULTMASK/BASEPRI/PRIMASK (packed into 4 bytes of word. CONTROL = CFBP[31:24], FAULTMASK = CFBP[16:23], BASEPRI = CFBP[15:8], PRIMASK = CFBP[7:0]
  JLINKARM_CM4_REG_APSR,              // Index 21: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_EPSR,              // Index 22: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_IPSR,              // Index 23: Pseudo reg. (Part of XPSR)
  //
  // Data for CFBP pseudo regs is expected & returned
  // in same byte lane as when reading/writing the CFBP.
  // This means when reading
  //  PRIMASK,     the data of PRIMASK is expected/returned at     bits [7:0]
  //  BASEPRI,     the data of BASEPRI is expected/returned at     bits [15:8]
  //  BASEPRI_MAX, the data of BASEPRI_MAX is expected/returned at bits [15:8]
  //  FAULTMASK,   the data of FAULTMASK is expected/returned at   bits [23:16]
  //  CONTROL,     the data of CONTROL is expected/returned at     bits [31:24]
  //
  // Sample CFBP = 0xx11223344
  // Read PRIMASK   will return 0x00000044
  // Read BASEPRI   will return 0x00003300
  // Read FAULTMASK will return 0x00220000
  // Read CONTROL   will return 0x11000000
  // When writing, data is expected at the same position
  //
  JLINKARM_CM4_REG_PRIMASK,           // Index 24: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI,           // Index 25: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_FAULTMASK,         // Index 26: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_CONTROL,           // Index 27: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_MAX,       // Index 28: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_IAPSR,             // Index 29: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_EAPSR,             // Index 30: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_IEPSR,             // Index 31: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_FPSCR,             // Index 32
  JLINKARM_CM4_REG_FPS0,              // Index 33
  JLINKARM_CM4_REG_FPS1,              // Index 34
  JLINKARM_CM4_REG_FPS2,              // Index 35
  JLINKARM_CM4_REG_FPS3,              // Index 36
  JLINKARM_CM4_REG_FPS4,              // Index 37
  JLINKARM_CM4_REG_FPS5,              // Index 38
  JLINKARM_CM4_REG_FPS6,              // Index 39
  JLINKARM_CM4_REG_FPS7,              // Index 40
  JLINKARM_CM4_REG_FPS8,              // Index 41
  JLINKARM_CM4_REG_FPS9,              // Index 42
  JLINKARM_CM4_REG_FPS10,             // Index 43
  JLINKARM_CM4_REG_FPS11,             // Index 44
  JLINKARM_CM4_REG_FPS12,             // Index 45
  JLINKARM_CM4_REG_FPS13,             // Index 46
  JLINKARM_CM4_REG_FPS14,             // Index 47
  JLINKARM_CM4_REG_FPS15,             // Index 48
  JLINKARM_CM4_REG_FPS16,             // Index 49
  JLINKARM_CM4_REG_FPS17,             // Index 50
  JLINKARM_CM4_REG_FPS18,             // Index 51
  JLINKARM_CM4_REG_FPS19,             // Index 52
  JLINKARM_CM4_REG_FPS20,             // Index 53
  JLINKARM_CM4_REG_FPS21,             // Index 54
  JLINKARM_CM4_REG_FPS22,             // Index 55
  JLINKARM_CM4_REG_FPS23,             // Index 56
  JLINKARM_CM4_REG_FPS24,             // Index 57
  JLINKARM_CM4_REG_FPS25,             // Index 58
  JLINKARM_CM4_REG_FPS26,             // Index 59
  JLINKARM_CM4_REG_FPS27,             // Index 60
  JLINKARM_CM4_REG_FPS28,             // Index 61
  JLINKARM_CM4_REG_FPS29,             // Index 62
  JLINKARM_CM4_REG_FPS30,             // Index 63
  JLINKARM_CM4_REG_FPS31,             // Index 64
  //
  // Pseudo registers (not really CPU registers but handled as such ones
  //
  JLINKARM_CM4_REG_DWT_CYCCNT,        // Index 65
  //
  // New regs introduced with ARMv8M architecture
  //
  JLINKARM_CM4_REG_MSP_NS,
  JLINKARM_CM4_REG_PSP_NS,
  JLINKARM_CM4_REG_MSP_S,
  JLINKARM_CM4_REG_PSP_S,
  JLINKARM_CM4_REG_MSPLIM_S,
  JLINKARM_CM4_REG_PSPLIM_S,
  JLINKARM_CM4_REG_MSPLIM_NS,
  JLINKARM_CM4_REG_PSPLIM_NS,
  JLINKARM_CM4_REG_CFBP_S,
  JLINKARM_CM4_REG_CFBP_NS,
  //
  // Data for CFBP pseudo regs is expected & returned
  // in same byte lane as when reading/writing the CFBP.
  // This means when reading
  //  PRIMASK,     the data of PRIMASK is expected/returned at     bits [7:0]
  //  BASEPRI,     the data of BASEPRI is expected/returned at     bits [15:8]
  //  BASEPRI_MAX, the data of BASEPRI_MAX is expected/returned at bits [15:8]
  //  FAULTMASK,   the data of FAULTMASK is expected/returned at   bits [23:16]
  //  CONTROL,     the data of CONTROL is expected/returned at     bits [31:24]
  //
  // Sample CFBP = 0xx11223344
  // Read PRIMASK   will return 0x00000044
  // Read BASEPRI   will return 0x00003300
  // Read FAULTMASK will return 0x00220000
  // Read CONTROL   will return 0x11000000
  // When writing, data is expected at the same position
  //
  JLINKARM_CM4_REG_PRIMASK_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_FAULTMASK_NS,      // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_CONTROL_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_MAX_NS,    // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_PRIMASK_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_FAULTMASK_S,       // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_CONTROL_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_MAX_S,     // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_MSPLIM,            // Either real or pseudo reg, depending on if security extensions are implemented or not
  JLINKARM_CM4_REG_PSPLIM,            // Either real or pseudo reg, depending on if security extensions are implemented or not
  JLINKARM_CM4_NUM_REGS
} JLINKARM_CM4_REG;

//
// JLINKARM_CORTEX_R4_REG
//
typedef enum {
  JLINKARM_CORTEX_R4_REG_R0,          // Index  0
  JLINKARM_CORTEX_R4_REG_R1,          // Index  1
  JLINKARM_CORTEX_R4_REG_R2,          // Index  2
  JLINKARM_CORTEX_R4_REG_R3,          // Index  3
  JLINKARM_CORTEX_R4_REG_R4,          // Index  4
  JLINKARM_CORTEX_R4_REG_R5,          // Index  5
  JLINKARM_CORTEX_R4_REG_R6,          // Index  6
  JLINKARM_CORTEX_R4_REG_R7,          // Index  7
  JLINKARM_CORTEX_R4_REG_CPSR,        // Index  8
  JLINKARM_CORTEX_R4_REG_R15,         // Index  9
  JLINKARM_CORTEX_R4_REG_R8_USR,      // Index 10
  JLINKARM_CORTEX_R4_REG_R9_USR,      // Index 11
  JLINKARM_CORTEX_R4_REG_R10_USR,     // Index 12
  JLINKARM_CORTEX_R4_REG_R11_USR,     // Index 13
  JLINKARM_CORTEX_R4_REG_R12_USR,     // Index 14
  JLINKARM_CORTEX_R4_REG_R13_USR,     // Index 15
  JLINKARM_CORTEX_R4_REG_R14_USR,     // Index 16
  JLINKARM_CORTEX_R4_REG_SPSR_FIQ,    // Index 17
  JLINKARM_CORTEX_R4_REG_R8_FIQ,      // Index 18
  JLINKARM_CORTEX_R4_REG_R9_FIQ,      // Index 19
  JLINKARM_CORTEX_R4_REG_R10_FIQ,     // Index 20
  JLINKARM_CORTEX_R4_REG_R11_FIQ,     // Index 21
  JLINKARM_CORTEX_R4_REG_R12_FIQ,     // Index 22
  JLINKARM_CORTEX_R4_REG_R13_FIQ,     // Index 23
  JLINKARM_CORTEX_R4_REG_R14_FIQ,     // Index 24
  JLINKARM_CORTEX_R4_REG_SPSR_SVC,    // Index 25
  JLINKARM_CORTEX_R4_REG_R13_SVC,     // Index 26
  JLINKARM_CORTEX_R4_REG_R14_SVC,     // Index 27
  JLINKARM_CORTEX_R4_REG_SPSR_ABT,    // Index 28
  JLINKARM_CORTEX_R4_REG_R13_ABT,     // Index 29
  JLINKARM_CORTEX_R4_REG_R14_ABT,     // Index 30
  JLINKARM_CORTEX_R4_REG_SPSR_IRQ,    // Index 31
  JLINKARM_CORTEX_R4_REG_R13_IRQ,     // Index 32
  JLINKARM_CORTEX_R4_REG_R14_IRQ,     // Index 33
  JLINKARM_CORTEX_R4_REG_SPSR_UND,    // Index 34
  JLINKARM_CORTEX_R4_REG_R13_UND,     // Index 35
  JLINKARM_CORTEX_R4_REG_R14_UND,     // Index 36
  JLINKARM_CORTEX_R4_REG_FPSID,       // Index 37
  JLINKARM_CORTEX_R4_REG_FPSCR,       // Index 38
  JLINKARM_CORTEX_R4_REG_FPEXC,       // Index 39
  JLINKARM_CORTEX_R4_REG_FPS0,        // Index 40
  JLINKARM_CORTEX_R4_REG_FPS1,        // Index 41
  JLINKARM_CORTEX_R4_REG_FPS2,        // Index 42
  JLINKARM_CORTEX_R4_REG_FPS3,        // Index 43
  JLINKARM_CORTEX_R4_REG_FPS4,        // Index 44
  JLINKARM_CORTEX_R4_REG_FPS5,        // Index 45
  JLINKARM_CORTEX_R4_REG_FPS6,        // Index 46
  JLINKARM_CORTEX_R4_REG_FPS7,        // Index 47
  JLINKARM_CORTEX_R4_REG_FPS8,        // Index 48
  JLINKARM_CORTEX_R4_REG_FPS9,        // Index 49
  JLINKARM_CORTEX_R4_REG_FPS10,       // Index 50
  JLINKARM_CORTEX_R4_REG_FPS11,       // Index 51
  JLINKARM_CORTEX_R4_REG_FPS12,       // Index 52
  JLINKARM_CORTEX_R4_REG_FPS13,       // Index 53
  JLINKARM_CORTEX_R4_REG_FPS14,       // Index 54
  JLINKARM_CORTEX_R4_REG_FPS15,       // Index 55
  JLINKARM_CORTEX_R4_REG_FPS16,       // Index 56
  JLINKARM_CORTEX_R4_REG_FPS17,       // Index 57
  JLINKARM_CORTEX_R4_REG_FPS18,       // Index 58
  JLINKARM_CORTEX_R4_REG_FPS19,       // Index 59
  JLINKARM_CORTEX_R4_REG_FPS20,       // Index 60
  JLINKARM_CORTEX_R4_REG_FPS21,       // Index 61
  JLINKARM_CORTEX_R4_REG_FPS22,       // Index 62
  JLINKARM_CORTEX_R4_REG_FPS23,       // Index 63
  JLINKARM_CORTEX_R4_REG_FPS24,       // Index 64
  JLINKARM_CORTEX_R4_REG_FPS25,       // Index 65
  JLINKARM_CORTEX_R4_REG_FPS26,       // Index 66
  JLINKARM_CORTEX_R4_REG_FPS27,       // Index 67
  JLINKARM_CORTEX_R4_REG_FPS28,       // Index 68
  JLINKARM_CORTEX_R4_REG_FPS29,       // Index 69
  JLINKARM_CORTEX_R4_REG_FPS30,       // Index 70
  JLINKARM_CORTEX_R4_REG_FPS31,       // Index 71
  JLINKARM_CORTEX_R4_REG_MVFR0,       // Index 72
  JLINKARM_CORTEX_R4_REG_MVFR1,       // Index 73
  JLINKARM_CORTEX_R4_REG_R8,          // Index 74
  JLINKARM_CORTEX_R4_REG_R9,          // Index 75
  JLINKARM_CORTEX_R4_REG_R10,         // Index 76
  JLINKARM_CORTEX_R4_REG_R11,         // Index 77
  JLINKARM_CORTEX_R4_REG_R12,         // Index 78
  JLINKARM_CORTEX_R4_REG_R13,         // Index 79
  JLINKARM_CORTEX_R4_REG_R14,         // Index 80
  JLINKARM_CORTEX_R4_REG_SPSR,        // Index 81 (0x51)
  JLINKARM_CORTEX_R4_REG_D16,         // Index 82 (0x52), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D17,         // Index 83 (0x53), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D18,         // Index 84 (0x54), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D19,         // Index 85 (0x55), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D20,         // Index 86 (0x56), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D21,         // Index 87 (0x57), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D22,         // Index 88 (0x58), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D23,         // Index 89 (0x59), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D24,         // Index 90 (0x5A), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D25,         // Index 91 (0x5B), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D26,         // Index 92 (0x5C), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D27,         // Index 93 (0x5D), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D28,         // Index 94 (0x5E), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D29,         // Index 95 (0x5F), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D30,         // Index 96 (0x60), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_REG_D31,         // Index 97 (0x61), 64-bit only. Only available on CPUs with implement VFP with D16-D31
  JLINKARM_CORTEX_R4_NUM_REGS
} JLINKARM_CORTEX_R4_REG;

//
// JLINKARM_RX_REG
//
typedef enum {
  JLINKARM_RX_REG_R0,                 // Index 0
  JLINKARM_RX_REG_R1,                 // Index 1
  JLINKARM_RX_REG_R2,                 // Index 2
  JLINKARM_RX_REG_R3,                 // Index 3
  JLINKARM_RX_REG_R4,                 // Index 4
  JLINKARM_RX_REG_R5,                 // Index 5
  JLINKARM_RX_REG_R6,                 // Index 6
  JLINKARM_RX_REG_R7,                 // Index 7
  JLINKARM_RX_REG_R8,                 // Index 8
  JLINKARM_RX_REG_R9,                 // Index 9
  JLINKARM_RX_REG_R10,                // Index 10
  JLINKARM_RX_REG_R11,                // Index 11
  JLINKARM_RX_REG_R12,                // Index 12
  JLINKARM_RX_REG_R13,                // Index 13
  JLINKARM_RX_REG_R14,                // Index 14
  JLINKARM_RX_REG_R15,                // Index 15
  JLINKARM_RX_REG_ISP,                // Index 16
  JLINKARM_RX_REG_USP,                // Index 17
  JLINKARM_RX_REG_INTB,               // Index 18
  JLINKARM_RX_REG_PC,                 // Index 19
  JLINKARM_RX_REG_PSW,                // Index 20
  JLINKARM_RX_REG_BPC,                // Index 21
  JLINKARM_RX_REG_BPSW,               // Index 22
  JLINKARM_RX_REG_FINTV,              // Index 23
  JLINKARM_RX_REG_FPSW,               // Index 24
  JLINKARM_RX_REG_CPEN,               // Index 25
  JLINKARM_RX_REG_ACCUH,              // Index 26  ACCU0[63:32]  (On RXv1 ACC[63:32] as there is only one ACC)
  JLINKARM_RX_REG_ACCUL,              // Index 27  ACCU0[31:0]   (On RXv1 ACC[31:0] as there is only one ACC)
  JLINKARM_RX_REG_ACCUE,              // Index 28  For RXv2: ACCU0[95:64], not present for RXv1
  JLINKARM_RX_REG_ACCU1H,             // Index 29  For RXv2: ACCU1[63:32], not present for RXv1
  JLINKARM_RX_REG_ACCU1L,             // Index 30  For RXv2: ACCU1[31:0], not present for RXv1
  JLINKARM_RX_REG_ACCU1E,             // Index 31  For RXv2: ACCU1[95:64], not present for RXv1
  JLINKARM_RX_REG_EXTB,               // Index 32
  JLINKARM_RX_NUM_REGS
} JLINKARM_RX_REG;

typedef enum {
  //
  //  General purpose registers:
  //
  JLINK_MIPS_REG_R0,            //    r0 (zero)      Always 0 (hard-wired)
  JLINK_MIPS_REG_R1,            //    r1 (at)        Assembler Temporary
  JLINK_MIPS_REG_R2,            //    r2 (v0)        Function Return Values
  JLINK_MIPS_REG_R3,            //    r3 (v1)        Function Return Values
  JLINK_MIPS_REG_R4,            //    r4 (a0)        Function Arguments
  JLINK_MIPS_REG_R5,            //    r5 (a1)        Function Arguments
  JLINK_MIPS_REG_R6,            //    r6 (a2)        Function Arguments
  JLINK_MIPS_REG_R7,            //    r7 (a3)        Function Arguments
  JLINK_MIPS_REG_R8,            //    r8 (t0)        Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R9,            //    r9 (t1)        Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R10,           //    r10 (t2)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R11,           //    r11 (t3)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R12,           //    r12 (t4)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R13,           //    r13 (t5)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R14,           //    r14 (t6)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R15,           //    r15 (t7)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R16,           //    r16 (s0)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R17,           //    r17 (s1)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R18,           //    r18 (s2)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R19,           //    r19 (s3)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R20,           //    r20 (s4)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R21,           //    r21 (s5)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R22,           //    r22 (s6)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R23,           //    r23 (s7)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R24,           //    r24 (t8)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R25,           //    r25 (t9)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R26,           //    r26 (k0)       Kernel temporary - Used for interrupt and exception handling
  JLINK_MIPS_REG_R27,           //    r27 (k1)       Kernel temporary - Used for interrupt and exception handling
  JLINK_MIPS_REG_R28,           //    r28 (gp)       Global Pointer - Used for fast-access common data
  JLINK_MIPS_REG_R29,           //    r29 (sp)       Stack Pointer - Software stack
  JLINK_MIPS_REG_R30,           //    r30 (s8 or fp) Saved Temporary - Caller must preserve contents OR Frame Pointer - Pointer to procedure frame on stack
  JLINK_MIPS_REG_R31,           //    r31 (ra)       Return Address (hard-wired)
  //
  // CP0 registers
  //
  JLINK_MIPS_REG_HWRENA,        // Index 32: HWREna        (CP0R7,  Sel0)
  JLINK_MIPS_REG_BADVADDR,      // Index 33: BadVAddr      (CP0R8,  Sel0)
  JLINK_MIPS_REG_COUNT,         // Index 34: Count         (CP0R9,  Sel0)
  JLINK_MIPS_REG_COMPARE,       // Index 35: Compare       (CP0R11, Sel0)
  JLINK_MIPS_REG_STATUS,        // Index 36: Status        (CP0R12, Sel0)
  JLINK_MIPS_REG_INTCTL,        // Index 37: IntCtl        (CP0R12, Sel1)
  JLINK_MIPS_REG_SRSCTL,        // Index 38: SRSCtl        (CP0R12, Sel2)
  JLINK_MIPS_REG_SRSMAP,        // Index 39: SRSMap        (CP0R12, Sel3)
  JLINK_MIPS_REG_CAUSE,         // Index 40: Cause         (CP0R13, Sel0)
  JLINK_MIPS_REG_EPC,           // Index 41: EPC           (CP0R14, Sel0)
  JLINK_MIPS_REG_PRID,          // Index 42: PRId          (CP0R15, Sel0)
  JLINK_MIPS_REG_EBASE,         // Index 43: EBASE         (CP0R15, Sel1)
  JLINK_MIPS_REG_CONFIG,        // Index 44: Config        (CP0R16, Sel0)
  JLINK_MIPS_REG_CONFIG1,       // Index 45: Config1       (CP0R16, Sel1)
  JLINK_MIPS_REG_CONFIG2,       // Index 46: Config2       (CP0R16, Sel2)
  JLINK_MIPS_REG_CONFIG3,       // Index 47: Config3       (CP0R16, Sel3)
  JLINK_MIPS_REG_DEBUG,         // Index 48: Debug         (CP0R23, Sel0)
  JLINK_MIPS_REG_TRACECONTROL,  // Index 49: TraceControl  (CP0R23, Sel1)
  JLINK_MIPS_REG_TRACECONTROL2, // Index 50: TraceControl2 (CP0R23, Sel2)
  JLINK_MIPS_REG_USERTRACEDATA, // Index 51: UserTraceData (CP0R23, Sel3)
  JLINK_MIPS_REG_TRACEBPC,      // Index 52: TraceBPC      (CP0R23, Sel4)
  JLINK_MIPS_REG_DEBUG2,        // Index 53: Debug2        (CP0R23, Sel5) CP0R23, Sel 6 on microAptiv core
  JLINK_MIPS_REG_PC,            // Index 54: DEPC          (CP0R24, Sel0)
  JLINK_MIPS_REG_ERROR_PC,      // Index 55: ErrorEPC      (CP0R30, Sel0)
  JLINK_MIPS_REG_DESAVE,        // Index 56: DESAVE        (CP0R31, Sel0)
  //
  // Special purpose registers
  //
  JLINK_MIPS_REG_HI,            // Index 57: HI
  JLINK_MIPS_REG_LO,            // Index 58: LO
  //
  // Additional registers (available on microAptiv core only)
  //
  JLINK_MIPS_REG_LO1,              // Index 59
  JLINK_MIPS_REG_LO2,              // Index 60
  JLINK_MIPS_REG_LO3,              // Index 61
  JLINK_MIPS_REG_HI1,              // Index 62
  JLINK_MIPS_REG_HI2,              // Index 63
  JLINK_MIPS_REG_HI3,              // Index 64
  JLINK_MIPS_REG_INDEX,            // Index 65: Index                 (CP0R0, Sel0)                 Index into the TLB array (microAptiv MPU only).
  JLINK_MIPS_REG_RANDOM,           // Index 66: Random                (CP0R1, Sel0)                 Randomly generated index into the TLB array (microAptiv MPU only).
  JLINK_MIPS_REG_ENTRY_LO0,        // Index 67: EntryLo0              (CP0R2, Sel0)                 Low-order portion of the TLB entry for even-numbered virtual pages (microAptiv MPU only).
  JLINK_MIPS_REG_ENTRY_LO1,        // Index 68: EntryLo1              (CP0R3, Sel0)                 Low-order portion of the TLB entry for odd-numbered virtual pages (microAptiv MPU only).
  JLINK_MIPS_REG_CONTEXT,          // Index 69: Context               (CP0R4, Sel0)                 Pointer to the page table entry in memory (microAptiv MPU only).
  JLINK_MIPS_REG_USER_LOCAL,       // Index 70: UserLocal             (CP0R4, Sel2)                 User information that can be written by privileged software and read via the RDHWR instruction.
  JLINK_MIPS_REG_PAGE_MASK,        // Index 71: PageMask              (CP0R5, Sel0)                 PageMask controls the variable page sizes in TLB entries.
  JLINK_MIPS_REG_PAGE_GRAIN,       // Index 72: PageGrain             (CP0R5, Sel1)                 PageGrain enables support of 1 KB pages in the TLB (microAptiv MPU only).
  JLINK_MIPS_REG_WIRED,            // Index 73: Wired                 (CP0R6, Sel0)                 Controls the number of fixed (i.e., wired) TLB entries (microAptiv MPU only).
  JLINK_MIPS_REG_BADINSTR,         // Index 74: BadInstr              (CP0R8, Sel1)                 Reports the instruction that caused the most recent exception
  JLINK_MIPS_REG_BADINSTRP,        // Index 75: BadInstrP             (CP0R8, Sel2)                 Reports the branch instruction if a delay slot caused the most recent exception
  JLINK_MIPS_REG_ENTRYHI,          // Index 76: EntryHi               (CP0R10, Sel0)                High-order portion of the TLB entry (microAptiv MPU only).
  JLINK_MIPS_REG_VIEWIPL,          // Index 77: View_IPL              (CP0R12, Sel4)                Allows the Priority Level to be read/written without extracting or inserting that bit from/to the Status register.
  JLINK_MIPS_REG_SRSMAP2,          // Index 78: SRSMAP2               (CP0R12, Sel5)                Contains two 4-bit fields that provide the mapping from a vector number to the shadow set number to use when servicing such an interrupt.
  JLINK_MIPS_REG_VIEW_RIPL,        // Index 79: View_RIPL             (CP0R13, Sel4)                Enables read access to the RIPL bit that is available in the Cause register.
  JLINK_MIPS_REG_NESTEDEXC,        // Index 80: NestedExc             (CP0R13, Sel5)                Contains the error and exception level status bit values that existed prior to the current exception.
  JLINK_MIPS_REG_NESTEDEPC,        // Index 81: NestedEPC             (CP0R14, Sel2)                Contains the exception program counter that existed prior to the current exception.
  JLINK_MIPS_REG_CDMMBASE,         // Index 82: CDMMBase              (CP0R15, Sel2)                Common device memory map base.
  JLINK_MIPS_REG_CONFIG4,          // Index 83: Config4               (CP0R16, Sel4)                Configuration register 4.
  JLINK_MIPS_REG_CONFIG5,          // Index 84: Config5               (CP0R16, Sel5)                Configuration register 5.
  JLINK_MIPS_REG_CONFIG7,          // Index 85: Config7               (CP0R16, Sel7)                Configuration register 7.
  JLINK_MIPS_REG_LLADDR,           // Index 86: LLAddr                (CP0R17, Sel0)                Load link address (microAptiv MPU only).
  JLINK_MIPS_REG_USERTRACEDATA2,   // Index 87: UserTraceData2        (CP0R24, Sel3)                EJTAG user trace data 2 register.
  JLINK_MIPS_REG_PERFCTL0,         // Index 88: PerfCtl0              (CP0R25, Sel0)                Performance counter 0 control.
  JLINK_MIPS_REG_PERFCNT0,         // Index 89: PerfCnt0              (CP0R25, Sel1)                Performance counter 0.
  JLINK_MIPS_REG_PERFCTL1,         // Index 90: PerfCtl1              (CP0R25, Sel2)                Performance counter 1 control.
  JLINK_MIPS_REG_PERFCNT1,         // Index 91: PerfCnt1              (CP0R25, Sel3)                Performance counter 1.
  JLINK_MIPS_REG_ERRCTL,           // Index 92: ErrCtl                (CP0R26, Sel0)                Software test enable of way-select and data RAM arrays for I-Cache and D-Cache (microAptiv MPU only).
  JLINK_MIPS_REG_CACHEERR,         // Index 93: CacheErr              (CP0R27, Sel0)                Records of information about cache/SPRAM parity errors
  JLINK_MIPS_REG_TAGLO,            // Index 94: TagLo                 (CP0R28, Sel0)                Low-order portion of cache tag interface (microAptiv MPU only).
  JLINK_MIPS_REG_DATALO,           // Index 95: DataLo                (CP0R28, Sel1)                Low-order portion of cache tag interface (microAptiv MPU only).
  JLINK_MIPS_REG_KSCRATCH1,        // Index 96: KScratch1             (CP0R31, Sel2)                Scratch Register for Kernel Mode
  JLINK_MIPS_REG_KSCRATCH2,        // Index 97: KScratch2             (CP0R31, Sel3)                Scratch Register for Kernel Mode
  JLINK_MIPS_REG_WATCHLO0,         // Index 98: WatchLo0              (CP0R18, Sel0)                Low-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHLO1,         // Index 99: WatchLo1              (CP0R18, Sel1)                Low-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHLO2,         // Index 100: WatchLo2              (CP0R18, Sel2)                Low-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHLO3,         // Index 101: WatchLo3              (CP0R18, Sel3)                Low-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHLO4,         // Index 102: WatchLo4              (CP0R18, Sel4)                Low-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHLO5,         // Index 103: WatchLo5              (CP0R18, Sel5)                Low-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHLO6,         // Index 104: WatchLo6              (CP0R18, Sel6)                Low-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHLO7,         // Index 105: WatchLo7              (CP0R18, Sel7)                Low-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHHI0,         // Index 106: WatchHi0              (CP0R19, Sel0)                High-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHHI1,         // Index 107: WatchHi1              (CP0R19, Sel1)                High-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHHI2,         // Index 108: WatchHi2              (CP0R19, Sel2)                High-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHHI3,         // Index 109: WatchHi3              (CP0R19, Sel3)                High-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHHI4,         // Index 110: WatchHi4              (CP0R19, Sel4)                High-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHHI5,         // Index 111: WatchHi5             (CP0R19, Sel5)                High-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHHI6,         // Index 112: WatchHi6             (CP0R19, Sel6)                High-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_REG_WATCHHI7,         // Index 113: WatchHi7             (CP0R19, Sel7)                High-order watchpoint address (microAptiv MPU only).
  JLINK_MIPS_NUM_REGS
} JLINK_MIPS_REG;

//
// Do NEVER change the numeric value of the register description, as certain parts of the DLL rely on this
//
#define JLINK_8051_REG_R0_B0   (0)
#define JLINK_8051_REG_R1_B0   (1)
#define JLINK_8051_REG_R2_B0   (2)
#define JLINK_8051_REG_R3_B0   (3)
#define JLINK_8051_REG_R4_B0   (4)
#define JLINK_8051_REG_R5_B0   (5)
#define JLINK_8051_REG_R6_B0   (6)
#define JLINK_8051_REG_R7_B0   (7)
#define JLINK_8051_REG_R0_B1   (8)
#define JLINK_8051_REG_R1_B1   (9)
#define JLINK_8051_REG_R2_B1  (10)
#define JLINK_8051_REG_R3_B1  (11)
#define JLINK_8051_REG_R4_B1  (12)
#define JLINK_8051_REG_R5_B1  (13)
#define JLINK_8051_REG_R6_B1  (14)
#define JLINK_8051_REG_R7_B1  (15)
#define JLINK_8051_REG_R0_B2  (16)
#define JLINK_8051_REG_R1_B2  (17)
#define JLINK_8051_REG_R2_B2  (18)
#define JLINK_8051_REG_R3_B2  (19)
#define JLINK_8051_REG_R4_B2  (20)
#define JLINK_8051_REG_R5_B2  (21)
#define JLINK_8051_REG_R6_B2  (22)
#define JLINK_8051_REG_R7_B2  (23)
#define JLINK_8051_REG_R0_B3  (24)
#define JLINK_8051_REG_R1_B3  (25)
#define JLINK_8051_REG_R2_B3  (26)
#define JLINK_8051_REG_R3_B3  (27)
#define JLINK_8051_REG_R4_B3  (28)
#define JLINK_8051_REG_R5_B3  (29)
#define JLINK_8051_REG_R6_B3  (30)
#define JLINK_8051_REG_R7_B3  (31)
#define JLINK_8051_REG_PC     (32)
#define JLINK_8051_REG_A      (33)     // Accumulator
#define JLINK_8051_REG_B      (34)
#define JLINK_8051_REG_DPTR   (35)     // Data pointer
#define JLINK_8051_REG_SP     (36)     // Stack pointer
#define JLINK_8051_REG_PSW    (37)     // Processor status word
#define JLINK_8051_REG_R0     (38)     // Pseudo register. Mapped to R0_B0/1/2/3 depending on active bank select in PSW
#define JLINK_8051_REG_R1     (39)     // Pseudo register. Mapped to R1_B0/1/2/3 depending on active bank select in PSW
#define JLINK_8051_REG_R2     (40)     // Pseudo register. Mapped to R2_B0/1/2/3 depending on active bank select in PSW
#define JLINK_8051_REG_R3     (41)     // Pseudo register. Mapped to R3_B0/1/2/3 depending on active bank select in PSW
#define JLINK_8051_REG_R4     (42)     // Pseudo register. Mapped to R4_B0/1/2/3 depending on active bank select in PSW
#define JLINK_8051_REG_R5     (43)     // Pseudo register. Mapped to R5_B0/1/2/3 depending on active bank select in PSW
#define JLINK_8051_REG_R6     (44)     // Pseudo register. Mapped to R6_B0/1/2/3 depending on active bank select in PSW
#define JLINK_8051_REG_R7     (45)     // Pseudo register. Mapped to R7_B0/1/2/3 depending on active bank select in PSW
#define JLINK_8051_NUM_REGS   (46)     // End of generic list
//
// EFM8 specific registers (start at index 47)
//
#define JLINK_EFM8_NUM_REGS     (46)   // End of EFM8 specific list
//
// BT5511 specific registers (start at index 47)
//
#define JLINK_BT5511_REG_DPS    (46)   // 8-bit, DPTR sel register. Selects either DPTR or DPTR1
#define JLINK_BT5511_REG_DPTR1  (47)   // Data pointer register 1 (24-bit)
#define JLINK_BT5511_REG_DPL    (48)   // DPTR[7:0]
#define JLINK_BT5511_REG_DPH    (49)   // DPTR[15:8]
#define JLINK_BT5511_REG_DPX    (50)   // DPTR[23:16]
#define JLINK_BT5511_REG_DPL1   (51)   // DPTR1[7:0]
#define JLINK_BT5511_REG_DPH1   (52)   // DPTR1[15:8]
#define JLINK_BT5511_REG_DPX1   (53)   // DPTR1[23:16]
#define JLINK_BT5511_REG_ACON   (54)   // ACON SFR
#define JLINK_BT5511_NUM_REGS   (55)   // End of BT5511 specific list

//
// JLINK_CF_REG
//
typedef enum {
  //
  // CPU registers
  //
  JLINK_CF_REG_D0,
  JLINK_CF_REG_D1,
  JLINK_CF_REG_D2,
  JLINK_CF_REG_D3,
  JLINK_CF_REG_D4,
  JLINK_CF_REG_D5,
  JLINK_CF_REG_D6,
  JLINK_CF_REG_D7,
  JLINK_CF_REG_A0,
  JLINK_CF_REG_A1,
  JLINK_CF_REG_A2,
  JLINK_CF_REG_A3,
  JLINK_CF_REG_A4,
  JLINK_CF_REG_A5,
  JLINK_CF_REG_A6,
  JLINK_CF_REG_A7,
  JLINK_CF_REG_OTHER_A7,
  JLINK_CF_REG_USP,                   // User Stack Pointer
  JLINK_CF_REG_SSP,                   // Supervisor Stack Pointer
  JLINK_CF_REG_SR,                    // Status Register
  JLINK_CF_REG_PC,                    // Program Counter
  JLINK_CF_REG_PC_CORRECTED,          // Program Counter Corrected
  //
  // Misc. CPU registers
  //
  JLINK_CF_REG_VBR,                   // Vector Base Register
  JLINK_CF_REG_CACR,                  // Cache Control Register
  JLINK_CF_REG_ACR0,                  // Access Control Register 0
  JLINK_CF_REG_ACR1,                  // Access Control Register 1
  JLINK_CF_REG_ACR2,                  // Access Control Register 2
  JLINK_CF_REG_ACR3,                  // Access Control Register 3
  //
  // MMU registers (only available if MMU is present)
  //
  JLINK_CF_REG_ASID,                  // Address Space Identifier
  JLINK_CF_REG_MMUBAR,                // MMU Base Address Register
  //
  // MAC registers (only available if MAC is present)
  //
  JLINK_CF_REG_MACSR,                 // MAC Status Register
  JLINK_CF_REG_MASK,                  // MAC Address Mask Register
  JLINK_CF_REG_ACC0,                  // MAC Accumulator 0
  //
  // EMAC registers (only available if EMAC is present)
  //
  JLINK_CF_REG_ACC1,                  // MAC Accumulator 1
  JLINK_CF_REG_ACC2,                  // MAC Accumulator 2
  JLINK_CF_REG_ACC3,                  // MAC Accumulator 3
  JLINK_CF_REG_ACCext01,              // MAC Accumulator 0,1 extension bytes
  JLINK_CF_REG_ACCext23,              // MAC Accumulator 2,3 extension bytes
  //
  // FPU registers (only available if FPU is present)
  //
  JLINK_CF_REG_FPU0,                  // FPU Data Register 0 (MSB)
  JLINK_CF_REG_FPL0,                  // FPU Data Register 0 (LSB)
  JLINK_CF_REG_FPU1,                  // FPU Data Register 1 (MSB)
  JLINK_CF_REG_FPL1,                  // FPU Data Register 1 (LSB)
  JLINK_CF_REG_FPU2,                  // FPU Data Register 2 (MSB)
  JLINK_CF_REG_FPL2,                  // FPU Data Register 2 (LSB)
  JLINK_CF_REG_FPU3,                  // FPU Data Register 3 (MSB)
  JLINK_CF_REG_FPL3,                  // FPU Data Register 3 (LSB)
  JLINK_CF_REG_FPU4,                  // FPU Data Register 4 (MSB)
  JLINK_CF_REG_FPL4,                  // FPU Data Register 4 (LSB)
  JLINK_CF_REG_FPU5,                  // FPU Data Register 5 (MSB)
  JLINK_CF_REG_FPL5,                  // FPU Data Register 5 (LSB)
  JLINK_CF_REG_FPU6,                  // FPU Data Register 6 (MSB)
  JLINK_CF_REG_FPL6,                  // FPU Data Register 6 (LSB)
  JLINK_CF_REG_FPU7,                  // FPU Data Register 7 (MSB)
  JLINK_CF_REG_FPL7,                  // FPU Data Register 7 (LSB)
  JLINK_CF_REG_FPIAR,                 // FPU Instruction Address Register
  JLINK_CF_REG_FPSR,                  // FPU Status Register
  JLINK_CF_REG_FPCR,                  // FPU Control Register
  JLINK_CF_NUM_REGS
} JLINK_CF_REG;

//
// JLINK_POWER_PC_REG
//
typedef enum {
  //
  // General Purpose Registers
  //
  JLINK_POWER_PC_REG_R0,              // Index 0
  JLINK_POWER_PC_REG_R1,              // Index 1
  JLINK_POWER_PC_REG_R2,              // Index 2
  JLINK_POWER_PC_REG_R3,              // Index 3
  JLINK_POWER_PC_REG_R4,              // Index 4
  JLINK_POWER_PC_REG_R5,              // Index 5
  JLINK_POWER_PC_REG_R6,              // Index 6
  JLINK_POWER_PC_REG_R7,              // Index 7
  JLINK_POWER_PC_REG_R8,              // Index 8
  JLINK_POWER_PC_REG_R9,              // Index 9
  JLINK_POWER_PC_REG_R10,             // Index 10
  JLINK_POWER_PC_REG_R11,             // Index 11
  JLINK_POWER_PC_REG_R12,             // Index 12
  JLINK_POWER_PC_REG_R13,             // Index 13
  JLINK_POWER_PC_REG_R14,             // Index 14
  JLINK_POWER_PC_REG_R15,             // Index 15
  JLINK_POWER_PC_REG_R16,             // Index 16
  JLINK_POWER_PC_REG_R17,             // Index 17
  JLINK_POWER_PC_REG_R18,             // Index 18
  JLINK_POWER_PC_REG_R19,             // Index 19
  JLINK_POWER_PC_REG_R20,             // Index 20
  JLINK_POWER_PC_REG_R21,             // Index 21
  JLINK_POWER_PC_REG_R22,             // Index 22
  JLINK_POWER_PC_REG_R23,             // Index 23
  JLINK_POWER_PC_REG_R24,             // Index 24
  JLINK_POWER_PC_REG_R25,             // Index 25
  JLINK_POWER_PC_REG_R26,             // Index 26
  JLINK_POWER_PC_REG_R27,             // Index 27
  JLINK_POWER_PC_REG_R28,             // Index 28
  JLINK_POWER_PC_REG_R29,             // Index 29
  JLINK_POWER_PC_REG_R30,             // Index 30
  JLINK_POWER_PC_REG_R31,             // Index 31
  JLINK_POWER_PC_REG_CR,              // Index 32  Condition Register
  JLINK_POWER_PC_REG_CTR,             // Index 33  Count Register
  JLINK_POWER_PC_REG_LR,              // Index 34  Link
  JLINK_POWER_PC_REG_XER,             // Index 35  XER
  JLINK_POWER_PC_REG_PC,              // Index 36
  //
  // Processor Control Registers
  //
  JLINK_POWER_PC_REG_MSR,             // Index 37  Machine State
  JLINK_POWER_PC_REG_PVR,             // Index 38  Processor Version
  JLINK_POWER_PC_REG_PIR,             // Index 39  Processor ID
  JLINK_POWER_PC_REG_SVR,             // Index 40  System Version
  JLINK_POWER_PC_REG_HID0,            // Index 41  Hardware Implementation Dependent
  JLINK_POWER_PC_REG_HID1,            // Index 42  Hardware Implementation Dependent
  //
  // Exception Handling/Control Registers
  //
  JLINK_POWER_PC_REG_SPRG0,           // Index 43  SPR General 0
  JLINK_POWER_PC_REG_SPRG1,           // Index 44  SPR General 1
  JLINK_POWER_PC_REG_SRR0,            // Index 45  Save and Restore
  JLINK_POWER_PC_REG_SRR1,            // Index 46  Save and Restore
  JLINK_POWER_PC_REG_CSRR0,           // Index 47  Save and Restore
  JLINK_POWER_PC_REG_CSRR1,           // Index 48  Save and Restore
  JLINK_POWER_PC_REG_DSRR0,           // Index 49  Save and Restore
  JLINK_POWER_PC_REG_DSRR1,           // Index 50  Save and Restore
  JLINK_POWER_PC_REG_ESR,             // Index 51  Exception Syndrome
  JLINK_POWER_PC_REG_MCSR,            // Index 52  Machine Check Syndrome Register
  JLINK_POWER_PC_REG_DEAR,            // Index 53  Data Exception Address
  JLINK_POWER_PC_REG_IVPR,            // Index 54  Interrupt Vector Prefix
  //
  // Memory Management Registers
  //
  JLINK_POWER_PC_REG_PID0,            // Index 55  Process ID
  JLINK_POWER_PC_REG_MMUCFG,          // Index 56
  //
  // Cache Registers
  //
  JLINK_POWER_PC_REG_L1CFG0,          // Index 57
  //
  // BTB Registers
  //
  JLINK_POWER_PC_REG_BUCSR,           // Index 58
  //
  // Debug Registers
  //
  JLINK_POWER_PC_REG_DBCR0,           // Index 59
  JLINK_POWER_PC_REG_DBCR1,           // Index 60
  JLINK_POWER_PC_REG_DBCR2,           // Index 61
  JLINK_POWER_PC_REG_DBSR,            // Index 62
  JLINK_POWER_PC_REG_IAC1,            // Index 63
  JLINK_POWER_PC_REG_IAC2,            // Index 64
  JLINK_POWER_PC_REG_IAC3,            // Index 65
  JLINK_POWER_PC_REG_IAC4,            // Index 66
  JLINK_POWER_PC_REG_DAC1,            // Index 67
  JLINK_POWER_PC_REG_DAC2,            // Index 68
  JLINK_POWER_PC_NUM_REGS
} JLINK_POWER_PC_REG;

//
// JLINK_RISCV_REG
//
typedef enum {
  JLINK_RISCV_REG_FFLAGS = 0x001,          // Bits [4:0] of FCSR
  JLINK_RISCV_REG_FRM,                     // Bits [7:5] of FCSR
  JLINK_RISCV_REG_FCSR,                    // Always 32-bit
  JLINK_RISCV_REG_USTATUS = 0x000,         // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_UIE     = 0x004,         // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_UTVEC   = 0x005,         // Length = ???
  JLINK_RISCV_REG_USCRATCH = 0x040,        // Length = ???
  JLINK_RISCV_REG_UEPC,                    // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_UCAUSE,                  // Length = ???
  JLINK_RISCV_REG_UTVAL,                   // Length = ???
  JLINK_RISCV_REG_UIP,                     // 32/64-bit. Depends on RV32/64
  //
  // Gap
  //
  JLINK_RISCV_REG_SSTATUS = 0x100,         // 32/64-bit. Depends on RV32/64
  //
  // Gap
  //
  JLINK_RISCV_REG_SEDELEG = 0x102,         // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_SIDELEG,                 // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_SIE,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_STVEC,                   // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_SCOUNTEREN,              // Always 32-bit
  //
  // Gap
  //
  JLINK_RISCV_REG_SSCRATCH = 0x140,        // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_SEPC,                    // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_SCAUSE,                  // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_STVAL,                   // 32/64-bit. Depends on RV32/64 Also called "SBADADDR" in some older manuals.
  JLINK_RISCV_REG_SIP,                     // 32/64-bit. Depends on RV32/64
  //
  // Gap
  //
  JLINK_RISCV_REG_SATP = 0x0180,           // 32/64-bit. Depends on RV32/64 Also called "SPTBR" in some older manuals.
  //
  // Gap
  //
  JLINK_RISCV_REG_MSTATUS = 0x300,         // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MISA,                    // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MEDELEG,                 // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MIDELEG,                 // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MIE,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MTVEC,                   // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MCOUNTEREN,              // Always 32-bit
  //
  // Gap
  //
  JLINK_RISCV_REG_MHPMEVENT3 = 0x323,      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT4,              // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT5,              // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT6,              // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT7,              // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT8,              // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT9,              // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT10,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT11,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT12,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT13,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT14,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT15,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT16,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT17,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT18,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT19,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT20,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT21,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT22,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT23,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT24,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT25,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT26,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT27,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT28,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT29,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT30,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHPMEVENT31,             // 32/64-bit. Depends on RV32/64
  //
  // Gap
  //
  JLINK_RISCV_REG_MSCRATCH = 0x340,        // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MEPC,                    // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MCAUSE,                  // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MTVAL,                   // Also called "MBADADDR" in some older manuals.
  JLINK_RISCV_REG_MIP,                     // 32/64-bit. Depends on RV32/64
  //
  // Gap
  //
  JLINK_RISCV_REG_PMPCFG0 = 0x3A0,         // Always 32-bit
  JLINK_RISCV_REG_PMPCFG1,                 // Always 32-bit
  JLINK_RISCV_REG_PMPCFG2,                 // Always 32-bit
  JLINK_RISCV_REG_PMPCFG3,                 // Always 32-bit
  //
  // Gap
  //
  JLINK_RISCV_REG_PMPADDR0 = 0x3B0,        // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR1,                // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR2,                // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR3,                // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR4,                // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR5,                // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR6,                // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR7,                // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR8,                // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR9,                // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR10,               // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR11,               // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR12,               // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR13,               // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR14,               // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_PMPADDR15,               // 32/64-bit. Depends on RV32/64
  //
  // Gap
  //
  JLINK_RISCV_REG_TSELECT = 0x7A0,         // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_TDATA1,                  // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_TDATA2,                  // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_TDATA3,                  // 32/64-bit. Depends on RV32/64
  //
  // Gap
  //
  JLINK_RISCV_REG_DCSR = 0x7B0,            // Always 32-bit
  JLINK_RISCV_REG_DPC,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_DSCRATCH,                // ???
  //
  // Gap
  //
  JLINK_RISCV_REG_MCYCLE   = 0xB00,        // Always 64bit. For RV32, higher bits can be accessed via MCYCLEH
  //
  // Gap
  //
  JLINK_RISCV_REG_MINSTRET = 0xB02,        // Always 64bit. For RV32, higher bits can be accessed via MINSTRETH
  JLINK_RISCV_REG_MHPMCOUNTER3,            // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER4,            // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER5,            // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER6,            // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER7,            // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER8,            // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER9,            // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER10,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER11,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER12,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER13,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER14,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER15,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER16,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER17,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER18,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER19,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER20,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER21,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER22,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER23,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER24,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER25,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER26,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER27,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER28,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER29,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER30,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  JLINK_RISCV_REG_MHPMCOUNTER31,           // Always 64bit. For RV32, higher bits can be accessed via MHPMCOUNTERxH
  //
  // Gap
  //
  JLINK_RISCV_REG_MCYCLEH   = 0xB80,       // Higher 32-bits of MCYCLE (needed for RV32)
  //
  // Gap
  //
  JLINK_RISCV_REG_MINSTRETH = 0xB82,       // Higher 32-bits of MINSTRET (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER3H,           // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER4H,           // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER5H,           // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER6H,           // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER7H,           // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER8H,           // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER9H,           // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER10H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER11H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER12H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER13H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER14H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER15H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER16H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER17H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER18H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER19H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER20H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER21H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER22H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER23H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER24H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER25H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER26H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER27H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER28H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER29H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER30H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  JLINK_RISCV_REG_MHPMCOUNTER31H,          // Higher 32-bits of MHPMCOUNTERx (needed for RV32)
  //
  // Gap
  //
  JLINK_RISCV_REG_CYCLE = 0xC00,           // Always 64-bit. For RV32, higher bits can be accessed via CYCLEH
  JLINK_RISCV_REG_TIME,                    // Always 64-bit. For RV32, higher bits can be accessed via TIMEH
  JLINK_RISCV_REG_INSTRET,                 // Always 64-bit. For RV32, higher bits can be accessed via INSTRETH
  JLINK_RISCV_REG_HPMCOUNTER3,             // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER4,             // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER5,             // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER6,             // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER7,             // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER8,             // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER9,             // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER10,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER11,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER12,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER13,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER14,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER15,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER16,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER17,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER18,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER19,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER20,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER21,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER22,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER23,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER24,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER25,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER26,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER27,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER28,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER29,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER30,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  JLINK_RISCV_REG_HPMCOUNTER31,            // Always 64bit. For RV32, higher bits can be accessed via HPMCOUNTERxH
  //
  // Gap
  //
  JLINK_RISCV_REG_CYCLEH = 0xC80,          // Higher 32-bit of CYCLE (needed for RV32)
  JLINK_RISCV_REG_TIMEH,                   // Higher 32-bit of TIME (needed for RV32)
  JLINK_RISCV_REG_INSTRETH,                // Higher 32-bit of INSTRET (needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER3H,            // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER4H,            // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER5H,            // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER6H,            // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER7H,            // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER8H,            // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER9H,            // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER10H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER11H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER12H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER13H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER14H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER15H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER16H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER17H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER18H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER19H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER20H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER21H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER22H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER23H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER24H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER25H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER26H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER27H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER28H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER29H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER30H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  JLINK_RISCV_REG_HPMCOUNTER31H,           // Higher 32-bits of HPMCOUNTERx (Needed for RV32)
  //
  // Gap
  //
  JLINK_RISCV_REG_MVENDORID = 0xF11,       // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MARCHID,                 // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MIMPID,                  // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_MHARTID,                 // 32/64-bit. Depends on RV32/64
  //
  // Gap
  //
  JLINK_RISCV_REG_X0 = 0x1000,             // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X1,                      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X2,                      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X3,                      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X4,                      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X5,                      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X6,                      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X7,                      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X8,                      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X9,                      // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X10,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X11,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X12,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X13,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X14,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X15,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X16,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X17,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X18,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X19,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X20,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X21,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X22,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X23,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X24,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X25,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X26,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X27,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X28,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X29,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X30,                     // 32/64-bit. Depends on RV32/64
  JLINK_RISCV_REG_X31,                     // 32/64-bit. Depends on RV32/64
  //
  // Gap
  //
  JLINK_RISCV_REG_F0 = 0x1020,             // Length depends on length of floating point unit
  JLINK_RISCV_REG_F1,                      // Length depends on length of floating point unit
  JLINK_RISCV_REG_F2,                      // Length depends on length of floating point unit
  JLINK_RISCV_REG_F3,                      // Length depends on length of floating point unit
  JLINK_RISCV_REG_F4,                      // Length depends on length of floating point unit
  JLINK_RISCV_REG_F5,                      // Length depends on length of floating point unit
  JLINK_RISCV_REG_F6,                      // Length depends on length of floating point unit
  JLINK_RISCV_REG_F7,                      // Length depends on length of floating point unit
  JLINK_RISCV_REG_F8,                      // Length depends on length of floating point unit
  JLINK_RISCV_REG_F9,                      // Length depends on length of floating point unit
  JLINK_RISCV_REG_F10,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F11,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F12,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F13,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F14,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F15,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F16,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F17,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F18,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F19,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F20,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F21,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F22,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F23,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F24,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F25,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F26,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F27,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F28,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F29,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F30,                     // Length depends on length of floating point unit
  JLINK_RISCV_REG_F31,                     // Length depends on length of floating point unit
  //
  // Gap
  //
  JLINK_RISCV_REG_V0 = 0x1040,             // Length = ???
  JLINK_RISCV_REG_V1,                      // Length = ???
  JLINK_RISCV_REG_V2,                      // Length = ???
  JLINK_RISCV_REG_V3,                      // Length = ???
  JLINK_RISCV_REG_V4,                      // Length = ???
  JLINK_RISCV_REG_V5,                      // Length = ???
  JLINK_RISCV_REG_V6,                      // Length = ???
  JLINK_RISCV_REG_V7,                      // Length = ???
  JLINK_RISCV_REG_V8,                      // Length = ???
  JLINK_RISCV_REG_V9,                      // Length = ???
  JLINK_RISCV_REG_V10,                     // Length = ???
  JLINK_RISCV_REG_V11,                     // Length = ???
  JLINK_RISCV_REG_V12,                     // Length = ???
  JLINK_RISCV_REG_V13,                     // Length = ???
  JLINK_RISCV_REG_V14,                     // Length = ???
  JLINK_RISCV_REG_V15,                     // Length = ???
  JLINK_RISCV_REG_V16,                     // Length = ???
  JLINK_RISCV_REG_V17,                     // Length = ???
  JLINK_RISCV_REG_V18,                     // Length = ???
  JLINK_RISCV_REG_V19,                     // Length = ???
  JLINK_RISCV_REG_V20,                     // Length = ???
  JLINK_RISCV_REG_V21,                     // Length = ???
  JLINK_RISCV_REG_V22,                     // Length = ???
  JLINK_RISCV_REG_V23,                     // Length = ???
  JLINK_RISCV_REG_V24,                     // Length = ???
  JLINK_RISCV_REG_V25,                     // Length = ???
  JLINK_RISCV_REG_V26,                     // Length = ???
  JLINK_RISCV_REG_V27,                     // Length = ???
  JLINK_RISCV_REG_V28,                     // Length = ???
  JLINK_RISCV_REG_V29,                     // Length = ???
  JLINK_RISCV_REG_V30,                     // Length = ???
  JLINK_RISCV_REG_V31,                     // Length = ???
  JLINK_RISCV_REG_VP0,                     // Length = ???
  JLINK_RISCV_REG_VP1,                     // Length = ???
  JLINK_RISCV_REG_VP2,                     // Length = ???
  JLINK_RISCV_REG_VP3,                     // Length = ???
  JLINK_RISCV_REG_VP4,                     // Length = ???
  JLINK_RISCV_REG_VP5,                     // Length = ???
  JLINK_RISCV_REG_VP6,                     // Length = ???
  JLINK_RISCV_REG_VP7,                     // Length = ???
  //
  // Gap
  //
  JLINK_RISCV_REG_PC = 0x1080,             // 32/64-bit. Depends on RV32/64
  // Reg indexes 0x10000 - 0x1FFFF must not be used for RISC-V. They are reserved for DLL internal use
  JLINK_RISCV_NUM_REGS
} JLINK_RISCV_REG;

//
// Registers for ARMv8-AR architecture (Cortex-A32, A53, A57, A72, ...)
// For registers > 32-bit, using the old Read/Write Register functions only the lower 32-bit can be read/written
// For writes, this would result in the upper 32-bits being written as 0s
// It is recommended to use the new 64-bit Read/Write register functions
//
typedef enum {
  //
  // GPR, PC, SP, ... (64-bit regs)
  // *_EL12    => EL12 aliases of EL1 special-purpose registers (See [1] C5.1.5 Moves to and from Special-purpose registers)
  // SPSR_EL12 => Accesses SPSR_EL1
  // ELR_EL12  => Accesses ELR_EL1
  // Not listed here as they do not make any sense within J-Link...
  //
  // SP_EL3 => For now it returns the same value as SP_EL2 because neither the ARM doc nor the GCC assembler have an instruction for SP_EL3...
  //
  JLINK_ARM_V8AR_REG_R0,
  JLINK_ARM_V8AR_REG_R1,
  JLINK_ARM_V8AR_REG_R2,
  JLINK_ARM_V8AR_REG_R3,
  JLINK_ARM_V8AR_REG_R4,
  JLINK_ARM_V8AR_REG_R5,
  JLINK_ARM_V8AR_REG_R6,
  JLINK_ARM_V8AR_REG_R7,
  JLINK_ARM_V8AR_REG_R8,
  JLINK_ARM_V8AR_REG_R9,
  JLINK_ARM_V8AR_REG_R10,
  JLINK_ARM_V8AR_REG_R11,
  JLINK_ARM_V8AR_REG_R12,
  JLINK_ARM_V8AR_REG_R13,
  JLINK_ARM_V8AR_REG_R14,
  JLINK_ARM_V8AR_REG_R15,
  JLINK_ARM_V8AR_REG_R16,
  JLINK_ARM_V8AR_REG_R17,
  JLINK_ARM_V8AR_REG_R18,
  JLINK_ARM_V8AR_REG_R19,
  JLINK_ARM_V8AR_REG_R20,
  JLINK_ARM_V8AR_REG_R21,
  JLINK_ARM_V8AR_REG_R22,
  JLINK_ARM_V8AR_REG_R23,
  JLINK_ARM_V8AR_REG_R24,
  JLINK_ARM_V8AR_REG_R25,
  JLINK_ARM_V8AR_REG_R26,
  JLINK_ARM_V8AR_REG_R27,
  JLINK_ARM_V8AR_REG_R28,
  JLINK_ARM_V8AR_REG_R29,
  JLINK_ARM_V8AR_REG_R30,
  JLINK_ARM_V8AR_REG_R31,
  JLINK_ARM_V8AR_REG_SP,
  JLINK_ARM_V8AR_REG_PC,
  JLINK_ARM_V8AR_REG_ELR_EL1,
  JLINK_ARM_V8AR_REG_ELR_EL2,
  JLINK_ARM_V8AR_REG_ELR_EL3,
  JLINK_ARM_V8AR_REG_SP_EL0,
  JLINK_ARM_V8AR_REG_SP_EL1,
  JLINK_ARM_V8AR_REG_SP_EL2,
  JLINK_ARM_V8AR_REG_SP_EL3,
  JLINK_ARM_V8AR_REG_CPSR,
  JLINK_ARM_V8AR_REG_SPSR_abt,
  JLINK_ARM_V8AR_REG_SPSR_EL1,
  JLINK_ARM_V8AR_REG_SPSR_EL2,
  JLINK_ARM_V8AR_REG_SPSR_EL3,
  JLINK_ARM_V8AR_REG_SPSR_fiq,
  JLINK_ARM_V8AR_REG_SPSR_irq,
  JLINK_ARM_V8AR_REG_SPSR_und,
  //
  // Floating point (32-bit)
  //
  JLINK_ARM_V8AR_REG_FPCR,
  JLINK_ARM_V8AR_REG_FPSR,
  //
  // Floating point (128-bit, split into LOW/HIGH 64-bit parts)
  //
  JLINK_ARM_V8AR_REG_V0_LOW,
  JLINK_ARM_V8AR_REG_V0_HIGH,
  JLINK_ARM_V8AR_REG_V1_LOW,
  JLINK_ARM_V8AR_REG_V1_HIGH,
  JLINK_ARM_V8AR_REG_V2_LOW,
  JLINK_ARM_V8AR_REG_V2_HIGH,
  JLINK_ARM_V8AR_REG_V3_LOW,
  JLINK_ARM_V8AR_REG_V3_HIGH,
  JLINK_ARM_V8AR_REG_V4_LOW,
  JLINK_ARM_V8AR_REG_V4_HIGH,
  JLINK_ARM_V8AR_REG_V5_LOW,
  JLINK_ARM_V8AR_REG_V5_HIGH,
  JLINK_ARM_V8AR_REG_V6_LOW,
  JLINK_ARM_V8AR_REG_V6_HIGH,
  JLINK_ARM_V8AR_REG_V7_LOW,
  JLINK_ARM_V8AR_REG_V7_HIGH,
  JLINK_ARM_V8AR_REG_V8_LOW,
  JLINK_ARM_V8AR_REG_V8_HIGH,
  JLINK_ARM_V8AR_REG_V9_LOW,
  JLINK_ARM_V8AR_REG_V9_HIGH,
  JLINK_ARM_V8AR_REG_V10_LOW,
  JLINK_ARM_V8AR_REG_V10_HIGH,
  JLINK_ARM_V8AR_REG_V11_LOW,
  JLINK_ARM_V8AR_REG_V11_HIGH,
  JLINK_ARM_V8AR_REG_V12_LOW,
  JLINK_ARM_V8AR_REG_V12_HIGH,
  JLINK_ARM_V8AR_REG_V13_LOW,
  JLINK_ARM_V8AR_REG_V13_HIGH,
  JLINK_ARM_V8AR_REG_V14_LOW,
  JLINK_ARM_V8AR_REG_V14_HIGH,
  JLINK_ARM_V8AR_REG_V15_LOW,
  JLINK_ARM_V8AR_REG_V15_HIGH,
  JLINK_ARM_V8AR_REG_V16_LOW,
  JLINK_ARM_V8AR_REG_V16_HIGH,
  JLINK_ARM_V8AR_REG_V17_LOW,
  JLINK_ARM_V8AR_REG_V17_HIGH,
  JLINK_ARM_V8AR_REG_V18_LOW,
  JLINK_ARM_V8AR_REG_V18_HIGH,
  JLINK_ARM_V8AR_REG_V19_LOW,
  JLINK_ARM_V8AR_REG_V19_HIGH,
  JLINK_ARM_V8AR_REG_V20_LOW,
  JLINK_ARM_V8AR_REG_V20_HIGH,
  JLINK_ARM_V8AR_REG_V21_LOW,
  JLINK_ARM_V8AR_REG_V21_HIGH,
  JLINK_ARM_V8AR_REG_V22_LOW,
  JLINK_ARM_V8AR_REG_V22_HIGH,
  JLINK_ARM_V8AR_REG_V23_LOW,
  JLINK_ARM_V8AR_REG_V23_HIGH,
  JLINK_ARM_V8AR_REG_V24_LOW,
  JLINK_ARM_V8AR_REG_V24_HIGH,
  JLINK_ARM_V8AR_REG_V25_LOW,
  JLINK_ARM_V8AR_REG_V25_HIGH,
  JLINK_ARM_V8AR_REG_V26_LOW,
  JLINK_ARM_V8AR_REG_V26_HIGH,
  JLINK_ARM_V8AR_REG_V27_LOW,
  JLINK_ARM_V8AR_REG_V27_HIGH,
  JLINK_ARM_V8AR_REG_V28_LOW,
  JLINK_ARM_V8AR_REG_V28_HIGH,
  JLINK_ARM_V8AR_REG_V29_LOW,
  JLINK_ARM_V8AR_REG_V29_HIGH,
  JLINK_ARM_V8AR_REG_V30_LOW,
  JLINK_ARM_V8AR_REG_V30_HIGH,
  JLINK_ARM_V8AR_REG_V31_LOW,
  JLINK_ARM_V8AR_REG_V31_HIGH,
  //
  // Encoding of REG_MISC:
  // [63:32]  Reserved for future use (currently 0)
  // [31:4]  Reserved for future use (currently 0)
  // [3:0]   ELAArchSupport
  //
  // ELAArchSupport:
  //   All: AArch32
  //   All: AArch32
  //   All: AArch32
  //   All: AArch32
  //   All: AArch32
  //   All: AArch32
  //   All: AArch32
  //   All: AArch32
  //   EL0,1,2: AArch32. Others: AArch64
  //   EL0,1,2: AArch32. Others: AArch64
  //   EL0,1,2: AArch32. Others: AArch64
  //   EL0,1,2: AArch32. Others: AArch64
  //   EL0,1: AArch32. Others: AArch64
  //   EL0,1: AArch32. Others: AArch64
  //   EL0: AArch32. Others: AArch64
  //   All: AArch64
  //
  JLINK_ARM_V8AR_REG_JMISC,     // J-Link specific 64-bit register which is used to add some kind of extra info that is usually only visible via the debug regs.
  //
  // Pseudo registers for AArch32 compatibility mode
  // DLL will map them internally to AArch64 regs
  //
  JLINK_ARM_V8AR_REG_AARCH32_R0,
  JLINK_ARM_V8AR_REG_AARCH32_R1,
  JLINK_ARM_V8AR_REG_AARCH32_R2,
  JLINK_ARM_V8AR_REG_AARCH32_R3,
  JLINK_ARM_V8AR_REG_AARCH32_R4,
  JLINK_ARM_V8AR_REG_AARCH32_R5,
  JLINK_ARM_V8AR_REG_AARCH32_R6,
  JLINK_ARM_V8AR_REG_AARCH32_R7,
  JLINK_ARM_V8AR_REG_AARCH32_R8_USR,
  JLINK_ARM_V8AR_REG_AARCH32_R9_USR,
  JLINK_ARM_V8AR_REG_AARCH32_R10_USR,
  JLINK_ARM_V8AR_REG_AARCH32_R11_USR,
  JLINK_ARM_V8AR_REG_AARCH32_R12_USR,
  JLINK_ARM_V8AR_REG_AARCH32_R13_USR,
  JLINK_ARM_V8AR_REG_AARCH32_R14_USR,
  JLINK_ARM_V8AR_REG_AARCH32_R13_HYP,
  JLINK_ARM_V8AR_REG_AARCH32_R13_IRQ,
  JLINK_ARM_V8AR_REG_AARCH32_R14_IRQ,
  JLINK_ARM_V8AR_REG_AARCH32_R13_SVC,
  JLINK_ARM_V8AR_REG_AARCH32_R14_SVC,
  JLINK_ARM_V8AR_REG_AARCH32_R13_ABT,
  JLINK_ARM_V8AR_REG_AARCH32_R14_ABT,
  JLINK_ARM_V8AR_REG_AARCH32_R13_UND,
  JLINK_ARM_V8AR_REG_AARCH32_R14_UND,
  JLINK_ARM_V8AR_REG_AARCH32_R8_FIQ,
  JLINK_ARM_V8AR_REG_AARCH32_R9_FIQ,
  JLINK_ARM_V8AR_REG_AARCH32_R10_FIQ,
  JLINK_ARM_V8AR_REG_AARCH32_R11_FIQ,
  JLINK_ARM_V8AR_REG_AARCH32_R12_FIQ,
  JLINK_ARM_V8AR_REG_AARCH32_R13_FIQ,
  JLINK_ARM_V8AR_REG_AARCH32_R14_FIQ,
  JLINK_ARM_V8AR_REG_AARCH32_PC,
  JLINK_ARM_V8AR_REG_AARCH32_ELR_HYP,
  JLINK_ARM_V8AR_REG_AARCH32_CPSR,
  JLINK_ARM_V8AR_REG_AARCH32_SPSR_ABT,
  JLINK_ARM_V8AR_REG_AARCH32_SPSR_SVC,
  JLINK_ARM_V8AR_REG_AARCH32_SPSR_HYP,
  JLINK_ARM_V8AR_REG_AARCH32_SPSR_FIQ,
  JLINK_ARM_V8AR_REG_AARCH32_SPSR_IRQ,
  JLINK_ARM_V8AR_REG_AARCH32_SPSR_UND,
  JLINK_ARM_V8AR_REG_AARCH32_FPSCR,
  JLINK_ARM_V8AR_REG_AARCH32_Q0_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q0_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q1_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q1_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q2_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q2_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q3_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q3_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q4_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q4_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q5_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q5_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q6_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q6_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q7_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q7_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q8_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q8_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q9_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q9_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q10_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q10_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q11_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q11_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q12_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q12_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q13_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q13_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q14_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q14_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_Q15_LOW,
  JLINK_ARM_V8AR_REG_AARCH32_Q15_HIGH,
  JLINK_ARM_V8AR_REG_AARCH32_R8,         // Current registers (automatically mapped to USR, IRQ, FIQ, ... by DLL)
  JLINK_ARM_V8AR_REG_AARCH32_R9,
  JLINK_ARM_V8AR_REG_AARCH32_R10,
  JLINK_ARM_V8AR_REG_AARCH32_R11,
  JLINK_ARM_V8AR_REG_AARCH32_R12,
  JLINK_ARM_V8AR_REG_AARCH32_R13,
  JLINK_ARM_V8AR_REG_AARCH32_R14,
  JLINK_ARM_V8AR_NUM_REGS
} JLINK_ARM_V8AR_REG;

//
// JLINK_STRACE_EVENT_INFO
//
typedef struct {
  U32 SizeofStruct;                   // Filled in by user
  U8  Type;                           // Of type JLINK_STRACE_EVENT_TYPE_
  U8  Op;                             // Of type JLINK_STRACE_OP_
  U8  AccessSize;
  U8  Reserved0;
  U64 Addr;
  U64 Data;
  U64 DataMask;
  U32 AddrRangeSize;
  //
  // Everything from here on was not present in the initial version of this struct and needs to be checked to be present via SizeofStruct
  //
} JLINK_STRACE_EVENT_INFO;

#define JLINK_STRACE_ERR_UNKNOWN_ADDR     (0xFFFFFFF0)
#define JLINK_STRACE_ERR_OVERFLOW         (0xFFFFFFF1)

//
// JLINK_STRACE_INST_STAT
//
typedef struct {
  U64 ExecCnt;
} JLINK_STRACE_INST_STAT;

#define JLINK_STRACE_CNT_TYPE_FETCHED             ( 0)         // Get counter of how many times the instruction at <Addr> was fetched.
#define JLINK_STRACE_CNT_TYPE_EXEC                ( 1)         // Get counter of how many times the instruction at <Addr> was fetched and executed.
#define JLINK_STRACE_CNT_TYPE_SKIP                ( 2)         // Get counter of how many times the instruction at <Addr> was fetched but not executed (skipped).

#define JLINK_STRACE_CNT_TYPE_TOTAL_FETCHED       ( 4)         // Sum of all fetch counts of all addresses we have so far. <Addr> parameter is don't care.
#define JLINK_STRACE_CNT_TYPE_TOTAL_EXEC          ( 5)         // Sum of all exec counts of all addresses we have so far. <Addr> parameter is don't care.
#define JLINK_STRACE_CNT_TYPE_TOTAL_SKIP          ( 6)         // Sum of all skip counts of all addresses we have so far. <Addr> parameter is don't care.
#define JLINK_STRACE_CNT_TYPE_TOTAL_NUM_INSTS     ( 7)         // Number of instructions which were ever fetched of all addresses we have so far. <Addr> parameter is don't care.
#define JLINK_STRACE_CNT_TYPE_FETCHED_EXEC        ( 8)         // Get the fetched and exec counters + their totals. Format: Fetch counters | fetch count total | exec counters | exec count total
#define JLINK_STRACE_CNT_TYPE_FETCHED_SKIP        ( 9)         // Get the fetched and skip counters + their totals. Format: Fetch counters | fetch count total | skip counters | skip count total
#define JLINK_STRACE_CNT_TYPE_EXEC_SKIP           (10)         // Get the exec and skip counters + their totals. Format: Exec counters | exec count total | skip counters | skip count total
#define JLINK_STRACE_CNT_TYPE_FETCHED_EXEC_SKIP   (11)         // Get the fetched, exec and skip counters + their totals. Format: Fetch counters | fetch count total | exec counters | exec count total | skip counters | skip count total
//
// Indices for JLINKARM_GetDebugInfo()
// Coresight infos start at index 0x100
//
#define JLINKARM_DEBUG_INFO_HAS_FPU_INDEX                   (0x001)

#define JLINKARM_ROM_TABLE_ADDR_INDEX                       (0x100)
#define JLINKARM_DEBUG_INFO_ETM_ADDR_INDEX                  (0x101)
#define JLINKARM_DEBUG_INFO_MTB_ADDR_INDEX                  (0x102)
#define JLINKARM_DEBUG_INFO_TPIU_ADDR_INDEX                 (0x103)
#define JLINKARM_DEBUG_INFO_ITM_ADDR_INDEX                  (0x104)
#define JLINKARM_DEBUG_INFO_DWT_ADDR_INDEX                  (0x105)
#define JLINKARM_DEBUG_INFO_FPB_ADDR_INDEX                  (0x106)
#define JLINKARM_DEBUG_INFO_NVIC_ADDR_INDEX                 (0x107)
#define JLINKARM_DEBUG_INFO_TMC_ADDR_INDEX                  (0x108)
#define JLINKARM_DEBUG_INFO_TF_ADDR_INDEX                   (0x109)
#define JLINKARM_DEBUG_INFO_PTM_ADDR_INDEX                  (0x10A)
#define JLINKARM_DEBUG_INFO_ETB_ADDR_INDEX                  (0x10B)
#define JLINKARM_DEBUG_INFO_DBG_ADDR_INDEX                  (0x10C)
#define JLINKARM_DEBUG_INFO_APBAP_INDEX                     (0x10D)  // Only valid before CoreSight SoC-600. Use JLINKARM_DEBUG_INFO_APBAP/AHBAP_ADDRESS for SoC-600 and later
#define JLINKARM_DEBUG_INFO_AHBAP_INDEX                     (0x10E)  // Only valid before CoreSight SoC-600. Use JLINKARM_DEBUG_INFO_APBAP/AHBAP_ADDRESS for SoC-600 and later
#define JLINKARM_DEBUG_INFO_HAS_CORTEX_M_SECURITY_EXT_INDEX (0x10F)
#define JLINKARM_DEBUG_INFO_APBAP_ADDRESS                   (0x10D)
#define JLINKARM_DEBUG_INFO_DEBUG_AP_ADDRESS                (0x110)
//
// ARM_ENDIAN
//
typedef enum {
  ARM_ENDIAN_LITTLE, ARM_ENDIAN_BIG
} ARM_ENDIAN;

//
// ARM_RESET_TYPE
//
typedef enum {
  JLINKARM_RESET_TYPE_NORMAL = 0,    // Resets core + peripherals. Reset pin is avoided where possible and reset via SFR access is preferred.
  //
  // --- Start ---
  // Do NOT use anymore
  //
  JLINKARM_RESET_TYPE_BP0,
  JLINKARM_RESET_TYPE_ADI,
  JLINKARM_RESET_TYPE_NO_RESET,
  JLINKARM_RESET_TYPE_HALT_WP,
  JLINKARM_RESET_TYPE_HALT_DBGRQ,
  JLINKARM_RESET_TYPE_SOFT,
  JLINKARM_RESET_TYPE_HALT_DURING,
  JLINKARM_RESET_TYPE_SAM7,
  JLINKARM_RESET_TYPE_LPC,
  //
  // --- End ---
  //
  //
  // Generic J-Link reset types (core independent)
  // CPU-specific reset types are still in the header for backward compatibility but should not be used anymore
  // All reset types halt the CPU before executing the first instruction of the user application, after reset release
  // If the CPU incorporates a ROM bootloader, J-Link makes sure that this bootloader is executed and the CPU is halted as soon as it jumps into the user application code
  //
  // Note:
  // If a specific reset type also resets the debug logic, it may happen that the CPU cannot be halted immediately after reset
  // so it may have already executed some instructions before J-Link has a chance to halt it
  //
  JLINK_RESET_TYPE_CORE=100,         // Resets core only
  JLINK_RESET_TYPE_RESET_PIN         // Toggles reset pin in order to issue a reset. Requires reset pin to be connected, otherwise result will be unpredictable
} JLINKARM_RESET_TYPE;

typedef enum {
  JLINK_PCODE_OP_RESET=0,   // Deprecated, use JLINK_PCODE_OP_PCODE_DEVICE instead which gets *one* PCode with *multiple* entry points for connect, reset, etc.
  JLINK_PCODE_OP_CONNECT,   // Deprecated, use JLINK_PCODE_OP_PCODE_DEVICE instead which gets *one* PCode with *multiple* entry points for connect, reset, etc.
  JLINK_PCODE_OP_UNSECURE,  // Deprecated, use JLINK_PCODE_OP_PCODE_DEVICE instead which gets *one* PCode with *multiple* entry points for connect, reset, etc.
  JLINK_PCODE_OP_SETUP,     // Deprecated, use JLINK_PCODE_OP_PCODE_DEVICE instead which gets *one* PCode with *multiple* entry points for connect, reset, etc.
  JLINK_PCODE_OP_PCODE_DEVICE,
  JLINK_PCODE_OP_NUM_OPERATIONS
} JLINK_PCODE_OPERATIONS;

//
// SWO Conversion Mode
//
//
#define JLINK_SWO_CONVERSION_MODE_WINDOWS  0    // If only '\n' is received, make it "\r\n" to make the line end Windows-compliant. (Default behavior)
#define JLINK_SWO_CONVERSION_MODE_NONE     1    // Leave everything as it is, do not add any characters

//
// JLINKARM_INDICATOR_CTRL
//
typedef struct {
  U16 IndicatorId;
  U16 Override;               // 1: Controlled by Host (PC), 0: controlled by emulator (default)
  U16 InitialOnTime;          // [1ms]
  U16 OnTime;                 // [1ms]
  U16 OffTime;                // [1ms]
} JLINKARM_INDICATOR_CTRL;

//
// JTAG_ID_DATA
//
typedef struct {
  int NumDevices;             // Number of devices in this scan chain
  U16 ScanLen;                // Total number of bits in all scan chain select register
  U32 aId      [3];
  U8  aScanLen [3];           // Number of bits in individual scan chain select registers
  U8  aIrRead  [3];           // Data read back from instruction register
  U8  aScanRead[3];           // Data read back from scan chain select register
} JTAG_ID_DATA;

//
// JLINKARM_BP_INFO
//
typedef struct {
  U32   SizeOfStruct;         // In
  U32   Handle;               // Out (also "In" if Index is invalid)
  U32   Addr;                 // Out
  U32   Type;                 // Out
  U32   ImpFlags;             // Out
  U32   UseCnt;               // Out
  U8    Internal;             // Out
  U8    Disabled;             // Out
// AG xxxxxxxxxxx Add 64-bit addr support
//  U8    Dummy0;             // Reserved for future use
//  U8    Dummy1;             // Reserved for future use
//  U32   AddrHigh;           // Out
} JLINKARM_BP_INFO;

//
// JLINKARM_WP_INFO
//
typedef struct {
  U32   SizeOfStruct;
  U32   Handle;
  U32   Addr;
  U32   AddrMask;
  U32   Data;
  U32   DataMask;
  U32   Ctrl;
  U32   CtrlMask;
  U8    WPUnit;
  U8    Internal;
  U8    Disabled;
  U8    Dummy;
  U32   UnitMask;
} JLINKARM_WP_INFO;

//
// JLINKARM_DATA_EVENT
//
typedef struct {
  int   SizeOfStruct;
  int   Type;
  U32   Addr;
  U32   AddrMask;
  U32   Data;
  U32   DataMask;
  U8    Access;
  U8    AccessMask;
} JLINKARM_DATA_EVENT;

//
// JLINKARM_HALT_REASON
//
#define JLINKARM_HALT_REASON_DBGRQ                (0)
#define JLINKARM_HALT_REASON_CODE_BREAKPOINT      (1)
#define JLINKARM_HALT_REASON_DATA_BREAKPOINT      (2)
#define JLINKARM_HALT_REASON_VECTOR_CATCH         (3)
#define JLINKARM_HALT_REASON_UNKNOWN              (-1)

typedef struct {
  int HaltReason;
  int Index;
} JLINKARM_MOE_INFO;

typedef struct {
  U32 Addr;
  U32 Size;
} JLINK_FLASH_AREA_INFO;

typedef struct {
  U32 Addr;
  U32 Size;
} JLINK_RAM_AREA_INFO;

typedef struct {
  const char*             sBankName;
  const char*             sAlgoFile;
  U32                     AlgoType;        // Really of type MAIN_FLASH_ALGO_TYPE but to avoid mutual inclusion, we choose U32 here for now...
  U32                     BaseAddr;
  const void*             paBlockInfo;     // For some algos, e.g. for SPIFI, this is just a default block info that may vary from target to target, so we need to request the actual one from the target via the RAMCode, at runtime
  const void*             pAlgoInfo;
} JLINK_FLASH_BANK_INFO;

typedef struct {
  //
  // Needed for algos that are implemented via RAMCode + PCode or PCode only
  // For these algos, pAlgoInfo is == NULL and needs to be constructed at runtime
  //
  const void*             pRAMCodeTurbo_LE;
  const void*             pRAMCodeTurbo_BE;
  const void*             pRAMCode_LE;
  const void*             pRAMCode_BE;
  U32                     SizeRAMCodeTurbo_LE;
  U32                     SizeRAMCodeTurbo_BE;
  U32                     SizeRAMCode_LE;
  U32                     SizeRAMCode_BE;
  const void*             pPCode;          // PCode for flash bank
  U32                     SizePCode;
} JLINK_FLASH_BANK_INFO_EXT;

typedef struct {
  const U8*               pPCode;          // Pointer to PCode.
  U32                     NumBytes;        // Length of PCode in bytes.
} JLINK_PCODE_INFO;

//
// JLINKARM_DEVICE_INFO
//
#define JLINKARM_DEVICE_MAX_NUM_FLASH_BANKS   16

typedef struct {
  U32                     SizeOfStruct;           // Required. Use SizeofStruct = sizeof(JLINKARM_DEVICE_INFO)
  const char*             sName;
  U32                     CoreId;
  U32                     FlashAddr;              // Start address of first flash area
  U32                     RAMAddr;                // Start address of first RAM area
  char                    EndianMode;             // 0=Little, 1=Big, 2=Both
  U32                     FlashSize;              // Total flash size in bytes (flash may contain gaps. For exact address & size of each region, please refer to aFlashArea)
  U32                     RAMSize;                // Total RAM size in bytes  (RAM may contain gaps. For exact address & size of each region, please refer to aRAMArea)
  const char*             sManu;                  // Device manufacturer
  JLINK_FLASH_AREA_INFO   aFlashArea[32];         // Region size of 0 bytes marks the end of the list
  JLINK_RAM_AREA_INFO     aRAMArea[32];           // Region size of 0 bytes marks the end of the list
  U32                     Core;                   // JLINKARM_CORE_... value
  JLINK_FLASH_BANK_INFO   aFlashBank[JLINKARM_DEVICE_MAX_NUM_FLASH_BANKS];         // Only interesting for J-Flash. Other applications can safely ignore this
  JLINK_PCODE_INFO        aPCodes[10];            // Only interesting for J-Flash. Other applications can safely ignore this. Currently, we support 5 different PCodes. We have allocated 5 elements as buffer for future versions.
  //
  // Available since extension for Flash banks without direct algo info linking
  //
  JLINK_FLASH_BANK_INFO_EXT aFlashBankExt[JLINKARM_DEVICE_MAX_NUM_FLASH_BANKS];    // Only interesting for J-Flash. Other applications can safely ignore this
  const char*             sNote;                  // This note will be shown on our website --> supported device list
} JLINKARM_DEVICE_INFO;

//
// JLINKARM_DEVICE_SELECT_INFO
//
typedef struct {
  U32 SizeOfStruct;           // Required. Use SizeofStruct = sizeof(JLINKARM_DEVICE_SELECT_INFO)
  U32 CoreIndex;
} JLINKARM_DEVICE_SELECT_INFO;

//
// JLINKARM_JTAG_DEVICE_CONF
//
typedef struct {
  U32 SizeOfStruct;           // Required. Use SizeofStruct = sizeof(JTAG_DEVICE_CONF)
  U32 IRLen;                  // Required
  U32 IRPrint;                // Optional. 0 means invalid.
  U32 Id;                     // Optional. 0 means invalid.
  const char* sName;          // Optional. NULL means invalid.
} JLINKARM_JTAG_DEVICE_CONF;

//
// JLINKARM_JTAG_DEVICE_INFO
//
typedef struct {
  const char* sName;
  U32         IRLen;
  U32         IRPrint;
} JLINKARM_JTAG_DEVICE_INFO;

//
// JLINKARM_RTCK_REACT_INFO
//
typedef struct {
  U32 SizeOfStruct;
  U32 Min;
  U32 Max;
  U32 Average;
} JLINKARM_RTCK_REACT_INFO;

//
// JLINKARM_SPEED_INFO
//
typedef struct {
  U32 SizeOfStruct;
  U32 BaseFreq;               // Clock frequency in Hz
  U16 MinDiv;                 // Minimum divider.  Max JTAG speed is BaseFreq / MinDiv.
  U16 SupportAdaptive;
} JLINKARM_SPEED_INFO;

//
// JLINKARM_EMU_INFO
//
typedef struct {
  U32 SizeOfStruct;
  U32 USBAddr;
  U32 SerialNo;
} JLINKARM_EMU_INFO;

//
// JLINKARM_EMU_CONNECT_INFO
//
typedef struct {
  U32       SerialNumber;             // This is the serial number reported in the discovery process, which is the "true serial number" for newer J-Links and 123456 for older J-Links.
  unsigned  Connection;               // Either JLINKARM_HOSTIF_USB = 1 or JLINKARM_HOSTIF_IP = 2
  U32       USBAddr;                  // USB Addr. Default is 0, values of 0..3 are permitted (Only filled if for J-Links connected via USB)
  U8        aIPAddr[16];              // IP Addr. in case emulator is connected via IP. For IP4 (current version), only the first 4 bytes are used.
  int       Time;                     // J-Link via IP only: Time period [ms] after which we have received the UDP discover answer from emulator (-1 if emulator is connected over USB)
  U64       Time_us;                  // J-Link via IP only: Time period [us] after which we have received the UDP discover answer from emulator (-1 if emulator is connected over USB)
  U32       HWVersion;                // J-Link via IP only: Hardware version of J-Link
  U8        abMACAddr[6];             // J-Link via IP only: MAC Addr
  char      acProduct[32];            // Product name
  char      acNickName[32];           // J-Link via IP only: Nickname of J-Link
  char      acFWString[112];          // J-Link via IP only: Firmware string of J-Link
  char      IsDHCPAssignedIP;         // J-Link via IP only: Is J-Link configured for IP address reception via DHCP?
  char      IsDHCPAssignedIPIsValid;  // J-Link via IP only
  char      NumIPConnections;         // J-Link via IP only: Number of IP connections which are currently established to this J-Link
  char      NumIPConnectionsIsValid;  // J-Link via IP only
  U8        aPadding[34];             // Pad struct size to 264 bytes
} JLINKARM_EMU_CONNECT_INFO;          // In general, unused fields are zeroed.
//
// JLINK_EMU_CURR_CONNECT_INFO
//
typedef struct {
  U32 HostIF;                         // See JLINKARM_HOSTIF_ defines in JLINKARM_Const.h Host interface that is currently used to communicate with J-Link. Extremely useful if HostIF is changed from USB to IP "behind the back" of the debugger in case the "nothing found on USB, earch via IP?" dialog was used
  U16 PortWEBS;                       // Port DLL control panel webserver is listening on. Set to 0 for (JLINKARM_GetDLLVersion() < 62006) && (JLINKARM_GetDLLVersion() < 62102). Assume 19080 for these versions...
  U8 acPadding[256 - (4 + 2)];
} JLINK_EMU_CURR_CONNECT_INFO;

//
// JLINKARM_HW_STATUS
//
typedef struct {
  U16 VTarget;                // Target supply voltage
  U8  tck;                    // Measured state of TCK pin:  0 or 1 or 255 (unsupported/unknown)
  U8  tdi;                    // Measured state of TDI pin:  0 or 1 or 255 (unsupported/unknown)
  U8  tdo;                    // Measured state of TDO pin:  0 or 1 or 255 (unsupported/unknown)
  U8  tms;                    // Measured state of TMS pin:  0 or 1 or 255 (unsupported/unknown)
  U8  tres;                   // Measured state of TRES pin: 0 or 1 or 255 (unsupported/unknown)
  U8  trst;                   // Measured state of TRST pin: 0 or 1 or 255 (unsupported/unknown)
} JLINKARM_HW_STATUS;

//
// JLINKARM_TRACE_DATA
//
typedef struct {
  U8 PipeStat;
  U8 Sync;
  U16 Packet;
} JLINKARM_TRACE_DATA;

//
// JLINKARM_TRACE_REGION_PROPS
//
typedef struct {
  U32 SizeofStruct;      // Size of this structure (allows extension in future versions)
  U32 RegionIndex;       // 0- based Index of region, where index 0 means oldest region.
  U32 NumSamples;        // Number of samples in region
  U32 Off;               // Offset in trace buffer
} JLINKARM_TRACE_REGION_PROPS;

//
// JLINKARM_TRACE_REGION_PROPS
//
typedef struct {
  U32 SizeofStruct;      // Size of this structure (allows extension in future versions)
  U32 RegionIndex;       // 0- based Index of region, where index 0 means oldest region.
  U32 NumSamples;        // Number of samples in region
  U32 Off;               // Offset in trace buffer
  U32 RegionCnt;
  U32 Dummy;
  U64 TimeStamp;
} JLINKARM_TRACE_REGION_PROPS_EX;

//
// JLINKARM_SWO_START_INFO
//
typedef struct {
  U32 SizeofStruct;         // Size of this structure (allows extension in future versions)
  U32 Interface;            // 0: UART, 1: Manchester, 2 (reserved for internal use)
  U32 Speed;                // 0: Auto, else [Hz]
} JLINKARM_SWO_START_INFO;

//
// JLINKARM_SWO_SPEED_INFO
//
typedef struct {
  U32 SizeofStruct;         // Size of this structure (allows extension in future versions)
  U32 Interface;            // 0: UART, 1: Manchester
  U32 BaseFreq;
  U32 MinDiv;
  U32 MaxDiv;
  U32 MinPrescale;          // 0: 1, 1: 2, 2: 4, 3: 8, ...
  U32 MaxPrescale;          // 0: 1, 1: 2, 2: 4, 3: 8, ...
} JLINKARM_SWO_SPEED_INFO;

//
// JLINK_SPI_COMMAND_DESC
//
typedef struct {
  U32       Command;
  const U8* pDataDown;
  U8*       pDataUp;
  U32       NumBits;
  U32       Flags;
  U8*       pCmdArgs;
  U32       NumBytesArgs;
  int       Result;
} JLINK_SPI_COMMAND_DESC;

//
// JLINK_WRITE_MEM_DESC
//
typedef struct {
  U32 Addr;
  U32 NumBytes;
  U8* pData;
  U8* pStatus;
  U32 AccessWidth;
  U32 Flags;
  int Result;
} JLINK_WRITE_MEM_DESC;

//
// JLINK_CORESIGHT_APDP_ACC_DESC
//
typedef struct {
  U32 Data;                     // Input for DAP-write, output for DAP-read accesses
  U32 Mask;                     // Available for DAP-read accesses: If != 0, the read request will result in a "ReadUntilValueMatch" request. Specifies a mask that it applied to the value read before it is compared against <CompVal>
  U32 CompVal;                  // Available for DAP-read accesses: If <Mask> != 0: Value to compare <ReadData> & <Mask> against. Otherwise, no effect
  int Status;                   // Indicates if the access went fine (>= 0: O.K., < 0: Error)
  int TimeoutMsReadUntil;       // Available for DAP-read accesses: If <Mask> != 0: Timeout [ms] for "ReadUntilValueMatch" request
  U8  RegIndex;                 // Index of DP/AP register to access. See JLINK_CORESIGHT_DP_ in JLINKARM_Const.h
  U8  APnDP;                    // 1 == AP access, 0 == DP access
  U8  RnW;                      // 0 == Write, 1 == read
} JLINK_CORESIGHT_APDP_ACC_DESC;

//
// JLINK_POWERTRACE_DATA_ITEM
//
typedef struct {
  U32 RefValue;
  U32 Data;
} JLINK_POWERTRACE_DATA_ITEM;

//
// JLINK_POWERTRACE_SETUP
//
typedef struct {
  int SizeOfStruct;
  U32 ChannelMask;      // 0: Int0, 1: Int1, ... 8: Ext0, 9: Ext1
  U32 SampleFreq;
  int RefSelect;        // 0: No reference, 1: SWOByteCnt
  int EnableCond;       // 0: Always, 1: RUN_ONLY
  U32 Flags;            // [0]: SBZ, [1]: 32-bit uA Mode
} JLINK_POWERTRACE_SETUP;

//
// JLINK_POWERTRACE_CAPS
//
typedef struct {
  int SizeOfStruct;
  U32 ChannelMask;      // 0: Int0, 1: Int1, ... 8: Ext0, 9: Ext1
  U32 Flags;            // [0]: More than 32 channels, [1]: 32-bit uA mode available, [31]: RAZ.
} JLINK_POWERTRACE_CAPS;

//
// JLINK_POWERTRACE_CHANNEL_CAPS_OUT
//
typedef struct {
  int SizeOfStruct;
  U32 BaseFreq;
  U32 MinDiv;
} JLINK_POWERTRACE_CHANNEL_CAPS_OUT;

//
// JLINK_POWERTRACE_CHANNEL_CAPS_IN
//
typedef struct {
  int SizeOfStruct;
  U32 ChannelMask;      // 0: Int0, 1: Int1, ... 8: Ext0, 9: Ext1
} JLINK_POWERTRACE_CHANNEL_CAPS_IN;

#define JLINK_POWERTRACE_FLAG_UA  (1 << 1)

//
// JLINK_RTTERMINAL_START
//
typedef struct {
  U32 ConfigBlockAddress;
  U32 Dummy0;         // Future use. SBZ
  U32 Dummy1;         // Future use. SBZ
  U32 Dummy2;         // Future use. SBZ
} JLINK_RTTERMINAL_START;

//
// JLINK_RTTERMINAL_STOP
//
typedef struct {
  U8  InvalidateTargetCB;
  U8  acDummy[3];     // Future use. SBZ
  U32 Dummy0;         // Future use. SBZ
  U32 Dummy1;         // Future use. SBZ
  U32 Dummy2;         // Future use. SBZ
} JLINK_RTTERMINAL_STOP;

typedef struct {
  int   BufferIndex;
  U32   Direction;
  char  acName[32];
  U32   SizeOfBuffer;
  U32   Flags;
} JLINK_RTTERMINAL_BUFDESC;

typedef struct {
  U32         NumBytesTransferred;  // Total number of bytes sent to client application(s)
  U32         NumBytesRead;         // Total number of bytes read from the target
  int         HostOverflowCount;    // Indicates a buffer overflow on the host system
  int         IsRunning;
  int         NumUpBuffers;         // Number of channels from the target which are handled by DLL.
  int         NumDownBuffers;       // Number of channels to the target which are handled by DLL.
  U32         OverflowMask;         // Earlier: SBZ, Since V6.93a: Indicates which buffer(s) overflowed. 1 << 0 == Buffer 0, 1 << 1 == Buffer 1, ... Or-combination is possible, indicating that multiple buffers overflowed.
  U32         Dummy1;               // Future use. SBZ
} JLINK_RTTERMINAL_STATUS;

//
// JLINK_HSS_MEM_BLOCK_DESC
//
typedef struct {
  U32 Addr;
  U32 NumBytes;
  U32 Flags;          // Future use. SBZ.
  U32 Dummy;          // Future use. SBZ.
} JLINK_HSS_MEM_BLOCK_DESC;

//
// JLINK_HSS_CAPS
//
typedef struct {
  U32 MaxBlocks;
  U32 MaxFreq;
  U32 Caps;
  U32 aDummy[5];      // Future use. SBZ.
} JLINK_HSS_CAPS;

//
// JLINK_EMU_GPIO_DESC
//
typedef struct {
  char  acName[32];
  U32   Caps;
} JLINK_EMU_GPIO_DESC;

//
// JLINKARM_EMU_PCODE_STATUS_INFO
//
typedef struct {
  U32 PCodeStat;                   // 0: Code has been executed properly
  U32 S32_PC;                      // PC of target system. Typically PC starts at 4 and last PC is the offset of a BRK instruction.
  U32 S32_R0;                      // R0 of target system. Typically used as return value.
  U32 S32_SP;                      // Stack pointer of target system. Not normally of interest.
  U32 S32_StatusReg;               // Status register of target system. Not normally of interest.
} JLINKARM_EMU_PCODE_STATUS_INFO;

//
// JLINKARM_FLASH_EVENT
//
typedef enum {
  JLINKARM_FLASH_EVENT_ERASE,
  JLINKARM_FLASH_EVENT_PROGRAM,
  JLINKARM_FLASH_EVENT_VERIFY
} JLINKARM_FLASH_EVENT;

//
// JLINKARM_FLASH_NOTIFY_INFO
//
typedef struct {
  U32 SizeofStruct;
  U32 Addr;
  U32 NumBytesTotal;
  U32 NumBytesCurrent;
} JLINKARM_FLASH_NOTIFY_INFO;

#define JLINK_MEM_ZONE_DEFAULT_NAME "Default"
#define JLINK_MEM_ZONE_DEFAULT_DESC "Default access mode"

typedef struct {
  const char* sName;
  const char* sDesc;
  U64         VirtAddr;
  U8 abDummy[16];       // Round up to 32 bytes. Future use. Currently 0
} JLINK_MEM_ZONE_INFO;

#define JLINK_DISASSEMBLY_MODE_CURRENT  (0)
#define JLINK_DISASSEMBLY_MODE_ARM      (1)
#define JLINK_DISASSEMBLY_MODE_THUMB    (2)
//
// Always set lowest bit for branch type defines,
// to keep things compatible to old DLLs which only distinguished between TRUE and FALSE by setting bit 0
//
#define JLINK_DISASSEMBLY_BRANCH_UNSPECIFIED  (1 << 0)
#define JLINK_DISASSEMBLY_BRANCH_FIXED        ((1 << 1) | (1 << 0))
#define JLINK_DISASSEMBLY_BRANCH_RELATIVE     ((2 << 1) | (1 << 0))
#define JLINK_DISASSEMBLY_BRANCH_REGISTER     ((3 << 1) | (1 << 0))
#define JLINK_DISASSEMBLY_BRANCH_RETURN       ((4 << 1) | (1 << 0)) // Return-style branch via (dedicated) register, such as LR
#define JLINK_DISASSEMBLY_BRANCH_STACK        ((5 << 1) | (1 << 0)) // Return-style branch via stack, such as POP PC
#define JLINK_DISASSEMBLY_BRANCH_NONE         (0)

typedef struct {
  U32 SizeOfStruct; // Required. Use SizeofStruct = sizeof(JLINK_DISASSEMBLY_INFO)
  U8  Mode;         // Mode to use for disassembling instruction. 0: Current CPU Mode; 1: ARM Mode; 2: Thumb Mode
  U8  Condition;    // [0]: 1 = Use condition for this instruction. [7:1]: Condition to use (e.g. [4:1] in IT Block on Cortex-M).
  U8  aDummy[2];    // Reserved for future use. S.B.Z.
} JLINK_DISASSEMBLY_INFO;

typedef struct {
  U8 Size;           // Instruction size in bytes
  U8 IsBranch;       // Old DLLs: 1 == Inst is a branch. Later DLLs: JLINK_DISASSEMBLY_BRANCH_
  U8 IsCall;         // Call, where the return addr. is stored either on stack or in a dedicated register, such as LR
  U8 IsMemAccess;    // Instruction reads or writes memory
  U8 IsFixedAddr;    // (Branch/Load/Store) address is fixed (absolute or PC-relative)
  U8 IsConditional;
  U8 ITBlock;        // != 0: Instruction is Cortex-M IT inst or similar, condition and mask encoded as in Cortex-M IT inst ([7:4] = FirstCond, [3:0] = Mask)
  U8 abAddr[8];      // Array to keep in-struct alignment. Address is stored in this element, little endian
  U8 aDummy[1];      // Reserved for future use. S.B.Z.
} JLINK_DISASSEMBLY_INST_INFO;

typedef int STDCALL JLINK_DISASSEMBLY_FUNC_GET_INST_INFO(JLINK_DISASSEMBLY_INST_INFO* pInfoOut, const U8* pData, U32 NumBytes, U8 CPUMode);
typedef int STDCALL JLINK_DISASSEMBLY_FUNC_DISASSEMBLE_BUFFER(char* pInst, U32 InstSize, const void* pBuffer, U32 BufferSize, U32 Addr, U8 CPUMode);

//
// JLINK_RAWTRACE_BIST_ERR_BUF_DESC
//
typedef struct {
  U32 FailAddr;
  U32 DataExpected;
  U32 DataRead;
  U32 DataReRead;
  U32 aReserved[4];    // Reserved for future use. Filled with 0, so when evaluating in future versions, make sure that new elements do not contain 0
} JLINK_RAWTRACE_BIST_ERR_BUF_DESC;

typedef struct {
  U32 Size;
  U8  SectorSizeShift;
  U8  MinAlignShift;
  U8  Dummy0;
  U8  Dummy1;
} JLINK_EMU_TARGET_IMG_AREA_INFO;

//
// JLINK_STRACE_TIMESTAMP_INFO
//
typedef struct {
  U64 Time;
  U32 Index;
  U32 Flags;
} JLINK_STRACE_TIMESTAMP_INFO;

typedef struct {
  U32 SizeofStruct;                   // Filled in by caller
  U32 VolumeSizeLo;
  U32 VolumeSizeHi;
  U32 NumBytesUsedLo;
  U32 NumBytesUsedHi;
} JLINK_EMU_FILE_VOLUME_INFO;

//
// Types, flash programming API
//
typedef unsigned BANK_HANDLE;
typedef unsigned DATA_FILE_HANDLE;
typedef void (JLINK_FLASH_PROGRESS_CB_FUNC)(const char * sAction, const char * sProg, int Percentage);

//
// Types, callback functions
//
typedef void STDCALL    JLINK_LOG                       (const char* sErr);
typedef void            JLINKARM_LOG                    (const char* sErr);
typedef void            JLINKARM_FLASH_NOTIFY_FUNC      (JLINKARM_FLASH_EVENT Event, const JLINKARM_FLASH_NOTIFY_INFO * pNotifyInfo);
typedef int             JLINKARM_WAIT_FUNC              (void* pContext);                                                                   // Returns 0 if o.k., else != 0
//
//
//  Flags:
//    Or combination of JLINK_DLG_BUTTON_ values.
//
//
//  Return value
//    Or combination of JLINK_DLG_BUTTON_ values.
//    Note: Only the values that have been passed in <Flags> can be returned.
//    Sample: <Flags> == JLINK_DLG_BUTTON_YES | JLINK_DLG_BUTTON_NO
//    Possible return values:
//                            JLINK_DLG_BUTTON_YES
//                            JLINK_DLG_BUTTON_NO
//

#define JLINK_UNSECURE_DIALOG_CB_FUNC  JLINK_HOOK_DIALOG_UNSECURE  // Define for backward compatability

typedef int JLINK_HOOK_DIALOG_UNSECURE       (const char* sTitle, const char* sMsg, U32 Flags);
typedef int JLINK_HOOK_DIALOG_UNLOCK_IDCODE  (const char* sTitle, const char* sMsg, U32 Flags, void* pIDCODE, int MaxNumBytesIDCODE);
//
// FLASHDL related
// Needed for
// J-Flash SPI
// Flasher stand-alone CFG generation
// J-Link / Flasher FlashDL
// Keep binary compatibility! Avoid changes in any names here as well
//
typedef enum {
   JLINK_FLASHDL_FLASH_TYPE_SPI_NOR  = 1  // Traditional SPI flash
  ,JLINK_FLASHDL_FLASH_TYPE_SPI_FRAM = 2  // FRAM
} JLINK_FLASHDL_FLASH_TYPE;

typedef enum {
   JLINK_FLASHDL_SPI_TRANSFER_MODE_SINGLE = 1  // Traditional single pin data transfer
  ,JLINK_FLASHDL_SPI_TRANSFER_MODE_QUAD   = 2  // Quad pin data transfer
} JLINK_FLASHDL_SPI_TRANSFER_MODE;

typedef enum {
   JLINK_FLASHDL_SPI_STAT_REG_FORMAT_MICRON      = (0)   // StatusFlagReg (used for wait during prog/erase): [1:1] == Err, [4:4] == Err, [5:5] == Err, [7:7] == Ready
                                                         // StatusReg (used for block protection): [4:2] = BP0-2, [6:6] == BP3
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_MICRON_2    = (1)   // [0:0] == Busy, [4:2] == BP0-2, [6:6] == BP3
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_SPANSION    = (2)   // [0:0] == Busy, [4:2] == BP0-2, [6:5] == ErrFlags
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_WINBOND_3BP = (3)   // Byte0: [0:0] == Busy, [4:2] == BP0-2; Byte1: [0:0] SRP1, [1:1] QE, [5:3] LB1-LB3, [6:6] CMP
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_WINBOND_4BP = (4)   // [0:0] == Busy, [5:2] == BP0-3, [6:6] == QE
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_ADESTO_1BP  = (5)   // [0:0] == Busy
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_GIGADEV_5BP = (6)   // [0:0] == Busy, [6:2] == BP0-4
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_MICRON_3BP  = (7)   // [0:0] == Busy, [4:2] == BP0-2
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_ADESTO_2BP  = (8)   // [0:0] == Busy, [3:2] == BP0-1
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_MICROCHIP   = (9)   // [0:0] == Busy, [4:4] == BP0
  ,JLINK_FLASHDL_SPI_STAT_REG_FORMAT_ADESTO_PE   = (10)  // [0:0] == Page_Size, [1:1] == Sector Protection, [7:7] == Busy
} JLINK_FLASHDL_SPI_STAT_REG_FORMAT;

typedef struct {
  U32  FlashType;            // See JLINK_FLASHDL_FLASH_TYPE enum
  U32  StatRegFormat;        // See JLINK_FLASHDL_SPI_STAT_REG_FORMAT enum
  U32  NumPages;
  U32  TimeoutMsEraseSect;
  U32  TimeoutMsEraseChip;
  U32  TimeoutMsProgPage;
  U32  TimeoutMsVerify;
  U32  Flags;                // [0]: <StatusIsReadyBit>, [1]: <HasDedicated4BAddrMode>, [2] <UseQuad>, [3] <SkipBlankDataOnProg>
  U8   CmdWriteEnable;
  U8   CmdWriteDisable;
  U8   CmdReadID;
  U8   CmdReadStatus;
  U8   CmdWriteStatus;
  U8   CmdReadData;
  U8   CmdWritePage;
  U8   CmdEraseSector;
  U8   CmdEraseBulk;
  U8   CmdEnter4ByteMode;    // Only valid if <HasDedicated4BAddrMode> == 1
  U8   CmdExit4ByteMode;     // Only valid if <HasDedicated4BAddrMode> == 1
  U8   NumAddrBytes;
  U8   StatusBitPos;
  U8   CmdReadQuad;          // Only valid if <UseQuad> == 1
  U8   CmdWritePageQuad;     // Only valid if <UseQuad> == 1
  U8   DummyCycReadQuad;     // Only valid if <UseQuad> == 1
  U8   PageSizeShift;
  U8   SectSizeShift;
  U8   TransferModeRdCmd;    // See JLINK_FLASHDL_SPI_TRANSFER_MODE enum
  U8   TransferModeWrCmd;    // See JLINK_FLASHDL_SPI_TRANSFER_MODE enum
  U8   TransferModeWrAddr;   // See JLINK_FLASHDL_SPI_TRANSFER_MODE enum
  U8   TransferModeRdAddr;   // See JLINK_FLASHDL_SPI_TRANSFER_MODE enum
  U8   EraseType;            // ERASE_TYPE_SECTORS == 1, ERASE_TYPE_SECTORS_IF_REQUIRED == 2.
} JLINK_FLASHDL_SPI_SETUP;

//
// Typedefs for JLINK_GetpFunc()
//
typedef enum {
  JLINK_IFUNC_SET_HOOK_DIALOG_UNLOCK_IDCODE=0,
  JLINK_IFUNC_SPI_TRANSFER_MULTIPLE,
  JLINK_IFUNC_PIN_OVERRIDE,                             // Internal use. Not explained in SDK manual
  JLINK_IFUNC_PIN_OVERRIDE_GET_PIN_CAPS,                // Internal use. Not explained in SDK manual
  JLINK_IFUNC_MRU_GETLIST,
  //
  // There was one version which by accident added some defines
  // for new functions here even though the functions have never been implemented.
  // To stay binary compatible, define them as dummies
  //
  JLINK_IFUNC_RESERVED3,
  JLINK_IFUNC_RESERVED4,
  JLINK_IFUNC_RESERVED5,
  JLINK_IFUNC_GET_SESSION_ID,
  JLINK_IFUNC_CORESIGHT_TRIGGER_READ_APDP_REG,
  JLINK_IFUNC_CAN_ACC_MEM_WHILE_RUNNING,
  JLINK_IFUNC_UPDATE_BTL,
  JLINK_IFUNC_GET_CURRENT_ENDIANESS,
  JLINK_IFUNC_ALGODB_GET_PALGO_INFO,      // Only needed for internal use (J-Flash).
  JLINK_IFUNC_ALGODB_GET_PALGO_INFO_CFI,  // Only needed for internal use (J-Flash).
  JLINK_IFUNC_ALGODB_GET_ALGO_NO,         // Only needed for internal use (J-Flash).
  JLINK_IFUNC_PCODE_SET_ENTRY_FUNC,       // Only needed for internal use (PCode compiler).
  JLINK_IFUNC_PCODE_DOWNLOAD,             // Only needed for internal use (script files etc.)
  JLINK_IFUNC_PCODE_EXEC_EX,              // Only needed for internal use
  JLINK_IFUNC_START_MERGE_COMMANDS,       // Only needed for internal use
  JLINK_IFUNC_END_MERGE_COMMANDS,         // Only needed for internal use
  JLINK_IFUNC_RAWTRACE_BIST_STARTSTOP,       // Only needed for internal use (J-Link Commander)
  JLINK_IFUNC_RAWTRACE_BIST_READ_ERR_STATS,  // Only needed for internal use (J-Link Commander)
  JLINK_IFUNC_GET_PF_GET_INST_INFO,
  JLINK_IFUNC_CORESIGHT_ACC_APDP_REG_MUL,
  JLINK_IFUNC_PCODE_DATA_DOWNLOAD,
  JLINK_IFUNC_PCODE_EXEC_EX2,
  JLINK_IFUNC_PCODE_FREE,                 // Only needed for internal use (script files etc.)
  JLINK_IFUNC_EMU_COMMANDLINE_WRITE_READ, // Emulates Flasher RS232 ASCII interface via USB commands
  JLINK_IFUNC_GET_PF_DISASSEMBLE_BUFFER,
  JLINK_IFUNC_EMU_GET_TARGET_IMG_AREA_INFO,  // Only needed internally for now. Only available on Calliope Mini
  JLINK_IFUNC_EMU_READ_TARGET_IMG_AREA,      // Only needed internally for now. Only available on Calliope Mini
  JLINK_IFUNC_EMU_WRITE_TARGET_IMG_AREA,     // Only needed internally for now. Only available on Calliope Mini
  JLINK_IFUNC_EMU_GET_CURR_CONN_INFO,        // Only needed internally for now. (J-Link Remote Server)
  JLINK_IFUNC_GET_PF_EXP_DEVICE_LIST_XML,
  JLINK_IFUNC_SCRIPTFILE_EXEC_FUNC,
  JLINK_IFUNC_EMU_ADD_FW_IMAGES,
  JLINK_IFUNC_READ_CSR,
  JLINK_IFUNC_WRITE_CSR,
  JLINK_IFUNC_EMU_FILE_GET_VOLUME_INFO,
  JLINK_IFUNC_ERASE,
  JLINK_IFUNC_EMU_WRITE_TRACE_BUF_RAM,       // Reserved for internal use
  JLINK_IFUNC_EMU_READ_TRACE_BUF_RAM,        // Reserved for internal use
  JLINK_IFUNC_FLASHDL_GET_STATUS,            // Reserved for internal use
  JLINK_IFUNC_FLASHDL_FREE_RESOURCES,        // Reserved for internal use
  JLINK_IFUNC_FLASHDL_SPI_SETUP,             // Reserved for internal use
  JLINK_IFUNC_FLASHDL_SPI_SCHED_PROG_JOB,    // Reserved for internal use
  JLINK_IFUNC_FLASHDL_SPI_SCHED_VERIFY_JOB,  // Reserved for internal use
  JLINK_IFUNC_FLASHDL_SPI_SCHED_ERASE_JOB,   // Reserved for internal use
  JLINK_NUM_FUNC_INDEXES
} JLINK_FUNC_INDEX;

typedef int STDCALL JLINK_FUNC_SET_HOOK_DIALOG_UNLOCK_IDCODE(JLINK_HOOK_DIALOG_UNLOCK_IDCODE* pfHook);        // JLINK_IFUNC_SET_HOOK_DIALOG_UNLOCK_IDCODE
typedef int STDCALL JLINK_FUNC_SPI_TRANSFER_MULTIPLE(JLINK_SPI_COMMAND_DESC* paDesc, U32 NumCommands);        // JLINK_IFUNC_SPI_TRANSFER_MULTIPLE
typedef int STDCALL JLINK_FUNC_MRU_GETLIST(U32 Group, char* sList, U32 BufferSize);                           // JLINK_IFUNC_MRU_GETLIST
typedef int STDCALL JLINK_FUNC_PIN_OVERRIDE(const U32* paMode, U32* paState);
typedef int STDCALL JLINK_FUNC_PIN_OVERRIDE_GET_PIN_CAPS(U32* paCaps);
typedef int STDCALL JLINK_FUNC_GET_SESSION_ID(void);
typedef int STDCALL JLINK_FUNC_CORESIGHT_TRIGGER_READ_APDP_REG(U8 RegIndex, U8 APnDP, U32* pData);
typedef int STDCALL JLINK_FUNC_CAN_ACC_MEM_WHILE_RUNNING(void);
typedef int STDCALL JLINK_FUNC_UPDATE_BTL(void);
typedef int STDCALL JLINK_FUNC_GET_CURRENT_ENDIANESS(void);
typedef int STDCALL JLINK_FUNC_ALGODB_GET_PALGO_INFO(unsigned AlgoNo, unsigned Variant, const void** ppAlgoInfo);
typedef int STDCALL JLINK_FUNC_ALGODB_GET_PALGO_INFO_CFI(unsigned AlgoNo, unsigned Variant, unsigned NumChips, unsigned NumBits, unsigned CoreType, const void** ppAlgoInfo);
typedef int STDCALL JLINK_FUNC_ALGODB_GET_ALGO_NO(const void* pAlgoInfo, unsigned* pAlgoNo, unsigned* pVariant);
typedef int STDCALL JLINK_FUNC_PCODE_SET_ENTRY(const char* sEntryFunc, U32* pPC, U32* pSP);
typedef int STDCALL JLINK_FUNC_PCODE_DOWNLOAD(U8* pPCode, U32 NumBytes);
typedef int STDCALL JLINK_FUNC_PCODE_DATA_DOWNLOAD(U8* pData, U32 NumBytes);
typedef int STDCALL JLINK_FUNC_PCODE_EXEC_EX(const char* sEntryFunc, JLINKARM_EMU_PCODE_STATUS_INFO* pPCodeStat);
typedef int STDCALL JLINK_FUNC_START_MERGE_COMMANDS(void);
typedef int STDCALL JLINK_FUNC_END_MERGE_COMMANDS(void);
typedef int STDCALL JLINK_FUNC_RAWTRACE_BIST_STARTSTOP(int StartBIST);
typedef int STDCALL JLINK_FUNC_RAWTRACE_BIST_READ_ERR_STATS(JLINK_RAWTRACE_BIST_ERR_BUF_DESC* paBuffer, int MaxNumItems);
typedef int STDCALL JLINK_FUNC_GET_PF_GET_INST_INFO(U32 Core, JLINK_DISASSEMBLY_FUNC_GET_INST_INFO** ppfFunc);
typedef int STDCALL JLINK_FUNC_ACC_APDP_REG_MUL(JLINK_CORESIGHT_APDP_ACC_DESC* paDesc, int NumEntries);
typedef int STDCALL JLINK_FUNC_PCODE_EXEC_EX2(const char* sEntryFunc, JLINKARM_EMU_PCODE_STATUS_INFO* pPCodeStat, U32 *pParams, U32 ParamCnt);
typedef int STDCALL JLINK_FUNC_PCODE_FREE(void);
typedef int STDCALL JLINK_FUNC_EMU_COMMANDLINE_WRITE_READ(const void* pDataWrite, U32 NumBytesToWrite, void* pDataRead, U32 MaxNumBytesToRead);
typedef int STDCALL JLINK_FUNC_GET_PF_DISASSEMBLE_BUFFER(U32 Core, JLINK_DISASSEMBLY_FUNC_DISASSEMBLE_BUFFER** ppfFunc);
typedef int STDCALL JLINK_FUNC_EMU_GET_TARGET_IMG_AREA_INFO(JLINK_EMU_TARGET_IMG_AREA_INFO* pInfo);
typedef int STDCALL JLINK_FUNC_EMU_READ_TARGET_IMG_AREA(U32 Off, U32 NumBytes, void* pData);
typedef int STDCALL JLINK_FUNC_EMU_WRITE_TARGET_IMG_AREA(U32 Off, U32 NumBytes, const void* pData);
typedef int STDCALL JLINK_FUNC_EMU_GET_CURR_CONN_INFO(JLINK_EMU_CURR_CONNECT_INFO* pInfo);
typedef int STDCALL JLINK_FUNC_GET_PF_EXP_DEVICE_LIST_XML(const char* sPath);
typedef int STDCALL JLINK_FUNC_SCRIPTFILE_EXEC_FUNC(const char* sFunc, I64* pRetVal, U32* paParams, U32 NumParams);
typedef int STDCALL JLINK_FUNC_ADD_FW_IMAGES(const void* paFWImg, U32 NumImages);
typedef int STDCALL JLINK_FUNC_READ_CSR(U32 RegIndex, U64* pData, U32 RegSize);
typedef int STDCALL JLINK_FUNC_WRITE_CSR(U32 RegIndex, U64 Data, U32 RegSize);
typedef int STDCALL JLINK_FUNC_EMU_FILE_GET_VOLUME_INFO(JLINK_EMU_FILE_VOLUME_INFO* pInfo);
typedef int STDCALL JLINK_FUNC_ERASE(U64 SAddr, U64 EAddr);
typedef int STDCALL JLINK_FUNC_EMU_WRITE_TRACE_BUF_RAM(U32 Off, U32 NumBytes, const void* pData);
typedef int STDCALL JLINK_FUNC_EMU_READ_TRACE_BUF_RAM(U32 Off, U32 NumBytes, void* pData);
typedef int STDCALL JLINK_FUNC_FLASHDL_GET_STATUS(U64* pFailAddr);
typedef int STDCALL JLINK_FUNC_FLASHDL_FREE_RESOURCES(void);
typedef int STDCALL JLINK_FUNC_FLASHDL_SPI_SETUP(const JLINK_FLASHDL_SPI_SETUP* pSetupData);
typedef int STDCALL JLINK_FUNC_FLASHDL_SPI_SCHED_PROG_JOB(U64 Addr, U32 NumBytes, const void* pData);
typedef int STDCALL JLINK_FUNC_FLASHDL_SPI_SCHED_VERIFY_JOB(U64 Addr, U32 NumBytes, const void* pData);
typedef int STDCALL JLINK_FUNC_FLASHDL_SPI_SCHED_ERASE_JOB(U64 Addr, U32 NumBytes);

/*********************************************************************
*
*       DP/AP register indexes
*/
//
// Note: The indexes here will be used as they are passed, meaning if DP/APRegX has different meanings depending on the selected DP/AP bank,
//       the currently selected DP/AP bank will be accessed. The J-Link software will *not* switch the DP/AP bank!
//       It is user's responsibility to switch to the appropriate bank, before using indexes related to that bank
//       Switching the bank is done via JLINK_CORESIGHT_DP_REG_SELECT that allows specifying the DP and AP bank. This register is always available, no matter what bank is selected
//
#define JLINK_CORESIGHT_DP_REG_0x0 0  // Access DPReg0 (called addr. 0x0 in the ARM docs)
#define JLINK_CORESIGHT_DP_REG_0x4 1  // Access DPReg1 (called addr. 0x4 in the ARM docs)
#define JLINK_CORESIGHT_DP_REG_0x8 2  // Access DPReg2 (called addr. 0x8 in the ARM docs)
#define JLINK_CORESIGHT_DP_REG_0xC 3  // Access DPReg3 (called addr. 0xC in the ARM docs)
#define JLINK_CORESIGHT_AP_REG_0x0 0  // Access DPReg0 (called addr. 0x0 in the ARM docs)
#define JLINK_CORESIGHT_AP_REG_0x4 1  // Access DPReg1 (called addr. 0x4 in the ARM docs)
#define JLINK_CORESIGHT_AP_REG_0x8 2  // Access DPReg2 (called addr. 0x8 in the ARM docs)
#define JLINK_CORESIGHT_AP_REG_0xC 3  // Access DPReg3 (called addr. 0xC in the ARM docs)
#define JLINK_CORESIGHT_DP_REG_0x0_NEW 0x000000F0  // Access DPReg0. Needed for JTAG since older DPs did not specify DPReg0 for JTAG-DPs so we returned IDCODE for DPReg0 accesses via JTAG. Use this value to bypass IDCODE logic for CORESIGHT functions
//
// Common MEM-AP bank 0 registers
//
#define JLINK_CORESIGHT_AP_REG_CSW 0  // Control and status
#define JLINK_CORESIGHT_AP_REG_TAR 1  // Target address
#define JLINK_CORESIGHT_AP_REG_DRW 3  // data read/write
//
// AP bank 1 registers (AP bank has to be selected manually by user via write to JLINK_CORESIGHT_DP_REG_SELECT)
//
#define JLINK_CORESIGHT_AP_REG_BD0 0
#define JLINK_CORESIGHT_AP_REG_BD1 1
#define JLINK_CORESIGHT_AP_REG_BD2 2
#define JLINK_CORESIGHT_AP_REG_BD3 3
//
// DP bank 0 registers (JLINK_CORESIGHT_DP_REG_SELECT[3:0] == 0)
//
#define JLINK_CORESIGHT_DP_REG_DPIDR       0      // On read.  SW-DP           v1 and higher => When used on JTAG-DP, IDCODE scan chain will automatically be accessed
#define JLINK_CORESIGHT_DP_REG_ABORT       0      // On write. SW-DP           v1 and higher => When used on JTAG-DP, ABORT scan chain will automatically be accessed
#define JLINK_CORESIGHT_DP_REG_CTRL_STAT   1      // On R/W.   JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_RESEND      2      // On read.  SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_SELECT      2      // On write. JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_RD_BUF      3      // On read.  JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_TARGETSEL   3      // On write. SW-DP           v2 and higher
//
// DP bank 1 registers (JLINK_CORESIGHT_DP_REG_SELECT[3:0] == 1)
//
#define JLINK_CORESIGHT_DP_REG_DPIDR       0      // On read.  SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_ABORT       0      // On write. SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_DLCR        1      //           SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_RESEND      2      // On read.  SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_SELECT      2      // On write. JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_RD_BUF      3      // On read.  JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_TARGETSEL   3      // On write. SW-DP           v2 and higher
//
// DP bank 2 registers (JLINK_CORESIGHT_DP_REG_SELECT[3:0] == 2)
//
#define JLINK_CORESIGHT_DP_REG_DPIDR       0      // On read.  SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_TARGETID    1      // On read.  SW-DP           v2 and higher => Undefined register on write
#define JLINK_CORESIGHT_DP_REG_RESEND      2      // On read.  SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_SELECT      2      // On write. JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_RD_BUF      3      // On read.  JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_TARGETSEL   3      // On write. SW-DP           v2 and higher
//
// DP bank 3 registers (JLINK_CORESIGHT_DP_REG_SELECT[3:0] == 3)
//
#define JLINK_CORESIGHT_DP_REG_DPIDR       0      // On read.  SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_DLPIDR      1      // On read.  SW-DP           v2 and higher => Undefined register on write
#define JLINK_CORESIGHT_DP_REG_RESEND      2      // On read.  SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_SELECT      2      // On write. JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_RD_BUF      3      // On read.  JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_TARGETSEL   3      // On write. SW-DP           v2 and higher
//
// DP bank 4 registers (JLINK_CORESIGHT_DP_REG_SELECT[3:0] == 4)
//
#define JLINK_CORESIGHT_DP_REG_DPIDR       0      // On read.  SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_EVENTSTAT   1      // On read.  SW-DP           v2 and higher => Undefined register on write
#define JLINK_CORESIGHT_DP_REG_RESEND      2      // On read.  SW-DP           v1 and higher
#define JLINK_CORESIGHT_DP_REG_SELECT      2      // On write. JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_RD_BUF      3      // On read.  JTAG-DP / SW-DP v1 and higher
#define JLINK_CORESIGHT_DP_REG_TARGETSEL   3      // On write. SW-DP           v2 and higher

/*********************************************************************
*
*       Config mask
*/
#define JLINK_CONFIG_MASK_HOST_IF         (1 << 0)
#define JLINK_CONFIG_MASK_TARGET_DEVICE   (1 << 1)
#define JLINK_CONFIG_MASK_TARGET_IF       (1 << 2)

/*********************************************************************
*
*       Trace source defines
*/
#define JLINKARM_TRACE_SOURCE_ETB  0
#define JLINKARM_TRACE_SOURCE_ETM  1
#define JLINKARM_TRACE_SOURCE_MTB  2              // Internally, same as JLINKARM_TRACE_SOURCE_ETB => TraceSource == on-chip trace buffer
#define JLINKARM_TRACE_SOURCE_SWO  3              // Same as JLINKARM_TRACE_SOURCE_ETM, but the data will be transferred over the SWO pin instead of the trace pins.

/*********************************************************************
*
*       TRACE Commands (for JLINKARM_TRACE_Control())
*/
#define JLINKARM_TRACE_CMD_START                0
#define JLINKARM_TRACE_CMD_STOP                 1
#define JLINKARM_TRACE_CMD_FLUSH                2
#define JLINKARM_TRACE_CMD_GET_NUM_SAMPLES      0x10
#define JLINKARM_TRACE_CMD_GET_CONF_CAPACITY    0x11
#define JLINKARM_TRACE_CMD_SET_CAPACITY         0x12
#define JLINKARM_TRACE_CMD_GET_MIN_CAPACITY     0x13
#define JLINKARM_TRACE_CMD_GET_MAX_CAPACITY     0x14
#define JLINKARM_TRACE_CMD_SET_FORMAT           0x20
#define JLINKARM_TRACE_CMD_GET_FORMAT           0x21
#define JLINKARM_TRACE_CMD_GET_NUM_REGIONS      0x30
#define JLINKARM_TRACE_CMD_GET_REGION_PROPS     0x31
#define JLINKARM_TRACE_CMD_GET_REGION_PROPS_EX  0x32

/*********************************************************************
*
*       TRACE Format flags
*/
#define JLINKARM_TRACE_FORMAT_4BIT              0x00000001ul  // 4 bit data
#define JLINKARM_TRACE_FORMAT_8BIT              0x00000002ul  // 8 bit data
#define JLINKARM_TRACE_FORMAT_16BIT             0x00000004ul  // 16 bit data
#define JLINKARM_TRACE_FORMAT_MULTIPLEXED       0x00000008ul  // muliplexing on ETM/buffer link
#define JLINKARM_TRACE_FORMAT_DEMULTIPLEXED     0x00000010ul  // demultiplexing on ETM/buffer link
#define JLINKARM_TRACE_FORMAT_DOUBLE_EDGE       0x00000020ul  // clock data on both edges on ETM/buffer link
#define JLINKARM_TRACE_FORMAT_ETM7_9            0x00000040ul  // Use ETM7/ETM9 protocol
#define JLINKARM_TRACE_FORMAT_ETM10             0x00000080ul  // Use ETM10 protocol
#define JLINKARM_TRACE_FORMAT_1BIT              0x00000100ul  // 1 bit data
#define JLINKARM_TRACE_FORMAT_2BIT              0x00000200ul  // 2 bit data

/*********************************************************************
*
*       RAWTRACE Commands (for JLINKARM_RAWTRACE_Control())
*/
#define JLINKARM_RAWTRACE_CMD_START                 0
#define JLINKARM_RAWTRACE_CMD_STOP                  1
#define JLINKARM_RAWTRACE_CMD_GET_TRACE_FREQ        2
#define JLINKARM_RAWTRACE_CMD_SET_BUFF_SIZE         3
#define JLINKARM_RAWTRACE_CMD_GET_CAPS              4
//
// RAWTRACE format flags
//
#define JLINKARM_RAWTRACE_FORMAT_1BIT           0x00000003ul  // 1 bit data
#define JLINKARM_RAWTRACE_FORMAT_2BIT           0x0000000Cul  // 2 bit data
#define JLINKARM_RAWTRACE_FORMAT_4BIT           0x00000030ul  // 4 bit data
//
// RAWTRACE capabilities (binary compatible to internal capability defines)
//
#define JLINK_RAWTRACE_CAP_RESERVED             (1 <<  0)
#define JLINK_RAWTRACE_CAP_START                (1 <<  1)
#define JLINK_RAWTRACE_CAP_STOP                 (1 <<  2)
#define JLINK_RAWTRACE_CAP_READ                 (1 <<  3)
#define JLINK_RAWTRACE_CAP_GET_FREQ             (1 <<  4)
#define JLINK_RAWTRACE_CAP_LTRACE               (1 <<  5)
#define JLINK_RAWTRACE_CAP_LTRACE_USB           (1 <<  6)

/*********************************************************************
*
*       RTTERMINAL Commands
*/
#define JLINKARM_RTTERMINAL_CMD_START           0
#define JLINKARM_RTTERMINAL_CMD_STOP            1
#define JLINKARM_RTTERMINAL_CMD_GETDESC         2
#define JLINKARM_RTTERMINAL_CMD_GETNUMBUF       3
#define JLINKARM_RTTERMINAL_CMD_GETSTAT         4

#define JLINKARM_RTTERMINAL_BUFFER_DIR_UP       0
#define JLINKARM_RTTERMINAL_BUFFER_DIR_DOWN     1

/*********************************************************************
*
*       POWERTRACE Commands (for JLINK_POWERTRACE_Control())
*/
#define JLINK_POWERTRACE_CMD_SETUP              0
#define JLINK_POWERTRACE_CMD_START              1
#define JLINK_POWERTRACE_CMD_STOP               2
#define JLINK_POWERTRACE_CMD_FLUSH              3
#define JLINK_POWERTRACE_CMD_GET_CAPS           4
#define JLINK_POWERTRACE_CMD_GET_CHANNEL_CAPS   5
#define JLINK_POWERTRACE_CMD_GET_NUM_ITEMS      6

/*********************************************************************
*
*       SWO Commands
*/
#define JLINKARM_SWO_CMD_START                  0         // Parameter: JLINKARM_SWO_START_INFO *
#define JLINKARM_SWO_CMD_STOP                   1
#define JLINKARM_SWO_CMD_FLUSH                  2         // Parameter: U32* Number of bytes to flush
#define JLINKARM_SWO_CMD_GET_SPEED_INFO         3
#define JLINKARM_SWO_CMD_GET_NUM_BYTES          10
#define JLINKARM_SWO_CMD_SET_BUFFERSIZE_HOST    20
#define JLINKARM_SWO_CMD_SET_BUFFERSIZE_EMU     21

/*********************************************************************
*
*       SWO Interfaces
*/
#define JLINKARM_SWO_IF_UART                    0
#define JLINKARM_SWO_IF_MANCHESTER              1         // Not supported in current version
#define JLINKARM_SWO_IF_TRACE                   2         // Only used internally, in case SWO ITM data is merged into trace stream

/*********************************************************************
*
*       Firmware update results
*/
#define JLINK_FW_UPDATE_RESULT_FW_INVALID             (6)
#define JLINK_FW_UPDATE_RESULT_EMU_FW_OLDER           (5)
#define JLINK_FW_UPDATE_RESULT_EMU_FW_NEWER           (4)
#define JLINK_FW_UPDATE_RESULT_EMU_FW_CURRENT         (3)
#define JLINK_FW_UPDATE_RESULT_NO_EMBEDDED_FW         (2)
#define JLINK_FW_UPDATE_RESULT_CANCELED_BY_USER       (1)
#define JLINK_FW_UPDATE_RESULT_OK                     (0)
#define JLINK_FW_UPDATE_RESULT_ERR_GENERIC            (-1)
#define JLINK_FW_UPDATE_RESULT_ERR_UNKNOWN_FW_ID      (-2)
#define JLINK_FW_UPDATE_RESULT_ERR_NO_TIMESTAMP       (-3)

/*********************************************************************
*
*       Obsolete, only for compatibility with older versions
*                 Do not use anymore
*/
#define JLINKARM_TIF_2_WIRE_JTAG_PIC32  JLINKARM_TIF_ICSP
#define JLINKARM_GO_OVERSTEP_BP         (1 << 0)      // Obsolete, use JLINKARM_GO_FLAG_OVERSTEP_BP instead
#define JLINKARM_EMU_CAP_TRACE          JLINKARM_EMU_CAP_TRACE_ARM79
#define JLINKARM_BP_TYPE_SW_RAM         JLINKARM_BP_IMP_SW_RAM
#define JLINKARM_BP_TYPE_SW_FLASH       JLINKARM_BP_IMP_SW_FLASH
#define JLINKARM_BP_TYPE_SW             JLINKARM_BP_IMP_SW
#define JLINKARM_BP_TYPE_HW             JLINKARM_BP_IMP_HW
#define JLINKARM_BP_TYPE_ANY            JLINKARM_BP_IMP_ANY
#define JLINKARM_BP_IMP_HW_ICE          (1 << 8)
#define JLINKARM_BP_IMP_HW_ETM          (1 << 9)
#define JLINKARM_BP_IMP_HW_NEXUS        (1 << 10)
typedef U32 JLINKARM_BP_TYPE;
#define JLINKARM_TIF_UART               JLINKARM_TIF_FINE
#define JLINK_MIPS_REG_DEPC             JLINK_MIPS_REG_PC
#define JLINKARM_CM3_RESET_TYPE_NORMAL                  ((JLINKARM_RESET_TYPE)(0))      // Use RESET pin. If this fails, try to reset core.
#define JLINKARM_CM3_RESET_TYPE_CORE                    ((JLINKARM_RESET_TYPE)(1))      // Resets the core only, not peripherals.
#define JLINKARM_CM3_RESET_TYPE_RESETPIN                ((JLINKARM_RESET_TYPE)(2))      // Resets core & peripherals using RESET pin.
#define JLINKARM_CM3_RESET_TYPE_CONNECT_UNDER_RESET     ((JLINKARM_RESET_TYPE)(3))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_HALT_AFTER_BTL          ((JLINKARM_RESET_TYPE)(4))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_HALT_BEFORE_BTL         ((JLINKARM_RESET_TYPE)(5))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_KINETIS                 ((JLINKARM_RESET_TYPE)(6))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_ADI_HALT_AFTER_KERNEL   ((JLINKARM_RESET_TYPE)(7))
#define JLINKARM_CM3_RESET_TYPE_CORE_AND_PERIPHERALS    ((JLINKARM_RESET_TYPE)(8))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_LPC1200                 ((JLINKARM_RESET_TYPE)(9))
#define JLINKARM_CM3_RESET_TYPE_S3FN60D                 ((JLINKARM_RESET_TYPE)(10))     // Performs a normal reset and disables the watchdog of the Samsung S3FN60D after the reset has been performed
#define JLINKARM_CM3_RESET_TYPE_LPC11A                  ((JLINKARM_RESET_TYPE)(11))
#define JLINKARM_CM3_RESET_TYPE_LPC176X                 ((JLINKARM_RESET_TYPE)(257))    // Used DLL-internally only. Automatically selected if LPC176x device is selected
#define JLINK_PPC_RESET_TYPE_NORMAL                     ((JLINKARM_RESET_TYPE)(0))      // Resets core & peripherals using RESET pin.

#define JLINK_CORE_CORTEX_M_V8BASEL                     JLINK_CORE_CORTEX_M23
#define JLINK_CORE_CORTEX_M_V8MAINL                     JLINK_CORE_CORTEX_M33

typedef enum {
  //
  // R0-R7 of each bank
  //
  JLINK_EFM8_R0_B0,     // Index 0: R0, bank 0
  JLINK_EFM8_R1_B0,
  JLINK_EFM8_R2_B0,
  JLINK_EFM8_R3_B0,
  JLINK_EFM8_R4_B0,
  JLINK_EFM8_R5_B0,
  JLINK_EFM8_R6_B0,
  JLINK_EFM8_R7_B0,
  JLINK_EFM8_R0_B1,     // Index 8: R0, bank 1
  JLINK_EFM8_R1_B1,
  JLINK_EFM8_R2_B1,
  JLINK_EFM8_R3_B1,
  JLINK_EFM8_R4_B1,
  JLINK_EFM8_R5_B1,
  JLINK_EFM8_R6_B1,
  JLINK_EFM8_R7_B1,
  JLINK_EFM8_R0_B2,     // Index 16: R0, bank 2
  JLINK_EFM8_R1_B2,
  JLINK_EFM8_R2_B2,
  JLINK_EFM8_R3_B2,
  JLINK_EFM8_R4_B2,
  JLINK_EFM8_R5_B2,
  JLINK_EFM8_R6_B2,
  JLINK_EFM8_R7_B2,
  JLINK_EFM8_R0_B3,     // Index 24: R0, bank 3
  JLINK_EFM8_R1_B3,
  JLINK_EFM8_R2_B3,
  JLINK_EFM8_R3_B3,
  JLINK_EFM8_R4_B3,
  JLINK_EFM8_R5_B3,
  JLINK_EFM8_R6_B3,
  JLINK_EFM8_R7_B3,
  //
  // Special registers like PC, stackpointer etc.
  //
  JLINK_EFM8_PC,        // Index 32: PC, 16-bit
  JLINK_EFM8_A,         // Accumulator
  JLINK_EFM8_B,
  JLINK_EFM8_DPTR,      // Data pointer register (16-bit)
  JLINK_EFM8_SP,
  JLINK_EFM8_PSW,
  //
  // Pseudo regs. Mapped to currenttly selected register bank
  // Bank is selected via PSW
  //
  JLINK_EFM8_R0,        // Index 38: Current R0 (pseudo reg)
  JLINK_EFM8_R1,
  JLINK_EFM8_R2,
  JLINK_EFM8_R3,
  JLINK_EFM8_R4,
  JLINK_EFM8_R5,
  JLINK_EFM8_R6,
  JLINK_EFM8_R7,
} JLINK_EFM8_REG;

typedef JLINK_FLASH_AREA_INFO   FLASH_AREA_INFO;
typedef JLINK_RAM_AREA_INFO     RAM_AREA_INFO;
//
// JLINKARM_READ_DESC
//
typedef struct JLINKARM_READ_DESC {
  U64 Addr;
  U32 NumBytes;
  U32 Flags;
} JLINKARM_READ_DESC;
//
// Backward compatibility for applications that used the old defines
//
#define JLINK_MEM_FLAG_WIDTH_DEFAULT         JLINK_MEM_ACC_WIDTH_DEFAULT
#define JLINK_MEM_FLAG_WIDTH_8BIT            JLINK_MEM_ACC_WIDTH_8BIT
#define JLINK_MEM_FLAG_WIDTH_16BIT           JLINK_MEM_ACC_WIDTH_16BIT
#define JLINK_MEM_FLAG_WIDTH_32BIT           JLINK_MEM_ACC_WIDTH_32BIT
#define JLINK_MEM_FLAG_WIDTH_64BIT           JLINK_MEM_ACC_WIDTH_64BIT

#define JLINKARM_DEV_FAMILY_CORTEX_AR_ARMV8  JLINK_DEV_FAMILY_CORTEX_AR_ARMV8
#define JLINK_CORE_RISC_V                    JLINK_CORE_RV32

#endif

/************************** end of file *****************************/
