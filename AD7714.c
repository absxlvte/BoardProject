#include "AD7714.h"
SPI_HandleTypeDef xSPI;
void SPI_Init(void){
	GPIO_InitTypeDef GPIO_DRDY;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_DRDY.Pin=AD_DRDY;
	GPIO_DRDY.Mode=GPIO_MODE_INPUT;
	GPIO_DRDY.Pull=GPIO_PULLDOWN;
	GPIO_DRDY.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_DRDY);
	
	GPIO_InitTypeDef SPI_GPIO;
	__HAL_RCC_SPI2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	SPI_GPIO.Pin=AD_SCLK|AD_DIN|AD_DOUT;
	SPI_GPIO.Mode=GPIO_MODE_AF_PP;
	SPI_GPIO.Pull=GPIO_NOPULL;
	SPI_GPIO.Speed=GPIO_SPEED_FREQ_HIGH;
	SPI_GPIO.Alternate=GPIO_AF5_SPI1;//
	HAL_GPIO_Init(GPIOB,&SPI_GPIO);
	
	xSPI.Instance=SPI2;
	xSPI.Init.Mode=SPI_MODE_MASTER;
	xSPI.Init.Direction=SPI_DIRECTION_2LINES ;
	xSPI.Init.DataSize=SPI_DATASIZE_8BIT;
	//xSPI.Init.CLKPolarity=SPI_POLARITY_HIGH; //
	xSPI.Init.CLKPolarity=SPI_POLARITY_LOW; //pol pin ->gnd
	xSPI.Init.CLKPhase=SPI_PHASE_1EDGE;
	xSPI.Init.NSS=SPI_NSS_HARD_OUTPUT;
	xSPI.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_64;
	xSPI.Init.FirstBit=SPI_FIRSTBIT_MSB;
	HAL_SPI_Init(&xSPI);
	HAL_NVIC_EnableIRQ(SPI1_IRQn); 
}

void AD_WriteRegister(uint8_t reg, uint8_t channels, uint8_t *data, uint8_t len){
	uint8_t info = (0x00 | (reg << 4)) | (channels);

  HAL_SPI_Transmit(&xSPI, &info, 1, 200);

  HAL_SPI_Transmit(&xSPI, data, len, 200);
}
void AD_ReadRegister(uint8_t reg, uint8_t channels, uint8_t *data, uint8_t len){
	uint8_t info = (0x08 | (reg << 4)) | (channels);
    
  HAL_SPI_Transmit(&xSPI, &info, 1, 200);
	while(HAL_GPIO_ReadPin(GPIOA, AD_DRDY) == GPIO_PIN_RESET);
	HAL_SPI_Receive(&xSPI, data, len, 200);
}

uint8_t AD_DataReady(void) {
    return (HAL_GPIO_ReadPin(GPIOA, AD_DRDY) == GPIO_PIN_RESET);
}

void AD_Init(void){
	uint8_t filterHigh = 0xC3; // 1 1 0 0 0011 - N=4000 quartz 24b unipolar
	uint8_t filterLow = 0xE8;  // 11101000 
	uint8_t mode = 0x00;			 // 0 0 000 000  - gain=1 mode=normal 
	//uint8_t calibrate = 0x20;  //	0 0 100 000  
	
	AD_WriteRegister(MFilter,AIN3_AIN4,&filterHigh,1);
	AD_WriteRegister(LFilter,AIN3_AIN4,&filterLow,1);
	//AD_WriteRegister(ModeReg,AIN3_AIN4,&calibrate,1);
	AD_WriteRegister(ModeReg,AIN3_AIN4,&mode,1);
	
}

uint32_t AD_ReadData(uint8_t channel){
	while(!AD_DataReady()) {
        HAL_Delay(1);
    }
  uint8_t data[3] = {0};
  uint8_t info = ((DataReg << 4) | (1 << 3)) | channel; // 0 101 1 101
    
  HAL_SPI_Transmit(&xSPI, &info, 1, 200);
  HAL_SPI_Receive(&xSPI, data, 3, 200);
  
	int32_t result = ((int32_t)data[0] << 16) | ((int32_t)data[1] << 8) | data[2];
    
  return result;
}