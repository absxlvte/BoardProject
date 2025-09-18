#ifndef AD7799
#define AD7799
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"

#define AD_SCLK GPIO_PIN_13 //PB13
#define AD_DIN GPIO_PIN_15 //PB15
#define AD_DOUT GPIO_PIN_14 //PB14
//registers
#define AD7799_REG_COMM        0x00
#define AD7799_REG_STATUS      0x00
#define AD7799_REG_MODE        0x01
#define AD7799_REG_CONFIG      0x02
#define AD7799_REG_DATA        0x03
#define AD7799_REG_ID          0x04
#define AD7799_REG_IO          0x05
#define AD7799_REG_OFFSET      0x06
#define AD7799_REG_FULLSCALE   0x07
//channels
#define AD7799_CH_AIN1         0x00
#define AD7799_CH_AIN2         0x01
#define AD7799_CH_AIN3         0x02
//gain
#define AD7799_GAIN_1          0x00
#define AD7799_GAIN_2          0x01
#define AD7799_GAIN_4          0x02
#define AD7799_GAIN_8          0x03
#define AD7799_GAIN_16         0x04
#define AD7799_GAIN_32         0x05
#define AD7799_GAIN_64         0x06
#define AD7799_GAIN_128        0x07
//mode
#define AD7799_MODE_CONT       0x00
#define AD7799_MODE_SINGLE     0x01
#define AD7799_MODE_IDLE       0x02
#define AD7799_MODE_PWRDN      0x03
//freqs
#define AD7799_RATE_470HZ      0x00
#define AD7799_RATE_242HZ      0x01
#define AD7799_RATE_123HZ      0x02
#define AD7799_RATE_62HZ       0x03
#define AD7799_RATE_50HZ       0x04
#define AD7799_RATE_39HZ       0x05
#define AD7799_RATE_33_2HZ     0x06
#define AD7799_RATE_19_6HZ     0x07
#define AD7799_RATE_16_7HZ     0x08
#define AD7799_RATE_16_7HZ_2   0x09
#define AD7799_RATE_12_5HZ     0x0A
#define AD7799_RATE_10HZ       0x0B
#define AD7799_RATE_8_33HZ     0x0C
#define AD7799_RATE_6_25HZ     0x0D
#define AD7799_RATE_4_17HZ     0x0E

uint8_t AD7799_Init(void);
HAL_StatusTypeDef AD7799_WriteRegister(uint8_t reg, uint16_t value);
uint32_t AD7799_ReadRegister(uint8_t reg, uint8_t size);
HAL_StatusTypeDef AD7799_SetChannel(uint8_t channel, uint8_t gain);
int32_t AD7799_ReadData(void);


extern SPI_HandleTypeDef xSPI;
extern uint8_t xID;

void SPI_Init(void);
//void SPI2_IRQHandler(void);


#endif
