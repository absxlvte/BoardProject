#ifndef TMP119
#define TMP119
#include "stm32f4xx.h"                 
#include "stm32f4xx_hal.h"

#define SL_ADDR 0x48<<1 // HAL documentation
//#define SL_ADDR 0x48

//#define SL_ADDR 0b10010000 // HAL documentation
//PB6 SCL
//PB7 SDA
//PB8 dR

#define TempResultReg 0x00
#define ConfigReg 		0x01
#define tHighLimReg 	0x02
#define tLowLimReg 		0x03
#define EEPROMunl			0x04
#define EEPROM1 			0x05
#define EEPROM2 			0x06
#define TempOffsetReg 0x07
#define EEPROM3 			0x08
#define DevIDreg 			0x0F
 
#define modeCC   0x00
#define modeSD	 0x01
#define modeCCx  0x02
#define modeOS	 0x03

#define convCycle1     0x00
#define convCycle2     0x01
#define convCycle3     0x02
#define convCycle4     0x03
#define convCycle5     0x04
#define convCycle6     0x05
#define convCycle7     0x06
#define convCycle8     0x07

#define avg0  0x00
#define avg8  0x01
#define avg32 0x02
#define avg64 0x03

extern I2C_HandleTypeDef hi2c1;
extern float f;
union TMP_pack{
	uint8_t bytes[2];
	uint16_t data;
}typedef TMP_pack;
extern TMP_pack rec;
void initI2C(void);
uint16_t readDataTMP119(uint8_t regAddr);
void writeDataTMP119(uint8_t regAddr, uint16_t regData);
int8_t I2C_Scan (void);
void TMP119_Init(uint8_t mode, uint8_t conv_cycle, uint8_t avg);
void InitDR(void);
void EXTI0_IRQHandler(void);
#endif