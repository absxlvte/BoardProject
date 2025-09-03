#include "M1601Z.h" 

static M1601Z_Command skipRom = {.code = 0xCC, .rxBytesNum = 0, .txBytesNum = 0};

static M1601Z_Command convertT = {.code = 0x44, .rxBytesNum = 0, .txBytesNum = 0};
static M1601Z_Command readScratchpad = {.code = 0xBE, .rxBytesNum = 9, .txBytesNum = 0};
static M1601Z_Command writeScratchpad = {.code = 0x4E, .rxBytesNum = 0, .txBytesNum = 3};

void M1601Z_Init(M1601Z *sensor, UART_HandleTypeDef *huart)
{
  sensor->isConnected = 0;
  sensor->uart = huart;
  sensor->isInitialized = 1;
}
static uint8_t CalculateChecksum(uint8_t *data, uint8_t length)
{
  uint8_t checksum = 0;
  while (length--)
  {
    uint8_t currentByte = *data++;
    for (uint8_t i = 8; i; i--)
    {
      uint8_t temp = (checksum ^ currentByte) & 0x01;
      checksum >>= 1;
      if (temp)
      {
        checksum ^= 0x8C;
      }
      currentByte >>= 1;
    }
  }
  return checksum;
}
M1601Z_Status M1601Z_InitializationCommand(M1601Z *sensor)
{
  if (sensor->isInitialized == 0)
  {
    return M1601Z_ERROR;
  }
  ONEWIRE_Status status = OneWire_Reset(sensor->uart);
  if (status == ONEWIRE_OK)
  {
    sensor->isConnected = 1;
    return M1601Z_OK;
  }
  else
  {
    sensor->isConnected = 0;
    return M1601Z_ERROR;
  }
}

static M1601Z_Status ExecuteCommand(M1601Z *sensor, M1601Z_Command command, uint8_t *data)
{
  if (sensor->isConnected == 0)
  {
    return M1601Z_ERROR;
  }
  OneWire_ProcessByte(sensor->uart, command.code);
  if (command.rxBytesNum != 0)
  {
    for (uint8_t i = 0; i < command.rxBytesNum; i++)
    {
      data[i] = OneWire_ProcessByte(sensor->uart, 0xFF);
    }
    uint8_t checkSum = CalculateChecksum(data, command.rxBytesNum - 1);
    if (checkSum != data[command.rxBytesNum - 1])
    {
      return M1601Z_ERROR;
    }
  }
  else
  {
    for (uint8_t i = 0; i < command.txBytesNum; i++)
    {
      OneWire_ProcessByte(sensor->uart, data[i]);
    }
  }
  return M1601Z_OK;
}

M1601Z_Status M1601Z_SkipRom(M1601Z *sensor)
{
  M1601Z_Status result;
  uint8_t rxDummyData;
  result = ExecuteCommand(sensor, skipRom, &rxDummyData);
  if (result != M1601Z_OK)
  {
    return result;
  }
  return M1601Z_OK;
}

M1601Z_Status M1601Z_ConvertT(M1601Z *sensor, M1601Z_WaitCondition waitCondition)
{
  M1601Z_Status result;
  uint8_t rxDummyData;
  result = ExecuteCommand(sensor, convertT, &rxDummyData);
  if (waitCondition == M1601Z_DATA)
  {
    WaitForConversionFinished(sensor);
  }
  if (waitCondition == M1601Z_DELAY)
  {
    uint32_t delayValueMs = 0;
    switch (sensor->configRegister)
    {
      case M1601Z_9_BITS_CONFIG:
        delayValueMs = M1601Z_9_BITS_DELAY_MS;
        break;
      case M1601Z_10_BITS_CONFIG:
        delayValueMs = M1601Z_10_BITS_DELAY_MS;
        break;
      case M1601Z_11_BITS_CONFIG:
        delayValueMs = M1601Z_11_BITS_DELAY_MS;
        break;
      case M1601Z_12_BITS_CONFIG:
        delayValueMs = M1601Z_12_BITS_DELAY_MS;
        break;
      default:
        break;
    }
    HAL_Delay(delayValueMs);
  }
  return result;
}

static void WaitForConversionFinished(M1601Z *sensor)
{
  uint8_t data = OneWire_ProcessBit(sensor->uart, 1);
  while(data != 0xFF)
  {
    data = OneWire_ProcessBit(sensor->uart, 1);
  }
}

M1601Z_Status M1601Z_ReadScratchpad(M1601Z *sensor)
{
  M1601Z_Status result;
  uint8_t rxData[M1601Z_READ_SCRATCHPAD_RX_BYTES_NUM];
  result = ExecuteCommand(sensor, readScratchpad, rxData);
  if (result != M1601Z_OK)
  {
    return result;
  }
  sensor->temperatureLimitHigh = rxData[M1601Z_SCRATCHPAD_T_LIMIT_H_BYTE_IDX];
  sensor->temperatureLimitLow = rxData[M1601Z_SCRATCHPAD_T_LIMIT_L_BYTE_IDX];
  sensor->configRegister = rxData[M1601Z_SCRATCHPAD_CONFIG_BYTE_IDX];
  uint16_t tRegValue = (rxData[M1601Z_SCRATCHPAD_T_MSB_BYTE_IDX] << 8) | rxData[M1601Z_SCRATCHPAD_T_LSB_BYTE_IDX];
  uint16_t sign = tRegValue & M1601Z_SIGN_MASK;
  if (sign != 0)
  {
    tRegValue = (0xFFFF - tRegValue + 1);
  }
  switch (sensor->configRegister)
  {
    case M1601Z_9_BITS_CONFIG:
      tRegValue &= M1601Z_9_BITS_DATA_MASK;
      break;
    case M1601Z_10_BITS_CONFIG:
      tRegValue &= M1601Z_10_BITS_DATA_MASK;
      break;
    case M1601Z_11_BITS_CONFIG:
      tRegValue &= M1601Z_11_BITS_DATA_MASK;
      break;
    case M1601Z_12_BITS_CONFIG:
      tRegValue &= M1601Z_12_BITS_DATA_MASK;
      break;
    default:
      tRegValue &= M1601Z_12_BITS_DATA_MASK;
      break;
  }
	sensor->temperature = 40.0 + ((float)tRegValue / 256.0);
  if (sign != 0)
  {
    sensor->temperature *= (-1);
  }
  return M1601Z_OK;
}

M1601Z_Status M1601Z_WriteScratchpad(M1601Z *sensor, uint8_t *data)
{
  M1601Z_Status result;
  result = ExecuteCommand(sensor, writeScratchpad, data);
  if (result != M1601Z_OK)
  {
    return result;
  }
  sensor->temperatureLimitHigh = data[0];
  sensor->temperatureLimitLow = data[1];
  sensor->configRegister = data[2];
  return result;
}