#include "AD7799.h"
SPI_HandleTypeDef xSPI;
void SPI_Init(void){
	GPIO_InitTypeDef SPI_GPIO;
	__HAL_RCC_SPI2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	SPI_GPIO.Pin=AD_SCLK|AD_DIN|AD_DOUT;
	SPI_GPIO.Mode=GPIO_MODE_AF_PP;
	SPI_GPIO.Pull=GPIO_NOPULL;
	SPI_GPIO.Speed=GPIO_SPEED_FREQ_HIGH;
	SPI_GPIO.Alternate=GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB,&SPI_GPIO);
	
	GPIO_InitTypeDef CS_GPIO;
	CS_GPIO.Pin=AD_CS;
	CS_GPIO.Mode=GPIO_MODE_OUTPUT_PP;
	CS_GPIO.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB,&CS_GPIO);
	
	
	xSPI.Instance=SPI2;
	xSPI.Init.Mode=SPI_MODE_MASTER;
	xSPI.Init.Direction=SPI_DIRECTION_2LINES ;
	xSPI.Init.DataSize=SPI_DATASIZE_8BIT;
	xSPI.Init.CLKPolarity=SPI_POLARITY_HIGH;	//
	xSPI.Init.CLKPhase=SPI_PHASE_2EDGE;  		//
	//xSPI.Init.NSS=SPI_NSS_SOFT;     			// SOFT or HARD?
	xSPI.Init.NSS=SPI_NSS_HARD_OUTPUT;      // SOFT or HARD?
	xSPI.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_128; //32 or 64 or ... ?
	xSPI.Init.FirstBit=SPI_FIRSTBIT_MSB;
	HAL_SPI_Init(&xSPI);
}


void AD7799_SetRegisterValue(unsigned char regAddress,
														 unsigned long regValue, 
														 unsigned char size) {
	unsigned char data[4] = {0x00, 0x00, 0x00, 0x00};	
	data[0] = AD7799_COMM_WRITE | AD7799_COMM_ADDR(regAddress);
	switch (size){
		case 1:
      data[1] = (unsigned char)regValue;
			break;
    case 2:
			data[2] = (unsigned char)((regValue & 0x0000FF) >> 0);
      data[1] = (unsigned char)((regValue & 0x00FF00) >> 8);
			break;
    case 3:
			data[3] = (unsigned char)((regValue & 0x0000FF) >> 0);
			data[2] = (unsigned char)((regValue & 0x00FF00) >> 8);
      data[1] = (unsigned char)((regValue & 0xFF0000) >> 16);
			break;
	}
	AD7799_CS_LOW;	    
	HAL_SPI_Transmit(&xSPI, data, 1+size, 100);
	AD7799_CS_HIGH;
}
unsigned long AD7799_GetRegisterValue(unsigned char regAddress, 
																			unsigned char size){
	unsigned char data[4] = {0x00, 0x00, 0x00, 0x00};
	unsigned long receivedData = 0x00;	
	data[0] = AD7799_COMM_READ | AD7799_COMM_ADDR(regAddress);
	AD7799_CS_LOW;  
	HAL_SPI_Transmit(&xSPI, data, 1, 100);
	//HAL_Delay(10);
	HAL_SPI_Receive(&xSPI, data, size, 100);
	AD7799_CS_HIGH;
	switch (size){
		case 1:
			receivedData += (data[0] << 0);
			break;
		case 2:
			receivedData += (data[0] << 8);
			receivedData += (data[1] << 0);
			break;
		case 3:
			receivedData += (data[0] << 16);
			receivedData += (data[1] << 8);
			receivedData += (data[2] << 0);
			break;
	}
    return receivedData;
}
																			
void AD7799_Reset(void){
	unsigned char data[4] = {0xFF, 0xFF, 0xFF, 0xFF};	
	AD7799_CS_LOW;  
	HAL_SPI_Transmit(&xSPI, data, 4, 100);
	AD7799_CS_HIGH;
}

uint8_t AD7799_Init(void){
	AD7799_Reset();
	AD7799_SetRegisterValue(AD7799_REG_MODE,AD7799_MODE_SEL(AD7799_MODE_IDLE),2);
	uint8_t ID = 0;
	uint8_t status = 0;
	ID = (uint8_t) AD7799_GetRegisterValue(AD7799_REG_ID,1);
	status = ((ID & AD7799_ID_MASK) == AD7799_ID) ? 0 : 1;
	return status;
}
void AD7799_SetMode(unsigned long Mode){
	unsigned long command;
  command = AD7799_GetRegisterValue(AD7799_REG_MODE,2);
  command &= ~AD7799_MODE_SEL(0xFF);
  command |= AD7799_MODE_SEL(Mode);
  AD7799_SetRegisterValue(AD7799_REG_MODE,command,2);
}

void AD7799_SetGain(unsigned long Gain, unsigned long Polarity){
  unsigned long command;
  command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
  command &= ~AD7799_CONF_GAIN(0xFF);
  command |= AD7799_CONF_GAIN(Gain);
	//command |= Polarity;
  AD7799_SetRegisterValue(AD7799_REG_CONF,command,2);
}

void AD7799_SetChannel(unsigned long Channel){
    unsigned long command;
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
    command &= ~AD7799_CONF_CHAN(0xFF);
    command |= AD7799_CONF_CHAN(Channel);
    AD7799_SetRegisterValue(AD7799_REG_CONF,command,2);
}

uint8_t AD7799_isDataReady(void){
	uint8_t rdy = 0;
    rdy = (AD7799_GetRegisterValue( AD7799_REG_STAT,1) & 0x80);   
	return rdy;
}