#ifndef TMP119
#define TMP119
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"

#define SL_ADDR 0x48<<1 //within datasheet
//PB6 SCL
//PB7 SDA

#define TempResultReg 0x00
#define ConfigReg 		0x01
#define tHighLimReg 0x02
#define tLowLimReg 0x03
#define EEPROMunl 0x04
#define EEPROM1 0x05
#define EEPROM2 0x06
#define TempOffsetReg 0x07
#define EEPROM3 0x08
#define DevIDreg 0x0F


extern I2C_HandleTypeDef hi2c1;
void initI2C(void);
uint16_t readDataTMP119(uint8_t regAddr);
void writeDataTMP119(uint8_t regAddr, uint16_t regData);

#endif