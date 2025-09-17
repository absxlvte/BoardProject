#ifndef xDAC
#define xDAC
#include "stm32f4xx.h"                
#include "stm32f4xx_hal.h"       
extern DAC_HandleTypeDef hDAC;
extern uint8_t dacVal;

void InitDAC(void);
void task_DAC(void);
#endif
