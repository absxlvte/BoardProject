#include "UART.h"
UART_HandleTypeDef xuart;
//void UartInit(void){
//	__HAL_RCC_USART3_CLK_ENABLE();
//	__HAL_RCC_GPIOD_CLK_ENABLE();
//	GPIO_InitTypeDef TXRX;
//	TXRX.Speed = GPIO_SPEED_FREQ_HIGH;
//	TXRX.Pull = GPIO_PULLUP;
//	TXRX.Mode = GPIO_MODE_AF_PP;
//	TXRX.Alternate = GPIO_AF7_USART3;
//	TXRX.Pin = GPIO_PIN_8|GPIO_PIN_9;
//	HAL_GPIO_Init(GPIOD, &TXRX);
//	xuart.Instance = USART3;
//	xuart.Init.BaudRate = 115200;
//	xuart.Init.WordLength = UART_WORDLENGTH_8B;
//	xuart.Init.Parity = UART_PARITY_NONE;
//	xuart.Init.StopBits = UART_STOPBITS_1;
//	xuart.Init.Mode=UART_MODE_TX_RX;
//	HAL_UART_Init(&xuart);
//}
void UartInit(void){
	__HAL_RCC_UART4_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef TXRX;
	TXRX.Speed = GPIO_SPEED_FREQ_HIGH;
	TXRX.Pull = GPIO_PULLUP;
	TXRX.Mode = GPIO_MODE_AF_PP;
	TXRX.Alternate = GPIO_AF8_UART4;
	TXRX.Pin = GPIO_PIN_0|GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &TXRX);
	xuart.Instance = UART4;
	xuart.Init.BaudRate = 115200;
	xuart.Init.WordLength = UART_WORDLENGTH_8B;
	xuart.Init.Parity = UART_PARITY_NONE;
	xuart.Init.StopBits = UART_STOPBITS_1;
	xuart.Init.Mode=UART_MODE_TX_RX;
	HAL_UART_Init(&xuart);
}