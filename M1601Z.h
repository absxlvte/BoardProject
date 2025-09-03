#pragma once
#include "OneWire.h" 

#define M1601Z_SERIAL_NUMBER_LEN_BYTES                                     8
#define M1601Z_SERIAL_NUMBER_OFFSET_BYTES                                  1

#define M1601Z_9_BITS_CONFIG                                               0x1F
#define M1601Z_10_BITS_CONFIG                                              0x3F
#define M1601Z_11_BITS_CONFIG                                              0x5F
#define M1601Z_12_BITS_CONFIG                                              0x7F

#define M1601Z_9_BITS_DELAY_MS                                             94
#define M1601Z_10_BITS_DELAY_MS                                            188
#define M1601Z_11_BITS_DELAY_MS                                            375
#define M1601Z_12_BITS_DELAY_MS                                            750

#define M1601Z_SCRATCHPAD_T_LSB_BYTE_IDX                                   0
#define M1601Z_SCRATCHPAD_T_MSB_BYTE_IDX                                   1
#define M1601Z_SCRATCHPAD_T_LIMIT_H_BYTE_IDX                               2
#define M1601Z_SCRATCHPAD_T_LIMIT_L_BYTE_IDX                               3
#define M1601Z_SCRATCHPAD_CONFIG_BYTE_IDX                                  4

#define M1601Z_9_BITS_DATA_MASK                                            0x7F8
#define M1601Z_10_BITS_DATA_MASK                                           0x7FC
#define M1601Z_11_BITS_DATA_MASK                                           0x7FE
#define M1601Z_12_BITS_DATA_MASK                                           0x7FF
#define M1601Z_SIGN_MASK                                                   0xF800

#define M1601Z_T_STEP                                                      0.004

#define M1601Z_READ_ROM_RX_BYTES_NUM                                       8
#define M1601Z_READ_SCRATCHPAD_RX_BYTES_NUM                                9

typedef struct M1601Z
{
  uint8_t isInitialized;
  uint8_t isConnected;
  UART_HandleTypeDef *uart;
  uint8_t serialNumber[M1601Z_SERIAL_NUMBER_LEN_BYTES];
  uint8_t temperatureLimitLow;
  uint8_t temperatureLimitHigh;
  uint8_t configRegister;
	uint8_t alarmStatus; 
  uint8_t repeatability;
  uint8_t measurementMode;
  float temperature;
} M1601Z;

typedef struct M1601Z_Command
{
  uint8_t code;
  uint8_t rxBytesNum;
  uint8_t txBytesNum;
} M1601Z_Command;

typedef enum
{
  M1601Z_OK              = 0x00,
  M1601Z_ERROR           = 0x01,
} M1601Z_Status;

typedef enum
{
  M1601Z_NONE            = 0x00,
  M1601Z_DATA            = 0x01,
  M1601Z_DELAY           = 0x02,
} M1601Z_WaitCondition;


void M1601Z_Init(M1601Z *sensor, UART_HandleTypeDef *huart);
static uint8_t CalculateChecksum(uint8_t *data, uint8_t length);
M1601Z_Status M1601Z_InitializationCommand(M1601Z *sensor);
static M1601Z_Status ExecuteCommand(M1601Z *sensor, M1601Z_Command command, uint8_t *data);
M1601Z_Status M1601Z_SkipRom(M1601Z *sensor);
M1601Z_Status M1601Z_ConvertT(M1601Z *sensor, M1601Z_WaitCondition waitCondition);
static void WaitForConversionFinished(M1601Z *sensor);
M1601Z_Status M1601Z_WriteScratchpad(M1601Z *sensor, uint8_t *data);
M1601Z_Status M1601Z_ReadScratchpad(M1601Z *sensor);