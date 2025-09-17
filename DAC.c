#include "DAC.h"

DAC_HandleTypeDef hDAC;
uint8_t dacVal = 0;

void InitDAC(void){
	__HAL_RCC_DAC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef ch;
	ch.Pin=GPIO_PIN_4;
	ch.Mode=GPIO_MODE_ANALOG;
	ch.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ch);
	
	hDAC.Instance=DAC1;
	HAL_DAC_Init(&hDAC);
	
	DAC_ChannelConfTypeDef channels;
	channels.DAC_Trigger=DAC_TRIGGER_NONE;
	channels.DAC_OutputBuffer=DAC_OUTPUTBUFFER_ENABLE;

	HAL_DAC_ConfigChannel(&hDAC,&channels,DAC_CHANNEL_1);
	
	HAL_DAC_Start(&hDAC,DAC_CHANNEL_1);
}
void task_DAC(void){
	HAL_DAC_SetValue(&hDAC,DAC_CHANNEL_1,DAC_ALIGN_8B_R,dacVal);
	dacVal++;
	HAL_Delay(20);
}
