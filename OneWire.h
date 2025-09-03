#pragma once
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"

#define ONEWIRE_BAUDRATE                                              115200
#define ONEWIRE_RESET_BAUDRATE                                        9600
#define ONEWIRE_RESET_BYTE                                            0xF0
#define ONEWIRE_UART_TIMEOUT                                          10
#define ONEWIRE_BITS_NUM                                              8

extern UART_HandleTypeDef xuart;
typedef enum
{
  ONEWIRE_OK              = 0x00,
  ONEWIRE_ERROR           = 0x01,
} ONEWIRE_Status;
/* Functions -----------------------------------------------------------------*/
extern ONEWIRE_Status OneWire_Reset(UART_HandleTypeDef *huart);
extern uint8_t OneWire_ProcessByte(UART_HandleTypeDef *huart, uint8_t byte);
extern uint8_t OneWire_ProcessBit(UART_HandleTypeDef *huart, uint8_t bit);
void UartInit(void);
