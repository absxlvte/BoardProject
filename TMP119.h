#ifndef TMP119
#define TMP119
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"

#define SL_ADDR 0x48

extern I2C_HandleTypeDef hi2c1;
void initI2C(void);
uint8_t readDataTMP119(uint8_t regAddr);
void writeDataTMP119(uint8_t regAddr, uint8_t regData);

#endif