#ifndef UART
#define UART
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"
extern UART_HandleTypeDef xuart;
extern volatile int8_t isPressed;
void UartInit(void);
void InitButton(void);
//void EXTI0_IRQHandler(void);
#endif