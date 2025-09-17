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
	xSPI.Init.CLKPolarity=SPI_POLARITY_HIGH;
	xSPI.Init.CLKPhase=SPI_PHASE_1EDGE;
	xSPI.Init.NSS=SPI_NSS_HARD_OUTPUT;
	xSPI.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_64;
	xSPI.Init.FirstBit=SPI_FIRSTBIT_MSB;
	HAL_SPI_Init(&xSPI);
	//HAL_NVIC_EnableIRQ(SPI2_IRQn); 
}

//void SPI2_IRQHandler(void){	
//	HAL_SPI_IRQHandler(&xSPI);	
//}

//void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef  *hspi){ 
//	if(hspi == &xSPI) {
//		HAL_SPI_Receive_IT(&xSPI,&xID,1);
//	}     
//}
//void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef  *hspi){ 
//	if(hspi == &xSPI) {
//		
//	}     
//}