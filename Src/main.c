/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "shell_port.h"
#include "u8g2.h"
#include "usbd_audio_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


#include "stdio.h"
#include <stdarg.h>
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#else
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif
PUTCHAR_PROTOTYPE
{
HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1 , 0xffff);
	//CDC_Transmit_FS((uint8_t *)&ch,1);
return ch;
}



uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr);
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
    void *arg_ptr);



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define OSA_BUF_SIZE 16      /* ADC oversampling buffer. Make sure to state the correct  
                                shift and offset amount in HAL_ADC_ConvCpltCallback() and
                                adjust the sampling frequency accordingly */
extern USBD_HandleTypeDef hUsbDeviceFS;
int16_t adc_buffer[OSA_BUF_SIZE * AUDIO_IN_PACKET / 2] = {0};


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

Shell shell;
uint8_t recv_buf = 0;
u8g2_t u8g2;
uint16_t temp_ADC_Value[128];
float adc_value ;

int count_adc_back;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	USBD_AUDIO_HandleTypeDef *haudio = hUsbDeviceFS.pClassData;
	int16_t *buf_part = haudio->in_buffer + (AUDIO_IN_PACKET / 2) * haudio->in_buffer_half;  // USB mic buffer access

	
	/* Oversample for +2 bits */
	for (uint16_t i = 0; i < (AUDIO_IN_PACKET / 2); i++) {
  	int32_t avg_value = 0;
		for (uint16_t j = 0; j < OSA_BUF_SIZE; j++) {
			avg_value += adc_buffer[OSA_BUF_SIZE * i + j];
		}
    // bit shift for signed variables is undefined behaviour
		// Don't forget the mic amp offset: 0-1706, 1-3413, 2-6826
		buf_part[i] = (avg_value / 4) - 6826;
		
		
		if(count_adc_back==0){
		temp_ADC_Value[i]=(buf_part[i]+6826);
		}
		else if(count_adc_back==1)
		{
		temp_ADC_Value[i+22]=(buf_part[i]+6826);
			
		}
			else if(count_adc_back==2)
			{
							temp_ADC_Value[i+44]=(buf_part[i]+6826);
	
			}
					else if(count_adc_back==3)
			{
							temp_ADC_Value[i+66]=(buf_part[i]+6826);
	
			}
					else if(count_adc_back==4)
			{
							temp_ADC_Value[i+88]=(buf_part[i]+6826);
	
			}
								else if(count_adc_back==5)
			{
							temp_ADC_Value[i+110]=(buf_part[i]+6826);
	
			}
											else if(count_adc_back==6)
			{
				if(i+110<128)
							temp_ADC_Value[i+110]=(buf_part[i]+6826);
	
			}
	}
	
	
	count_adc_back=count_adc_back+1;
		if(count_adc_back==7)
		{
	count_adc_back=0;
		}
	

	

		
}

void ADC_to_MIC(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, OSA_BUF_SIZE * (AUDIO_IN_PACKET / 2)); // Start ADC transfer into oversampling buffer
}



int u8g2_printf(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *fmt,
...)
{
va_list ap;
va_start(ap, fmt);
int length;
char buffer[128];
length = vsnprintf(buffer, 128, fmt, ap);
u8g2_DrawStr(u8g2, x, y, buffer);
va_end(ap);
return length;
}


void hello(void)
{
    printf("hello letter shell!\n");
}
 
//导出到命令列表里
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), hello, hello, test_hello);


uint8_t map(uint16_t _value)
{
    return 63 - ((_value * 3) >> 10);
}
 
void timer()
{
	   static uint8_t phase = 0;
    static uint16_t waveform[128];

	
	
	phase++;

	
    if (phase == 5)
    {
   adc_value=temp_ADC_Value[0]/16384.0f * 3.3f;
			      for (uint8_t i = 0; i < 128; ++i)
			{
				 	waveform[i]=(temp_ADC_Value[i]);
			}
			
	
        phase = 0;
			u8g2_FirstPage(&u8g2);
		do
		{
	
			u8g2_printf(&u8g2, 0, 8, "48KAD 100FPS V=%.2fV",adc_value);
			
			
        for (uint8_t i = 1; i != 128; ++i)
			{
					
			
            u8g2_DrawLine(&u8g2, i - 1, map(waveform[i - 1]),i, map(waveform[i]));
			}
			
			
			} while (u8g2_NextPage(&u8g2));

    }
    
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start(&htim3);
	//HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1); //启定时器PWM模式
	//HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&temp_ADC_Value, 1);


		//使能串口中断接收
	User_Shell_Init();
	HAL_UART_Receive_IT(&huart1,(uint8_t*)&recv_buf,1);


	u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
			u8g2_SetFont(&u8g2, u8g2_font_courR08_tf);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
		
//		u8g2_FirstPage(&u8g2);
//		do
//		{
//			u8g2_SetFont(&u8g2, u8g2_font_courR18_tf);
//			u8g2_DrawStr(&u8g2, 20, 20, "CAST");
//			

//			
//		} while (u8g2_NextPage(&u8g2));

				HAL_Delay(200);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_GPIO_AND_DELAY_INIT:
    HAL_Delay(1);
    break;
  case U8X8_MSG_DELAY_MILLI:
    HAL_Delay(arg_int);
    break;
  case U8X8_MSG_GPIO_DC:
    HAL_GPIO_WritePin(OLED_DS_GPIO_Port, OLED_DS_Pin, arg_int);
    break;
  case U8X8_MSG_GPIO_RESET:
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, arg_int);
    break;
  }
  return 1;
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
    void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_BYTE_SEND:
    HAL_SPI_Transmit(&hspi1, (uint8_t *) arg_ptr, arg_int, HAL_MAX_DELAY);
    break;
  case U8X8_MSG_BYTE_INIT:
    break;
  case U8X8_MSG_BYTE_SET_DC:
    HAL_GPIO_WritePin(OLED_DS_GPIO_Port, OLED_DS_Pin, arg_int);
    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
		HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
		HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
    break;
  default:
    return 0;
  }
  return 1;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* 判断是哪个串口触发的中断 */
    if(huart ->Instance == USART1)
    {
        //调用shell处理数据
			  shellHandler(&shell,recv_buf);
        //使能串口中断接收
			  HAL_UART_Receive_IT(&huart1, (uint8_t*)&recv_buf, 1);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  
    if (htim == (&htim2))
    {
			
			//printf("begin=%f v\n",temp_ADC_Value/4096.0f*3.3);
			
	 // HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    timer();
			
    }
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
