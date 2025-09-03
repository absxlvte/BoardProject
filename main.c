#include "AD7714.h"
//#include "TMP119.h"
//#include "M1601Z.h"
//#include "OneWire.h"
void SystemClock_Config(void);
void SysTick_Handler(void);

int main(void){
	HAL_Init();
	SystemClock_Config();
	//UartInit();
	//I2C_Init();
	//TMP119_Init();
	//id = TMP119_ReadDeviceID(); //0x2117
	//res = TMP119_ReadTemperature();
//	uint8_t data=0;
//	SPI_Init();
//	AD_Init();
//	AD_ReadRegister(MFilter,AIN3_AIN4,&data,1);
	//uint32_t value = AD_ReadData(AIN1_AIN2);
  SPI_Init();
	uint8_t filter_high_conf = 0x27; 											//0b 0 010 0 111
	uint8_t filter_high_val = 0x4F;  											//0b 0 100 1 111
	HAL_SPI_Transmit(&xSPI, &filter_high_conf, 1, 200);
	HAL_SPI_Transmit(&xSPI, &filter_high_val, 1, 200);
	uint8_t filter_low_conf = 0x37; 											//0b 0 011 0 111
	uint8_t filter_low_val = 0xA0; 												//0b 1 010 0 000
	HAL_SPI_Transmit(&xSPI, &filter_low_conf, 1, 200);
	HAL_SPI_Transmit(&xSPI, &filter_low_val, 1, 200);
	uint8_t mode_conf = 0x17; 														//0b 0 001 0 111
	uint8_t mode_val = 0x20; 															//0b 0 010 0 000
	HAL_SPI_Transmit(&xSPI, &mode_conf, 1, 200);
	HAL_SPI_Transmit(&xSPI, &mode_val, 1, 200);
	uint8_t read_conf = 0x5F; 														//0b 0 101 1 111
	uint16_t buffer = 0;
	while(1){
		if (HAL_GPIO_ReadPin(GPIOA, AD_DRDY) == GPIO_PIN_RESET){
			HAL_SPI_Transmit(&xSPI, &read_conf, 1, 200);
			HAL_SPI_Receive(&xSPI, &read_conf, 1, 200); //how many bytes read>?
		}
		
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}


void SysTick_Handler(void) 
{	
    HAL_IncTick();
}
