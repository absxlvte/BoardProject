#pragma once
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"

#define TMP119_Adress 			0x48
#define TempRes_Adress 			0x00
#define Config_Adress 			0x01
#define THigh_Adress 				0x02
#define TLow_Adress 				0x03
#define EEPROM_UL_Adress 		0x04
#define EEPROM1_Adress 			0x05
#define EEPROM2_Adress 			0x06
#define TempOffset_Adress 	0x07
#define EEPROM3_Adress 			0x08
#define DeviceID_Adress 		0x0F

#define     TMP117_DeviceID1 0x48<<1    //	GND
#define     TMP117_ID_Register             0x0F

#define TMP117_I2C_R_ADDR_GND 	0x91			// 1001 0001 read
#define TMP117_I2C_W_ADDR_GND		0x90			// 1001 0000 write




#define TMP119_SDA GPIO_PIN_7  //PB7
#define TMP119_SCL GPIO_PIN_6  //PB6

extern I2C_HandleTypeDef xI2C;  


void I2C_Init(void);
void TMP119_Init(void);
void TMP119_WriteReg(uint8_t reg, uint16_t value);
uint16_t TMP119_ReadReg(uint8_t reg);
float TMP119_ReadTemperature(void);
uint16_t TMP119_ReadDeviceID(void);

uint16_t TMP117_get_ID_Register(I2C_HandleTypeDef *i2c);
