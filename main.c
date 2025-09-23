#include "AD7799.h"

void SystemClock_Config(void);
void SysTick_Handler(void);

int main(void){
	HAL_Init();
	SystemClock_Config();
	SPI_Init();
	AD7799_CS_HIGH;
	uint8_t status = 0xFF;
	uint8_t rdy = 0;
	unsigned long Data = 0;
	double mvData = 0;
	double Temperature = 0;
	status = AD7799_Init();
	
	AD7799_SetMode(AD7799_MODE_CAL_INT_ZERO);
	HAL_Delay(10);
	AD7799_SetMode(AD7799_MODE_CONT);
	AD7799_SetChannel(AD7799_CH_AIN1P_AIN1M);
	AD7799_SetGain(AD7799_GAIN_1, AD7799_CONF_UNIPOLAR);

	while(1){
		rdy = AD7799_isDataReady();
		if (!rdy){
			Data = AD7799_GetRegisterValue(AD7799_REG_DATA,3);
			mvData = AD7799_ConvTo_mV(Data,VREF,GAIN,ADCN,uPOLAR);
			Temperature = -8.451576E-06 * pow(mvData,2) - 1.769281E-01 * pow(mvData,1) + 2.043937E+02;
			//mvData = (Data*2500.0)/(pow(2,24)*1);   	//unipolar
			//mvData = (Data/pow(2,24-1)-1)*2500.0/1; //bipolar
		}
	}
}

void SystemClock_Config(void){
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

        /** Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

        /** Initializes the CPU, AHB and APB busses clocks */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 50;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
        /** Initializes the CPU, AHB and APB busses clocks  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
		HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);		
}


void SysTick_Handler(void) 
{	
    HAL_IncTick();
}
