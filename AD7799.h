#ifndef AD7799
#define AD7799
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"

#define AD_SCLK GPIO_PIN_13 //PB13
#define AD_DIN GPIO_PIN_15 //PB15
#define AD_DOUT GPIO_PIN_14 //PB14

extern SPI_HandleTypeDef xSPI;
extern uint8_t xID;

void SPI_Init(void);
//void SPI2_IRQHandler(void);


#endif
