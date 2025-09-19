#include "AD7799.h"
SPI_HandleTypeDef xSPI;
uint8_t xID = 0;
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
	
	xSPI.Instance=SPI2;
	xSPI.Init.Mode=SPI_MODE_MASTER;
	xSPI.Init.Direction=SPI_DIRECTION_2LINES ;
	xSPI.Init.DataSize=SPI_DATASIZE_8BIT;
	xSPI.Init.CLKPolarity=SPI_POLARITY_LOW;	//CPOL=1
	xSPI.Init.CLKPhase=SPI_PHASE_2EDGE;  		//CPHA=1
	//xSPI.Init.NSS=SPI_NSS_SOFT;     			// SOFT or HARD?
	xSPI.Init.NSS=SPI_NSS_HARD_OUTPUT;      // SOFT or HARD?
	xSPI.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_2; //32 or 64 or ... ?
	xSPI.Init.FirstBit=SPI_FIRSTBIT_MSB;
	HAL_SPI_Init(&xSPI);
	//HAL_NVIC_EnableIRQ(SPI2_IRQn); 
}


uint8_t AD7799_Init(void) {
    //HAL_Delay(10);
    //uint8_t reset_cmd[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    //HAL_SPI_Transmit(&xSPI, reset_cmd, 4, 100);
    //HAL_Delay(1);
    uint8_t id = AD7799_ReadRegister(AD7799_REG_ID, 1);
    return id;
}

HAL_StatusTypeDef AD7799_WriteRegister(uint8_t reg, uint16_t value) {
    uint8_t tx_data[3];
    tx_data[0] = (0 << 7) | (0 << 6) | ((reg & 0x7) << 3);
    tx_data[1] = value >> 8;
    tx_data[2] = value & 0xFF;
    
    return HAL_SPI_Transmit(&xSPI, tx_data, 3, 100);
}

uint32_t AD7799_ReadRegister(uint8_t reg, uint8_t size) {
    uint8_t tx_data[4] = {0};
    uint8_t rx_data[4] = {0};
    
    tx_data[0] = (0 << 7) | (1 << 6) | ((reg & 0x7) << 3);
    
    HAL_SPI_TransmitReceive(&xSPI, tx_data, rx_data, size + 1, 100);
    
    uint32_t result = 0;
    for(int i = 0; i < size; i++) {
        result = (result << 8) | rx_data[i + 1];
    }
    
    return result;
}

HAL_StatusTypeDef AD7799_SetChannel(uint8_t channel, uint8_t gain) {
    uint16_t config = (1 << 4) | (gain << 8) | (channel & 0x7);
    AD7799_WriteRegister(AD7799_REG_CONFIG, config);
    uint16_t mode = (AD7799_MODE_CONT << 13) | (AD7799_RATE_16_7HZ << 0);
    return AD7799_WriteRegister(AD7799_REG_MODE, mode);
}

uint32_t AD7799_ReadData(void) {
    return AD7799_ReadRegister(AD7799_REG_DATA, 3);
}