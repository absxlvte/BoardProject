#include "AD7799.h"
#include "UART.h"
#include "TMP119.h"
void SystemClock_Config(void);
void SysTick_Handler(void);
//avaliable defines: LMT70_WORK,TMP119_WORK
//#define LMT70_WORK
#define TMP119_WORK
struct sendData{
	uint32_t time;
	unsigned long z;
}typedef sendData;
union package{
	sendData data;
	long long send;
}typedef Pack;


int main(void){
	HAL_Init();
	SystemClock_Config();
	SPI_Init();
	UartInit();
	//InitButton();
	initI2C();
	InitDR();
	
	//uint8_t data[2] = {0xC0,0x00};
	//writeDataTMP119(ConfigReg,data);
	//HAL_I2C_Mem_Write(&hi2c1, SL_ADDR, ConfigReg, I2C_MEMADD_SIZE_8BIT, data, 2, 200);
	//uint8_t regData = 0;
	//uint8_t regAddress = DevIDreg;
	//HAL_I2C_Master_Transmit(&hi2c1, (0x48 << 1), &regAddress, 1,  10);
	//HAL_I2C_Master_Receive(&hi2c1, (0x48 << 1), &regData, 1,  10);
	//int8_t addr = I2C_Scan();
	
	#ifdef TMP119_WORK 
	TMP119_Init(modeCC,convCycle6,avg8);
	int16_t st = 0;
	st = HAL_I2C_IsDeviceReady(&hi2c1,SL_ADDR,1,HAL_MAX_DELAY) == HAL_OK ? 1 : -1;
	uint16_t ID = 0;
	ID = readDataTMP119(DevIDreg);
	int16_t mC, C;
	
	int8_t adrr = 0;
	adrr = I2C_Scan();
	//0x2117
	#endif
	#ifdef LMT70_WORK 
	uint8_t status = 0xFF;
	uint8_t rdy = 0;
	unsigned long Data = 0;
	double mvData = 0;
	double Temperature = 0;
	uint32_t time = 0;
	Pack tuple = {};
	AD7799_CS_HIGH;
	status = AD7799_Init();
	AD7799_SetMode(AD7799_MODE_CAL_INT_ZERO);
	HAL_Delay(10);
	AD7799_SetMode(AD7799_MODE_CONT); //set freq
	AD7799_SetChannel(AD7799_CH_AIN1P_AIN1M);
	AD7799_SetGain(AD7799_GAIN_2, AD7799_CONF_UNIPOLAR);
	#endif
	while(1){
		#ifdef TMP119_WORK 
		//ID = readDataTMP119(DevIDreg);
		
		#endif
		#ifdef LMT70_WORK
		rdy = AD7799_isDataReady();
		if (!rdy){
			Data = AD7799_GetRegisterValue(AD7799_REG_DATA,3);
			mvData = AD7799_ConvTo_mV(Data,VREF,GAIN,ADCN,uPOLAR);
			Temperature = -8.451576E-06 * pow(mvData,2) - 1.769281E-01 * pow(mvData,1) + 2.043937E+02;
			tuple.data.time = HAL_GetTick();
			tuple.data.z = Data;
			//if (isPressed) HAL_UART_Transmit(&xuart,&Data, 3, 100);
			HAL_UART_Transmit(&xuart,&Data, 3, 1);
			//HAL_UART_Transmit(&xuart,&tuple.send, 8, 100);
			}
		#endif
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
