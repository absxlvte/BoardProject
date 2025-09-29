#ifndef UART
#define UART
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"
extern UART_HandleTypeDef xuart;
void UartInit(void);

#endif