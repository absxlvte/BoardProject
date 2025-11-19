#include "TMP119.h"
I2C_HandleTypeDef hi2c1;
TMP_pack rec;
float f;
void initI2C(void){
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);
  
}

uint16_t readDataTMP119(uint8_t regAddr)
{
	uint16_t regData = 0;
  uint8_t data[2] = {0};
  HAL_I2C_Mem_Read(&hi2c1, SL_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, data, 2, 100);
  regData = (data[0] << 8) | data[1];
   
  return regData;
}

void writeDataTMP119(uint8_t regAddr, uint16_t regData) 
{	
	uint8_t data[2] = {regData >> 8, regData & 0xFF};
  HAL_I2C_Mem_Write(&hi2c1, SL_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, data, 2, 100);
}

int8_t I2C_Scan (void)
{
  HAL_StatusTypeDef res;                          
	for(uint16_t i = 1; i < 128; i++){
		res = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, HAL_MAX_DELAY);  		
    if(res == HAL_OK){
			return i;
		}
	}
	return -1;
}

void TMP119_Init(uint8_t mode, uint8_t conv_cycle, uint8_t avg){
	uint16_t params = 0;
	params |= mode << 10;
	params |= conv_cycle << 7;
	params |= avg << 5;
	params |= 1 << 2;
	writeDataTMP119(ConfigReg,params);
}

void InitDR(void){
	GPIO_InitTypeDef Button;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	Button.Pin=GPIO_PIN_0;
	Button.Mode=GPIO_MODE_IT_FALLING;
	Button.Pull=GPIO_PULLDOWN;
	Button.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &Button);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
void EXTI0_IRQHandler(void){
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	rec.data = readDataTMP119(TempResultReg);
	f = ((int8_t) rec.bytes[1] << 8 | rec.bytes[0]) * 0.0078125f; 
	//mC = ((int8_t) rec.bytes[1] << 8 | rec.bytes[0]) * 1000 >> 7; 
	//C = ((int8_t) rec.bytes[1] << 8 | rec.bytes[0]) >> 7;
}
