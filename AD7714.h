#pragma once
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"

//AD7714 pins
#define AD_SCLK GPIO_PIN_13 //PB13
#define AD_DIN GPIO_PIN_15 //PB15
#define AD_DOUT GPIO_PIN_14 //PB14
#define AD_DRDY GPIO_PIN_6 //PA6



//channels AIN+ and AIN-
#define AIN1_AIN6 				0x00
#define AIN2_AIN6 				0x01
#define AIN3_AIN6 				0x02
#define AIN4_AIN6 				0x03
#define AIN1_AIN2 				0x04 //RTD
#define AIN3_AIN4 				0x05 //LMT70
#define AIN5_AIN6 				0x06
#define AIN6_AIN6 				0x07

//Registers
#define CommReg 					0x00
#define ModeReg 					0x01
#define MFilter 					0x02
#define LFilter 					0x03
#define TestReg 					0x04 //DON'T USE
#define DataReg 					0x05
#define ZeroCallReg 			0x06
#define FullScaleReg 			0x07

//Modes
#define Normal 						0x00
#define SelfCal 					0x01
#define SystemCalZero 		0x02
#define SystemCalScale 		0x03
#define SystemCalOffset 	0x04
#define BackCal 					0x05
#define SelfCalZero 			0x06
#define SelfCalScale 			0x07

extern SPI_HandleTypeDef xSPI;

void SPI_Init(void);
void AD_WriteRegister(uint8_t reg, uint8_t channels, uint8_t *data, uint8_t len);
void AD_ReadRegister(uint8_t reg, uint8_t channels, uint8_t *data, uint8_t len);
uint8_t AD_DataReady(void);
	
void AD_Init(void);
uint32_t AD_ReadData(uint8_t channel);