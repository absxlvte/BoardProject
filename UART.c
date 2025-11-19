#include "UART.h"
UART_HandleTypeDef xuart;
volatile int8_t isPressed = 0;
volatile uint32_t last_press_time = 0;
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
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef TXRX;
	TXRX.Speed = GPIO_SPEED_FREQ_HIGH;
	TXRX.Pull = GPIO_PULLUP;
	TXRX.Mode = GPIO_MODE_AF_PP;
	TXRX.Alternate = GPIO_AF7_USART2;
	TXRX.Pin = GPIO_PIN_2|GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &TXRX);
	xuart.Instance = USART2;
	xuart.Init.BaudRate = 115200;
	xuart.Init.WordLength = UART_WORDLENGTH_8B;
	xuart.Init.Parity = UART_PARITY_NONE;
	xuart.Init.StopBits = UART_STOPBITS_1;
	xuart.Init.Mode=UART_MODE_TX_RX;
	HAL_UART_Init(&xuart);
}
//void InitButton(void){
//	GPIO_InitTypeDef Button;
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	Button.Pin=GPIO_PIN_0;
//	Button.Mode=GPIO_MODE_IT_RISING;
//	Button.Pull=GPIO_PULLDOWN;
//	Button.Speed=GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &Button);
//	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
//}
//void EXTI0_IRQHandler(void){
//	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
//	  uint32_t now = HAL_GetTick();
//        if (now - last_press_time > 150){
//            last_press_time = now;
//						isPressed = isPressed == 0 ? 1 : 0;
//				}
//}