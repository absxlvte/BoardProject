#include "TMP119.h"
I2C_HandleTypeDef xI2C;

uint8_t TMP117_DeviceID=TMP117_DeviceID1;
void I2C_Init(void){
	GPIO_InitTypeDef xGPIO_I2C;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	xGPIO_I2C.Pin=TMP119_SDA|TMP119_SCL;
	xGPIO_I2C.Mode=GPIO_MODE_AF_OD;
	xGPIO_I2C.Pull = GPIO_PULLUP;
	xGPIO_I2C.Alternate=GPIO_AF4_I2C1;
	xGPIO_I2C.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB,&xGPIO_I2C);
	
	__HAL_RCC_I2C1_CLK_ENABLE();
	xI2C.Instance = I2C1;
	xI2C.Init.ClockSpeed = 100000;
  xI2C.Init.DutyCycle = I2C_DUTYCYCLE_2;
  xI2C.Init.OwnAddress1 = 0;
  xI2C.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  xI2C.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  xI2C.Init.OwnAddress2 = 0;
  xI2C.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  xI2C.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&xI2C);
}

void TMP119_WriteReg(uint8_t reg, uint16_t value) {
    uint8_t data[2] = {value >> 8, value & 0xFF};
    HAL_I2C_Mem_Write(&xI2C, TMP119_Adress, reg, I2C_MEMADD_SIZE_8BIT, data, 2, 200);
}

uint16_t TMP119_ReadReg(uint8_t reg) {
    uint8_t data[2];
    HAL_I2C_Mem_Read(&xI2C, TMP119_Adress, reg, I2C_MEMADD_SIZE_8BIT, data, 2, 200);
    return (data[0] << 8) | data[1];
}

void TMP119_Init(void) {
    uint16_t config = 0x0220;
    TMP119_WriteReg(Config_Adress, config);
}

float TMP119_ReadTemperature(void) {
    uint16_t raw_temp = TMP119_ReadReg(TempRes_Adress);
    return (int16_t)raw_temp * 0.0078125f;
}

uint16_t TMP119_ReadDeviceID(void) {
    return TMP119_ReadReg(DeviceID_Adress);
}

uint16_t TMP117_get_ID_Register(I2C_HandleTypeDef *i2c)
{
      static uint8_t buf[3];
      buf[0]=TMP117_ID_Register;
      
      HAL_I2C_Master_Transmit(i2c,TMP117_I2C_W_ADDR_GND,buf,1,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(i2c,TMP117_I2C_R_ADDR_GND,buf,2,HAL_MAX_DELAY);
      return ((buf[0]<<8)|buf[1]);
      
}