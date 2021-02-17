#ifndef MB_PROTOCOL_H
#define MB_PROTOCOL_H

#include <stdint.h>

enum {
  MB_SYSTEM_RESET                    = 1,
  MB_SWITCH_SWD                      = 2,                  // data[0] - DUT index, RES - error
  MB_SWITCH_POWER                    = 3,                  // data[0] - DUT index, data[1] - boolean state, RES - error
  MB_READ_DIN                        = 4,                  // data[0] - DUT index, data[1] - DIN index, RES - boolean state or error
  MB_WRITE_DOUT                      = 5,                  // data[0] - DUT index, data[1] - DOUT index, data[2] - boolean state, RES - error
  MB_READ_CSA                        = 6,                  // data[0] - gain, RET - raw value or error
  MB_READ_ANALOG                     = 7,                  // data[0] - DUT index, data[1] - AIN index, data[2] - gain, RES - raw value or error
  MB_CONFIG_DUT_DEBUG                = 8,                  // data[0] - DUT index, data[1:4] - baud rate, data[5] - bits, data[6] - parity, data[7] - stop bits, RES - error
  MB_SWITCH_DALI                     = 9,                  // data[0] - boolean state, RES - error
  MB_READ_DALI_ADC                   = 10,                 // RES - raw value or error
  MB_READ_DIN_ADC                    = 11,                 // data[0] - DUT index, data[1] - DIN index, RES - raw value or error
  MB_READ_ADC_24V                    = 12,                 // RET - raw value or error
  MB_READ_ADC_3V                     = 13,                 // RET - raw value or error
  MB_READ_ADC_TEMP                   = 14,                 // RET - raw value or error
};

enum {
  MB_STARTUP                         = 0x8000,             // On start-up and after MB_SYSTEM_RESET
  MB_ASYNC_EVENT                     = 0x8001,
  MB_GENERAL_RESULT                  = 0x8002,             // MB_SWITCH_SWD, MB_SWITCH_POWER, MB_READ_DIN, MB_WRITE_DOUT, MB_READ_CSA, MB_READ_ANALOG, MB_CONFIG_DUT_DEBUG,
                                                           // MB_SWITCH_DALI, MB_READ_DALI_ADC, MB_READ_DIN_ADC, MB_READ_ADC_24V, MB_READ_ADC_3V, MB_READ_ADC_TEMP
};

enum {
  MB_EVENT_SLIP_ERROR                = 1,
  MB_EVENT_COMMAND_TOO_LONG          = 2,
  MB_EVENT_CMDQUEUE_FULL             = 3,
  MB_EVENT_DUTDBGTX_FULL             = 4,
  MB_EVENT_INVALID_CHANNEL           = 5,
};

enum {
  MB_NO_ERROR                        = 0,
  MB_ERROR_COMMAND_TOO_SHORT         = -1,
  MB_ERROR_INVALID_DATA_SIZE         = -2,
  MB_ERROR_INVALID_PACKET_TYPE       = -3,
  MB_ERROR_INVALID_ARGUMENT          = -4,
  MB_ERROR_I2C1_TIMEOUT              = -5,
  MB_ERROR_ADC_TIMEOUT               = -6,
  MB_ERROR_UART_INIT                 = -7,
  MB_ERROR_ADC_CONFIG_CHANNEL        = -8,
};

#pragma pack (push, 1)
struct MB_Packet_t
{
  uint16_t type;
  uint8_t
    sequence,
    dataLen;
  uint8_t* data;
};
#pragma pack (pop)

#pragma pack (push, 1)
struct MB_ConfigDutDebug_t
{
  MB_Packet_t header;
  uint8_t dutIndex;                                        // 1, 2, 3
  uint32_t baudRate;
  uint8_t
    bits,                                                  // 8 or 9
    parity,                                                // 0 - none, 1 - even, 2 - odd
    stopBits;                                              // 1 or 2
};
#pragma pack (pop)

#pragma pack (push, 1)
struct MB_GeneralResult_t
{
  MB_Packet_t header;
  int32_t errorCode;
};
#pragma pack (pop)

#pragma pack (push, 1)
struct MB_Event_t
{
  MB_Packet_t header;
  int32_t eventCode;
};
#pragma pack (pop)

#endif // MB_PROTOCOL_H
