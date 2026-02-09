/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/*Changes at the time of Generate code
 * In Main. c 	:	- Disable void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 * 					- Disable MX_NVIC_Init() before USER CODE BEGIN 2
 * 					- In void StartTCPTask(void *argument) before LWIP_Init
 * 					- Disable NVIC_Init in main initialization
 * 					- In NVIC_Init, Disable HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
 * 					- Disable Following Portion
 * 					  Configure GPIO pin : ENCODER_Z_Pin
						GPIO_InitStruct.Pin = ENCODER_Z_Pin;
						GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
						GPIO_InitStruct.Pull = GPIO_NOPULL;
						HAL_GPIO_Init(ENCODER_Z_GPIO_Port, &GPIO_InitStruct);
 * In lwip.c	:	- Inside MX_LWIP_Init, After netif_set_default(&gnetif);
 * In ethernetif.c	- Inside low_level_init function
 * 					  //  MACAddr[5] = 0x00;
						  MACAddr[5] = Temp_Mac_Address;
* In stm32h7xx_it.c


 */

//ERR_CLSD tcp_poll

//APB1 => 138 MHZ MAX, 125 MHZ Used => TIMER 02, 03, 04, 05, 06, 07, 12, 13, 14, 23, 24
//APB2 => 138 MHZ MAX, 125 MHZ Used => TIMER	01, 08, 15, 16, 17
//APB3 => 138 MHZ MAX, 125 MHZ Used
//APB4 => 138 MHZ MAX, 125 MHZ Used

//AHB1 => 275 MHZ MAX, 250 MHZ Used
//AHB2 => 275 MHZ MAX, 250 MHZ Used
//AHB3 => 275 MHZ MAX, 250 MHZ Used
//AHB4 => 275 MHZ MAX, 250 MHZ Used

//ADC 		: MHZ
//TIMERS => 275 MHZ MAX, 250 MHZ Used

//TIMER 1 : 5mS
//TIMER 2 : 1 uS Pulse Period, for Speed Calculation
//TIMER 3 : ENCODER
//TIMER 15, 16 : PULSE GENERATION  return (struct mem *)(void *)&ram[ptr] while (cur->used && cur != ram_end)
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* ETH_CODE: add lwiperf, see comment in StartDefaultTask function */
#include "lwip/apps/lwiperf.h"

#include "string.h"
#include "ARCSS_variables.h"
#include "variables_BLE.h"
#include "flash.h"
#include "functions.h"
#include "BLE.h"
#include "GEN_functions.h"
#include "slave_lcd.h"
#include "Correction.h"
#include "ARCSS_functions.h"
#include "tcpClientRAW.h"

#include "tftp_client.h"
#include "flash_if.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern struct netif gnetif;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc3;

DAC_HandleTypeDef hdac1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim15;
TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart3;

/* Definitions for TCPTask */
osThreadId_t TCPTaskHandle;
const osThreadAttr_t TCPTask_attributes = {
  .name = "TCPTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ARCTask */
osThreadId_t ARCTaskHandle;
const osThreadAttr_t ARCTask_attributes = {
  .name = "ARCTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TCP_Write */
osMessageQueueId_t TCP_WriteHandle;
const osMessageQueueAttr_t TCP_Write_attributes = {
  .name = "TCP_Write"
};
/* Definitions for TCP_Read */
osMessageQueueId_t TCP_ReadHandle;
const osMessageQueueAttr_t TCP_Read_attributes = {
  .name = "TCP_Read"
};
/* USER CODE BEGIN PV */
char Debug_String[200] = "Welcome to Alteem\n";
char TCP_Debug_String[100] = "Welcome to Alteem\n";
_Bool Flag_TCP_Debug = 0;

_Bool TFTP_CON = 0;
int tsize = 0;
_Bool Flag_OTA_File_Receive_Completed = 0;
_Bool Flag_TFTP_Error = 0;
_Bool oack_received = 0;

#define TFTP_Timer_Limit 30

//extern void tcp_client_init(void);
extern int tcp_client_init(void);
//extern void tcp_Close_Connection(void);
extern int tcp_Close_Connection(void);

/*SAHAJ =======================================*/
_Bool LWIP_FLAG = 0;

uint32_t TCP_Time_Update = 0;

ERROR_HANDLER err_handler;
Program_Counter err_pc = {0};

#define TCP_WriteQueueTimeCount_Limit	50
#define alt_tcp_ack_time_sec_limit		20

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
static void MX_UART4_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM15_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM16_Init(void);
static void MX_DAC1_Init(void);
static void MX_TIM6_Init(void);
static void MX_ADC3_Init(void);
void StartTCPTask(void *argument);
void StartARCTask(void *argument);

static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
uint8_t get_reset_cause(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//PUTCHAR_PROTOTYPE
uint8_t get_reset_cause(void)
{
	uint8_t reset_cause = 0;

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWR1RST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: \n");
    	reset_cause =  RESET_D1_STANDBY;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDG1RST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: WINDOW WATCHDOG REST\n");
    	reset_cause =  RESET_WINDOW_WATCHDOG;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: IND. WATCHDOG RESET\n");
    	reset_cause = RESET_INDEPENDENT_WATCHDOG;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: SYSTEM RESET\n");
    	reset_cause = RESET_SYSTEM;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: POWER ON\n");
    	reset_cause = RESET_POWER_ON;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: PIN RESET\n");
    	reset_cause = RESET_PIN;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: BORWN OUT\n");
    	reset_cause = RESET_BROWNOUT;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_D2RST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: D2 POWER ON\n");
    	reset_cause = RESET_D2_POWER_ON;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_D1RST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: D1 POWER ON\n");
    	reset_cause = RESET_D1_POWER_ON;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_CPURST))
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: CPU RESET\n");
    	reset_cause = RESET_CPU;
    }
    else
    {
    	Send_String_UART_232((uint8_t *)"\rRESET CAUSE: UNKNOWN\n");
    	reset_cause = RESET_UNKNOWN;
    }
    __HAL_RCC_CLEAR_RESET_FLAGS();
    return reset_cause;
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	arc_execution_int_prev_id = arc_execution_func_cur_id;
	arc_execution_int_id = 15;
	if(hadc->Instance == ADC3)
	{
		if(bl_count <= 1000000)
		{
			for(uint16_t i = 0; i < bl_buffer_size_half; i++)
			{
				bl_sum = bl_sum + bl_buffer[i];
				bl_count++;
			}
		}
	}
	else
	{
		Send_String_UART_232((uint8_t *)"Half 1\n");
	}
	arc_execution_int_id = 16;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	arc_execution_int_prev_id = arc_execution_func_cur_id;
	arc_execution_int_id = 17;
	if(hadc->Instance == ADC3)
	{
		if(bl_count <= 1000000)
		{
			for(uint16_t i = bl_buffer_size_half; i < bl_buffer_size; i++)
			{
				bl_sum = bl_sum + bl_buffer[i];
				bl_count++;
			}
		}
	}
	else
	{
		Send_String_UART_232((uint8_t *)"Full 1\n");
	}
	arc_execution_int_id = 18;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	char temp_uart_str[100];
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_UART4_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM15_Init();
  MX_USART3_UART_Init();
  MX_TIM16_Init();
  MX_DAC1_Init();
  MX_TIM6_Init();
  MX_ADC3_Init();

  /* Initialize interrupts */
//  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  Reset_Cause = get_reset_cause();

  sprintf(temp_uart_str, "DEVICE RESET: %d\n", Reset_Cause);
  Send_String_UART_232((uint8_t *)temp_uart_str);

  //Read Program counter and fault
  Flash_Read_Buffer(Flash_Address_PC, (uint8_t*)&err_pc, sizeof(err_pc));
  sprintf(temp_uart_str, "EEPROM --> PC: 0x%08X \t ERROR HANDLER: %d, Size : %d\n", err_pc.PC, err_pc.FAULT,sizeof(err_pc));
  Send_String_UART_232((uint8_t *)temp_uart_str);

//  //Clear memory once values read into err_pc
//  Program_Counter temp_pc;
//  temp_pc.FAULT = 0;
//  temp_pc.PC = 0;
//  Flash_Write_Buffer(Flash_Address_PC, (uint8_t*)&temp_pc, sizeof(temp_pc));
//  HAL_Delay(10);
//
//  Flash_Read_Buffer(Flash_Address_PC, (uint8_t*)&err_pc, sizeof(err_pc));
//  sprintf(temp_uart_str, "EEPROM --> PC: 0x%08X \t ERROR HANDLER: %d\n", err_pc.PC, err_pc.FAULT);
//  Send_String_UART_232((uint8_t *)temp_uart_str);

  //Read Auto Horz and Vert Status
  Auto_HV_Status = Flash_Read_Single_Byte(Flash_Address_Auto);
  sprintf(temp_uart_str, "Auto HV Status : %d\n", Auto_HV_Status);
  Send_String_UART_232((uint8_t *)temp_uart_str);
  if(Auto_HV_Status > 3)
  {
	  Auto_HV_Status = 0;
	  Send_String_UART_232((uint8_t *)"Auto HV Status False Value, Reset to Zero\n");
  }
  Flash_Write_Single_Byte(Flash_Address_Auto, 0);	//Clear Memory once Auto Status read into Variable
  HAL_Delay(10);
  Send_String_UART_232((uint8_t *)"ALTEEM INSTRUMENTS\n");

  HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);	//Calibrate ADC1
  HAL_ADC_Start(&hadc1);													//Start ADC1 without DMA

  HAL_Delay(10);

  HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);	//Calibrate ADC3
  __HAL_DMA_ENABLE_IT(&hdma_adc3, DMA_IT_HT);  // Half Transfer				//Enable Half Conversion Complete Interrupt
  __HAL_DMA_ENABLE_IT(&hdma_adc3, DMA_IT_TC);  // Full Transfer				//Enable Full Conversion Complete Interrupt
  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)bl_buffer, bl_buffer_size);			//Start ADC3 DMA for bl_buffer_size

  //To Generate HardFault for Testing
//  volatile int *p = (int *)0xFFFFFFFF; // invalid address
//  int val = *p;  // This will cause a fault

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of TCP_Write */
  TCP_WriteHandle = osMessageQueueNew (50, 6, &TCP_Write_attributes);

  /* creation of TCP_Read */
  TCP_ReadHandle = osMessageQueueNew (50, 6, &TCP_Read_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TCPTask */
  TCPTaskHandle = osThreadNew(StartTCPTask, NULL, &TCPTask_attributes);

  /* creation of ARCTask */
  ARCTaskHandle = osThreadNew(StartARCTask, NULL, &ARCTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  	  volatile int *p = (int *)0xFFFFFFFF; // invalid address
//	  	  int val = *p;  // This will cause a fault
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 125;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV16;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 4;
  PeriphClkInitStruct.PLL2.PLL2N = 28;
  PeriphClkInitStruct.PLL2.PLL2P = 8;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI9_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 7, 0);
//  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* TIM1_UP_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_UP_IRQn, 12, 0);
  HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
  /* ADC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(ADC_IRQn);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;									//ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv 		= ADC_EXTERNALTRIG_T6_TRGO;				//ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_RISING;		//ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.Oversampling.Ratio = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analog WatchDog 1
  */
  AnalogWDGConfig.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
  AnalogWDGConfig.Channel = ADC_CHANNEL_2;
  AnalogWDGConfig.ITMode = ENABLE;
  AnalogWDGConfig.HighThreshold = 4095;
  AnalogWDGConfig.LowThreshold = 0;
  if (HAL_ADC_AnalogWDGConfig(&hadc1, &AnalogWDGConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_32CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.DataAlign = ADC3_DATAALIGN_RIGHT;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T6_TRGO;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
  hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
  hadc3.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc3.Init.OversamplingMode = DISABLE;
  hadc3.Init.Oversampling.Ratio = ADC3_OVERSAMPLING_RATIO_2;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC3_SAMPLETIME_47CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSign = ADC3_OFFSET_SIGN_NEGATIVE;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */
  HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);
  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x60202334;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 24999;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 49;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 249;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 25-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 50-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 24999;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 499;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 24999;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 499;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, DO_1_Pin|DO_2_Pin|DO_3_Pin|DO_4_Pin
                          |DO_5_Pin|DO_6_Pin|DO_7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_GREEN_Pin|DETECT_Z_OP_Pin|LED_RED_Pin|DETECT_TIM3_OP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_SW_Pin */
  GPIO_InitStruct.Pin = USER_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_SW_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DO_1_Pin DO_2_Pin DO_3_Pin DO_4_Pin
                           DO_5_Pin DO_6_Pin DO_7_Pin */
  GPIO_InitStruct.Pin = DO_1_Pin|DO_2_Pin|DO_3_Pin|DO_4_Pin
                          |DO_5_Pin|DO_6_Pin|DO_7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_GREEN_Pin DETECT_Z_OP_Pin LED_RED_Pin DETECT_TIM3_OP_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin|DETECT_Z_OP_Pin|LED_RED_Pin|DETECT_TIM3_OP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MODE_PP_ALTIMATCH_Pin */
  GPIO_InitStruct.Pin = MODE_PP_ALTIMATCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(MODE_PP_ALTIMATCH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ENCODER_Z_Pin */
//  GPIO_InitStruct.Pin = ENCODER_Z_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(ENCODER_Z_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_YELLOW_Pin */
  GPIO_InitStruct.Pin = LED_YELLOW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_YELLOW_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
__attribute__((naked)) void Error_Handler(void) {
    __asm volatile (
    		" movs r0, #4 \n"        	// Load immediate value 4 into r0, updating flags
    		" movs r1, lr \n"        	// Move the LR value into r1, updating flags
    		" tst r0, r1 \n"         	// Bitwise AND of r0 and r1, discarding result but setting flags
    		" beq _MSP_6 \n"           	// Branch to _MSP if the result of tst was zero
    		" mrs r0, psp \n"        	// If not zero, move PSP (Process Stack Pointer) into r0
    		" b _HALT_6 \n"            	// Unconditional branch to _HALT
    		"_MSP_6: \n"               	// Label for when MSP is used
    		" mrs r0, msp \n"        	// Move MSP (Main Stack Pointer) into r0
    		"_HALT_6: \n"              	// Label to continue execution
    		"mov r1, #7 \n"				// Load 7 into r1, this will passed in the 2nd argument of fault_handler
    		" b fault_handler \n" 		// Branch to the C function
    );
}

void Process_TCP_Received_Data(uint8_t *temp, uint16_t temp_length)
{
    uint16_t temp_position = 0;
    uint16_t bytes_remaining,i;
    unsigned char temp_cmd;
	unsigned char temp_name_length;

    bytes_remaining = temp_length;

    if((temp[0] != Config_Parameters[Alt_Station_ID]) && (temp[0] != 0xAA))
    {
    	Send_String_UART_232((uint8_t *)"TCP_Rec : Invalid First Byte\n");
    	return;
    }

    while(1)
    {
    	temp_cmd = temp[temp_position + 1];
    	switch(temp_cmd)
    	{

    	case 0x10 								:	//BLE_Command_Request_Station_ID	- Length 4
    											if(bytes_remaining < 4)
    											{
    												temp_position = temp_length;	//way to comeout from while loop
    												break;
    											}

    											BLE_Received_Data_Length[Flag_Slave_DMA_Buffer_Write] = 4;
												for(i = 0; i < 4; i++)
												{
													Slave_receive_buff[Flag_Slave_DMA_Buffer_Write][i] = temp[temp_position + i];
												}
												temp_position += 4;
												bytes_remaining -= 4;
												Flag_Slave_DMA_Buffer_Write++;
												if(Flag_Slave_DMA_Buffer_Write >= Max_Count_Slave_DMA_Buffer_Write_Read)
												{
													Flag_Slave_DMA_Buffer_Write = 0;
												}
												break;

    	case 0x11 								:	//BLE_Command_Buzzer_Control 		- Length 5
    	case 0x19 								:	//BLE_Command_Enable_Disable 		- Length 5
    	case 0x1A 								:   //BLE_Command_Read_Input_Reg_Group	- Length 5
												if(bytes_remaining < 5)
												{
													temp_position = temp_length;	//way to comeout from while loop
													break;
												}
												BLE_Received_Data_Length[Flag_Slave_DMA_Buffer_Write] = 5;
												for(i = 0; i < 5; i++)
												{
													Slave_receive_buff[Flag_Slave_DMA_Buffer_Write][i] = temp[temp_position + i];
												}
												temp_position += 5;
												bytes_remaining -= 5;
												Flag_Slave_DMA_Buffer_Write++;
												if(Flag_Slave_DMA_Buffer_Write >= Max_Count_Slave_DMA_Buffer_Write_Read)
												{
													Flag_Slave_DMA_Buffer_Write = 0;
												}
												break;

    	case 0x12 								:	//BLE_Command_Read_Config_Reg			- Length - 8
    	case 0x13								:	//BLE_Command_Write_Single_Key			- Length - 8
    	case 0x14								:	//BLE_Command_Write_Single_Config16_Reg	- Length - 8
												if(bytes_remaining < 8)
												{
													temp_position = temp_length;	//way to comeout from while loop
													break;
												}
    											BLE_Received_Data_Length[Flag_Slave_DMA_Buffer_Write] = 8;
												for(i = 0; i < 8; i++)
												{
													Slave_receive_buff[Flag_Slave_DMA_Buffer_Write][i] = temp[temp_position + i];
												}
												temp_position += 8;
												bytes_remaining -= 8;
												Flag_Slave_DMA_Buffer_Write++;
												if(Flag_Slave_DMA_Buffer_Write >= Max_Count_Slave_DMA_Buffer_Write_Read)
												{
													Flag_Slave_DMA_Buffer_Write = 0;
												}
												break;

    	case 0x15 								:	//BLE_Command_Write_Single_Config32_Reg	- Length - 10
												if(bytes_remaining < 10)
												{
													temp_position = temp_length;	//way to comeout from while loop
													break;
												}
    											BLE_Received_Data_Length[Flag_Slave_DMA_Buffer_Write] = 10;
												for(i = 0; i < 10; i++)
												{
													Slave_receive_buff[Flag_Slave_DMA_Buffer_Write][i] = temp[temp_position + i];
												}
												temp_position += 10;
												bytes_remaining -= 10;
												Flag_Slave_DMA_Buffer_Write++;
												if(Flag_Slave_DMA_Buffer_Write >= Max_Count_Slave_DMA_Buffer_Write_Read)
												{
													Flag_Slave_DMA_Buffer_Write = 0;
												}
												break;

		case 0x16								:	//BLE_Command_Write_Job_Name
												//3rd byte length shows byte length from 4th byte i.e string + CRC
												temp_name_length = temp[temp_position + 2];	//station id, cmd, length
												temp_name_length = temp_name_length + 3;	//3 init byte

												if(bytes_remaining < temp_name_length)
												{
													temp_position = temp_length;	//way to comeout from while loop
													break;
												}

												for(i = 0; i < temp_name_length; i++)
												{
													Temp_Job_Name[i] = temp[temp_position + i];
												}
												temp_position += temp_name_length;
												bytes_remaining -= temp_name_length;

												//Store Dummy Data
												BLE_Received_Data_Length[Flag_Slave_DMA_Buffer_Write] = 3;	//Dummy Length
												for(i = 0; i < 3; i++)
												{
													Slave_receive_buff[Flag_Slave_DMA_Buffer_Write][i] = Temp_Job_Name[i];
												}
												Flag_Slave_DMA_Buffer_Write++;
												if(Flag_Slave_DMA_Buffer_Write >= Max_Count_Slave_DMA_Buffer_Write_Read)
												{
													Flag_Slave_DMA_Buffer_Write = 0;
												}
												break;

		case 0x17								:	//BLE_Command_Request_OTA
												//3rd byte length shows byte length from 4th byte i.e string + CRC
												temp_name_length = temp[temp_position + 2];	//0xAA, cmd, length
												temp_name_length = temp_name_length + 3;	//including 3 init byte, i.e. total length

												if(bytes_remaining < temp_name_length)
												{
													temp_position = temp_length;	//way to comeout from while loop
													break;
												}

												for(i = 0; i < temp_name_length; i++)
												{
													Temp_File_Name[i] = temp[temp_position + i];
												}
												temp_position += temp_name_length;
												bytes_remaining -= temp_name_length;

												//Store Dummy Data
												BLE_Received_Data_Length[Flag_Slave_DMA_Buffer_Write] = 3;	//Dummy Length
												for(i = 0; i < 3; i++)
												{
													Slave_receive_buff[Flag_Slave_DMA_Buffer_Write][i] = Temp_File_Name[i];
												}
												Flag_Slave_DMA_Buffer_Write++;
												if(Flag_Slave_DMA_Buffer_Write >= Max_Count_Slave_DMA_Buffer_Write_Read)
												{
													Flag_Slave_DMA_Buffer_Write = 0;
												}
												break;

    	default									:
    											temp_position = temp_length;	//Just to make condition outside(below) switch true,
    																			//so that while look break
    											break;
    	}

    	if(temp_position >= temp_length)
    		break;
    }
}

void Encoder_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : ENCODER_Z_Pin */
  GPIO_InitStruct.Pin = ENCODER_Z_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ENCODER_Z_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartTCPTask */
/**
  * @brief  Function implementing the TCPTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTCPTask */
void StartTCPTask(void *argument)
{
  /* init code for LWIP */
//  MX_LWIP_Init();
  /* USER CODE BEGIN 5 */

	while(Flag_Memory_Read_Done == 0)	//Wait for Memory Read Done, to get IP address from memory
	{
		osDelay(1);
	}

    if(!LWIP_FLAG)
    {
  	  LWIP_FLAG = 1;
  	  MX_LWIP_Init();
    }

//	while(Flag_Memory_Read_Done == 0)	//Wait for Memory Read Done, to get IP address from memory
//	{
//		osDelay(1);
//	}

 	uint16_t Counter = 0;
 	uint16_t Reconnect_Time_Counter = TCP_Try_For_Reconnect_Time_ms;
 	Reconnect_Time_Counter = Reconnect_Time_Counter - 5000;

    _Bool Flag_Prev_TCP_Connected = Flag_TCP_Connected;
    _Bool Flag_Temp_TCP_Connected;

    _Bool Flag_Prev_ETH_Link_Status	= Flag_ETH_Link_Status;
    _Bool Flag_Temp_ETH_Link_Status;

    uint8_t TFTP_Timer = 0;

    int Err_Code = 0;

    tcp_queue *msgw;
    uint32_t Get_Count = 0;
    uint16_t TCP_WriteQueueTimeCount = 0;

    _Bool Flag_TCP_Init = 0;
    _Bool Flag_OTA_Close_TCP_Connection = 0;

    uint16_t Prev_BLE_Reply_Index = 0;
    uint8_t alt_tcp_ack_time_sec = 0;
	uint8_t Temp_Retry_Display_Duration = 0;
	_Bool Flag_Prev_TCP_Reconnect = 0;

    for(;;)
    {
    	if(Flag_OTA_Request_Received == 0)
    	{
		 if(Flag_TCP_Debug != 0)
		 {
			  HAL_UART_Transmit(&huart3, (uint8_t *)TCP_Debug_String, strlen(TCP_Debug_String), 100);
			  Flag_TCP_Debug = 0;
		 }

		 //Get Live Status of TCP and Link Flags, if they are changed
		 Flag_Temp_TCP_Connected 	= Flag_TCP_Connected;
		 Flag_Temp_ETH_Link_Status 	= Flag_ETH_Link_Status;

		  Counter++;
		  if(Counter >= 1000)	//1 Second Process
		  {
			  Counter = 0;
			  HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin);

			  TCP_Time_Update = HAL_GetTick();

			  if(Flag_Temp_TCP_Connected != 0)
			  {
				  if((Flag_Prev_TCP_Reconnect != 0) && (Flag_TCP_Reconnect == 0))
				  {
					  TCP_Reconnect_Cause = TCP_Reconnect_Cause_Unknown;
				  }
				  Flag_Prev_TCP_Reconnect = Flag_TCP_Reconnect;

				  //Check Data Sent Confirmation and based on it, disconnect @ 20 seconds
				  if(Prev_BLE_Reply_Index == BLE_Reply_Index)		//Ack not received
				  {
					alt_tcp_ack_time_sec++;
					if(alt_tcp_ack_time_sec >= alt_tcp_ack_time_sec_limit)
					{
						alt_tcp_ack_time_sec = 0;

						Send_String_UART_232((uint8_t *)"TCP Close..");

						LOCK_TCPIP_CORE();
						Err_Code = tcp_Close_Connection();
						UNLOCK_TCPIP_CORE();

						sprintf(TCP_Debug_String,"..Ack Missing - Err : %d, TCP : %d\n",Err_Code, Flag_TCP_Connected);
						Send_String_UART_232((uint8_t *)TCP_Debug_String);

						sprintf(TCP_Debug_String,"ms:%d, p02:%d, p01:%d, p:%d, c:%d, itp:%d, it:%d\n",
								arc_execution_main_stage,
								arc_execution_func_prev_id_02,
								arc_execution_func_prev_id_01,
								arc_execution_func_prev_id,
								arc_execution_func_cur_id,
								arc_execution_int_prev_id,
								arc_execution_int_id
								);
						Send_String_UART_232((uint8_t *)TCP_Debug_String);

						TCP_Reconnect_Cause = TCP_Reconnect_Cause_Ack_Missing;
						TCP_Dis_Ack_Missing_Count++;

						Flag_TCP_Init = 0;
					}
				  }
				  else	//Ack received, so restart time variable
				  {
					alt_tcp_ack_time_sec = 0;
				  }
				  Prev_BLE_Reply_Index = BLE_Reply_Index;
			  }

				if(Flag_TCP_Retry != 0)
				{
					Temp_Retry_Display_Duration++;
					if(Temp_Retry_Display_Duration >= 3)
					{
						Temp_Retry_Display_Duration = 0;
						Flag_TCP_Retry = 0;
					}
				}
		  }

		 //Flag_TCP_Connected = 0 ,means client connection closed from tcpClientRaw file
		 //So do not need to close connection in that case.
		 //For other cases like link and other cases, we need to close connection

		 //Check Queue @ 50 ms, if available send data to TCP
		 if(Flag_Temp_TCP_Connected)
		 {
			 TCP_WriteQueueTimeCount++;
			 if(TCP_WriteQueueTimeCount >= TCP_WriteQueueTimeCount_Limit)
			 {
				 TCP_WriteQueueTimeCount = 0;
				 int status = osMessageQueueGet(TCP_WriteHandle, &msgw, NULL, 0);
				 if(status == osOK)
				 {
					 Send_New_Data(msgw->data,msgw->length);
					 vPortFree(msgw->data);
					 vPortFree(msgw);
					 Get_Count++;
				 }
				 else
				 {

				 }
			 }
		 }

		 if(Flag_Temp_TCP_Connected != Flag_Prev_TCP_Connected)	//TCP Status Changed
		 {
			if(Flag_Temp_TCP_Connected == 0)
			{
				Send_String_UART_232((uint8_t *)"TCP DISCONNECTED\n");

				sprintf(TCP_Debug_String,"ms:%d, p02:%d, p01:%d, p:%d, c:%d, itp:%d, it:%d\n",
						arc_execution_main_stage,
						arc_execution_func_prev_id_02,
						arc_execution_func_prev_id_01,
						arc_execution_func_prev_id,
						arc_execution_func_cur_id,
						arc_execution_int_prev_id,
						arc_execution_int_id
						);
				Send_String_UART_232((uint8_t *)TCP_Debug_String);

				//TCP DISCONNECTED, so load reconnect timer such that after 5 seconds it try for reconnection
				Reconnect_Time_Counter =  TCP_Try_For_Reconnect_Time_ms;
				Reconnect_Time_Counter = Reconnect_Time_Counter - 5000;

				//Will try atleast for one time for TCP connection even if Flag_Use_With_PC is OFF
				Flag_First_Time_TCP_Request_Done = 0;
				TCP_Dis_Total_Count++;
			}
			else
			{
				Send_String_UART_232((uint8_t *)"TCP CONNECTED\n");

				sprintf(TCP_Debug_String,"ms:%d, p02:%d, p01:%d, p:%d, c:%d, itp:%d, it:%d\n",
						arc_execution_main_stage,
						arc_execution_func_prev_id_02,
						arc_execution_func_prev_id_01,
						arc_execution_func_prev_id,
						arc_execution_func_cur_id,
						arc_execution_int_prev_id,
						arc_execution_int_id
						);
				Send_String_UART_232((uint8_t *)TCP_Debug_String);
			}

			//Hooter OFF whenever TCP gets Reconnected or Disconnected
			HAL_GPIO_WritePin(DO_5_GPIO_Port, DO_5_Pin,Pulse_Level_Idle);
			HAL_GPIO_WritePin(DO_6_GPIO_Port, DO_6_Pin,Pulse_Level_Idle);

			alt_tcp_ack_time_sec = 0;
			BLE_Reply_Index = 0;
		 }

		 //Link Connection Changed
		 if(Flag_Temp_ETH_Link_Status != Flag_Prev_ETH_Link_Status)
		 {
			 if(Flag_Temp_ETH_Link_Status != 0)	//Link Reconnection
			 {
				 Send_String_UART_232((uint8_t *)"LINK RECONNECTED\n");

				 Reconnect_Time_Counter =  TCP_Try_For_Reconnect_Time_ms;
				 Reconnect_Time_Counter = Reconnect_Time_Counter - 5000;
			 }
			 else	//Link Disconnection, so close tcp connection if it was initiated
			 {
				TCP_Dis_Link_Count++;
				if(Flag_TCP_Init != 0)
				{
					Send_String_UART_232((uint8_t *)"LINK DISCON..");

					LOCK_TCPIP_CORE();
					Err_Code = tcp_Close_Connection();
					UNLOCK_TCPIP_CORE();

					sprintf(TCP_Debug_String,"..TCP Close - Err : %d, TCP : %d\n",Err_Code, Flag_TCP_Connected);
					Send_String_UART_232((uint8_t *)TCP_Debug_String);

					if(Flag_Temp_TCP_Connected != 0)
					{
						//TCP was connected at the time of Link Disconnection, then only cause is Link
						TCP_Reconnect_Cause = TCP_Reconnect_Cause_Link_Disconnect;
					}

					Flag_TCP_Init = 0;
				}
				else
				{
					Send_String_UART_232((uint8_t *)"LINK DISCON.. TCP_Init not initiated yet\n");
				}
			 }
		 }

		 //Link is Connected, But TCP is not Connected, So try for Reconnection at certain interval
		 if((Flag_Temp_ETH_Link_Status != 0) && (Flag_Temp_TCP_Connected == 0))
		 {
			 Reconnect_Time_Counter++;
			 if(Reconnect_Time_Counter >= TCP_Try_For_Reconnect_Time_ms)
			 {
				Reconnect_Time_Counter = 0;

				if((Flag_Use_With_PC != 0) || (Flag_First_Time_TCP_Request_Done == 0))
				{
					if(Flag_TCP_Init != 0)
					{
						Send_String_UART_232((uint8_t *)"TCP Close..");

						LOCK_TCPIP_CORE();
						Err_Code = tcp_Close_Connection();
						UNLOCK_TCPIP_CORE();

						sprintf(TCP_Debug_String,"..Before Init - Err : %d, TCP : %d\n",Err_Code, Flag_TCP_Connected);
						Send_String_UART_232((uint8_t *)TCP_Debug_String);

						Flag_TCP_Init = 0;
					}

					Send_String_UART_232((uint8_t *)"TCP Init..");

					LOCK_TCPIP_CORE();
					int temp_init = tcp_client_init();
					UNLOCK_TCPIP_CORE();

					sprintf(TCP_Debug_String,".. Ret Value : %d\n",temp_init);
					Send_String_UART_232((uint8_t *)TCP_Debug_String);

					sprintf(TCP_Debug_String,"ms:%d, p02:%d, p01:%d, p:%d, c:%d, itp:%d, it:%d\n",
							arc_execution_main_stage,
							arc_execution_func_prev_id_02,
							arc_execution_func_prev_id_01,
							arc_execution_func_prev_id,
							arc_execution_func_cur_id,
							arc_execution_int_prev_id,
							arc_execution_int_id
							);
					Send_String_UART_232((uint8_t *)TCP_Debug_String);

					Flag_First_Time_TCP_Request_Done = 1;
					Flag_TCP_Retry = 1;

					if(temp_init == 0)
					{
						Flag_TCP_Init = 1;
					}
				}
			 }
		 }

//======================================================================

		 Flag_Prev_TCP_Connected 	= Flag_Temp_TCP_Connected;
		 Flag_Prev_ETH_Link_Status	= Flag_Temp_ETH_Link_Status;
		 osDelay(1);
    	}
    	else
    	{
    		if(Flag_OTA_Close_TCP_Connection == 0)	//One Time only, When Comes from TCP to TFTP
    		{
				LOCK_TCPIP_CORE();
				Err_Code = tcp_Close_Connection();
				UNLOCK_TCPIP_CORE();

				sprintf(TCP_Debug_String,"TCP Close Called @ TFTP-OTA - Err : %d, TCP : %d\n",Err_Code, Flag_TCP_Connected);
				Send_String_UART_232((uint8_t *)TCP_Debug_String);

				Flag_OTA_Close_TCP_Connection = 1;
				Flag_TFTP_Error = 0;
    		}

    		//Blink LED and send periodic request to server
			Counter++;
			if(Counter >= 1000)
			{
				Counter = 0;
				HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin);
				TCP_Time_Update = HAL_GetTick();

				TFTP_Timer++;
				if(TFTP_Timer >= TFTP_Timer_Limit)
				{
					 tftp_client_close();
					 NVIC_SystemReset();
				}

				if(TFTP_CON)
				{
					if(!oack_received)
					{
						send_rrq_periodic();
					}
				}
			}

			//Check Ethernet Link Status, If Disconnected, Restart Device
			 Flag_Temp_ETH_Link_Status = Flag_ETH_Link_Status;
			 if(Flag_Temp_ETH_Link_Status == 0)
			 {
				 //Link Disconnected
				 tftp_client_close();
				 NVIC_SystemReset();
			 }

			if(TFTP_CON == 0)
			{
				tftp_init_client();
				osDelay(1);
			}

			if(Flag_TFTP_Error)			//TFTP Error - Closed Connection in the callback
			{
				NVIC_SystemReset();
			}

			if(Flag_OTA_File_Receive_Completed)
			{
		          printf("\rFile Size: %d bytes\n", tsize);
		          uint32_t t_buf[1] = {tsize};
		          if(FLASH_If_Erase(ADDR_FLASH_SECTOR_7_BANK1, 1) == FLASHIF_OK)
		          {
		          	if((FLASH_If_Write(ADDR_FLASH_SECTOR_7_BANK1, t_buf, 1)) != FLASHIF_OK)
		          	{
		          		printf("\r------>ERROR: Writing file size in flash<------\n");
		          	}
		          }
		          Flag_OTA_File_Receive_Completed = 0;
				  printf("\rRestarting the system....\n");
				  NVIC_SystemReset();
			}
			osDelay(1);
    	}
    }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartARCTask */
/**
* @brief Function implementing the ARCTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartARCTask */
void StartARCTask(void *argument)
{
  /* USER CODE BEGIN StartARCTask */
	char temp_uart_str[50];


	strcpy(temp_uart_str,"ALTEEM INSTRUMENTS ARC\n");
	Send_String_UART_232((uint8_t *)temp_uart_str);

	//Making All Digital Outputs Idle
	HAL_GPIO_WritePin(DO_1_GPIO_Port, DO_1_Pin,Pulse_Level_Idle);
	HAL_GPIO_WritePin(DO_2_GPIO_Port, DO_2_Pin,Pulse_Level_Idle);
	HAL_GPIO_WritePin(DO_3_GPIO_Port, DO_3_Pin,Pulse_Level_Idle);
	HAL_GPIO_WritePin(DO_4_GPIO_Port, DO_4_Pin,Pulse_Level_Idle);
	HAL_GPIO_WritePin(DO_5_GPIO_Port, DO_5_Pin,Pulse_Level_Idle);
	HAL_GPIO_WritePin(DO_6_GPIO_Port, DO_6_Pin,Pulse_Level_Idle);
	HAL_GPIO_WritePin(DO_7_GPIO_Port, DO_7_Pin,Pulse_Level_Idle);
//	HAL_GPIO_WritePin(DO_8_GPIO_Port, DO_8_Pin,Pulse_Level_Idle);

	HAL_GPIO_WritePin(LED_RED_GPIO_Port, 	LED_RED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, 	LED_GREEN_Pin,GPIO_PIN_RESET);

	//LCD UART RECEIVE
	if((UART4->ISR & 0X00000010) != 0)
	{
		__HAL_UART_CLEAR_FLAG(&huart4,UART_CLEAR_IDLEF);
	}

	HAL_UART_Receive_IT(&huart4, (uint8_t*)receive_buff[Flag_DMA_Buffer_Write], BUFFER_SIZE_DMA_UART4);

	//SEND INIT COMMANDS TO LCD AND WAIT FOR RESPONSE
	Wait_for_LCD_Init();

//	Config_Parameters[Alt_Mem_Read_OK] = 0;
//	Flash_Write_Config_Parameters_Single(Alt_Mem_Read_OK);
//	HAL_Delay(50);

	while(1)
	{
		Flash_Read_Config_Parameters();
		Flash_Read_Config_Float();
		Flash_Read_Job_Name();
		if(Config_Parameters[Alt_Mem_Read_OK] == 1947)
		{
			Send_String_UART_232((uint8_t *)"\nFLASH READ OK\n");
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_SET);
			Check_Config_Para_Limits();
			Check_Config_Float_Limits();
			Check_Job_Name();
			osDelay(10);
			Flash_Write_Config_Parameters();
			Flash_Write_Config_Float();
			Flash_Write_Job_Name();
			break;
		}
		else
		{
			Send_String_UART_232((uint8_t *)"\nFLASH READ ERROR\n");
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12,GPIO_PIN_SET);
			Initialize_Config_Parameters();
			Initialize_Config_Float();
			Initialize_Job_Name();
			osDelay(10);
			Flash_Write_Config_Parameters();
			Flash_Write_Config_Float();
			Flash_Write_Job_Name();
		}
	}

	Config_Parameters[Alt_Print_Mode] = 1;
	Flag_Print_Mode = Config_Parameters[Alt_Print_Mode];

	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Config_Parameters[Alt_Sensor_Gain]);

	//=======================================

	Flag_ARCSS_Model = HAL_GPIO_ReadPin(MODE_PP_ALTIMATCH_GPIO_Port, MODE_PP_ALTIMATCH_Pin);

	//	Flag_ARCSS_Model = MODEL_ALTIMATCH;
	//	Flag_ARCSS_Model = MODEL_PRINTPILOT;

	temp_speed_multiplier = 60000;
	temp_speed_multiplier = temp_speed_multiplier * Config_Parameters[Alt_BagLength];

	if(Flag_ARCSS_Model != MODEL_ALTIMATCH)
	{										//AM	PP
		icon_id_move_icon = 123;			//64	123
		maximum_pixel_move_icon = 38;		//75	38
		icon_half_width_pixel = 15;			//06	15
		position_move_icon_center_x = 127;	//200	127
		position_move_icon_center_y = 212;	//266	212

		COLOUR_BOX_START_X 	= 16;			//01	16
		COLOUR_BOX_START_Y 	= 33;			//01	33
		COLOUR_BOX_END_X 	= 237;			//47	237
		COLOUR_BOX_END_Y 	= 67;			//47	67

		LCD_Graph_Gate_Y_Start = 85;		//75	85
		LCD_Graph_Gate_Y_End   = 297;		//324	297

		Float_Length_Home = 4;				//3		4
		Float_Length_Diagnosis = 3;			//4		3

		Page_Home 		= Page_Home_PP;
		Page_Graph 		= Page_Graph_PP;
		Page_Password 	= Page_Password_PP;
		Page_Setting 	= Page_Setting_PP;
		Page_Recipe_Edit= Page_Recipe_Edit_PP;
		Page_Recipe_View= Page_Recipe_View_PP;
		Page_IP_Settings= Page_IP_Settings_PP;
		Page_Diagnose 	= Page_Diagnose_PP;
		Page_Good_Copy  = Page_Good_Copy_PP;
		Page_Error 		= Page_Error_PP;
	}

	Flag_Use_With_PC = Config_Parameters[Alt_Use_With_PC];
	if(Flag_Use_With_PC == 0)
	{
		Flag_Permit_Actions = 1;
	}

	//=======================================

	Calculate_Multiplier_mm_to_pixel();

	Flag_Home_Mode = Config_Parameters[Alt_Home_Mode];

	Set_Application_Mode();
	Screen_ID = Scr_Home;

	osDelay(10);
	if(Config_Parameters[Alt_ValidBaseLine] < Config_Parameters[Alt_Valid_BL_Window_Lower])
	{
		Config_Parameters[Alt_Valid_BL_Window_Lower] = 0;
		Flash_Write_Config_Parameters_Single(Alt_Valid_BL_Window_Lower);
	}

	Transmit_Config_Parameters(0,No_Of_Config_Parameters);
	osDelay(40);
	Transmit_Config_Float(0,No_Of_Config_Float);
	osDelay(40);
	/*Set LCD Brightness*/
	Transmit_Brightness_Value_00_3F(LCD_Brightness);
	osDelay(40);

	/* Timer init in interrupt mode*/
	HAL_TIM_Base_Start_IT(&htim1);						//Start Timer 1 with 5ms overflow
	HAL_TIM_Base_Start(&htim2);							//Start Timer 2

	__HAL_TIM_SET_COUNTER(&htim15,0);
	__HAL_TIM_SET_AUTORELOAD(&htim15,Max_Value_Timer_Idle);

	__HAL_TIM_SET_COUNTER(&htim16,0);
	__HAL_TIM_SET_AUTORELOAD(&htim16,Max_Value_Timer_Idle);

	MX_NVIC_Init();
	/* Commands Received from LCD is 0*/
	Flag_ReceivedValue = 0;

	Position_Gate[Slave] = Config_Parameters[Alt_Position_Gate_Slave_GCount];	//Gate_Graph_Default_Value;
	Init_Calc_ARCSS();

	Min_Mark_Height_Count = Config_Parameters[Alt_MinMarkHeight] * Convert_mm_To_PPR;		//Only for ARC
	Max_Mark_Height_Count = Config_Parameters[Alt_MaxMarkHeight] * Convert_mm_To_PPR;		//Only for ARC

	Set_Colour_Alarm(0);
	osDelay(20);

	Transmit_Page_ID(Scr_Home);
	Screen_ID = Scr_Home;
	Clear_Buffer();
	osDelay(20);

	Transmit_Buzzer_OFF();
	osDelay(20);

	sprintf(Debug_String,"\nMin Control Speed : %.2f\n", Config_Float[AltF_Min_Control_Speed]);
	Send_String_UART_232((uint8_t *)Debug_String);

	Send_String_UART_232((uint8_t *)"START OF WHILE\n");

	sprintf(Debug_String,"Baglength : %d, Total_Baglength_Count : %d, Skip_Limit : %d\n"
			,Config_Parameters[Alt_BagLength]
			,Total_Baglength_Count
			,Skip_Limit
			);
	Send_String_UART_232((uint8_t *)Debug_String);
	LCD_Draw_Colour_Box();
	Flag_DMA_ON = 1;

	Clear_Buffer();
	Fill_lcd_buffer_StartEndBytes();

	Config_Float[AltF_Disp_SetValue_Horz] = Distance_From_Slave_mm[Horizontal];
	Config_Float[AltF_Disp_SetValue_Vert] = Distance_From_Slave_mm[Vertical];
	BLE_Display_Float[BLE_DispF_SetValue_Horz] = Distance_From_Slave_mm[Horizontal];
	BLE_Display_Float[BLE_DispF_SetValue_Vert] = Distance_From_Slave_mm[Vertical];

	Config_Float[AltF_Disp_SetValue_Plus_OffSetV] = Config_Float[AltF_SetValue] + Config_Float[AltF_Corr_Offset_Vert];

	AutoSearch_Setmm = (Config_Parameters[Alt_BagLength] - (8 * Config_Float[AltF_SetValue])) / 2;

	Transmit_Config_Float(AltF_Disp_SetValue_Horz,3);
	Transmit_Config_Parameters_Single(ARC_Gate_Slave_Min_Valid_Value_Location,Gate_Graph_Min_Valid_Value_Slave_Count);
	Transmit_Config_Parameters_Single(ARC_Gate_Slave_Max_Valid_Value_Location,Gate_Graph_Max_Valid_Value_Slave_Count);
	Transmit_Config_Parameters_Single(Display_Error_Location,Flag_LCD_Init_Error);
	Transmit_Config_Parameters_Single(Version_Higher_Location,Version_Higher_Byte);
	Transmit_Config_Parameters_Single(Version_Lower_Location,Version_Lower_Byte);
	Transmit_Config_Parameters_Single(LCD_Flag_Permit_Action_Location,Flag_Permit_Actions);
	Job_Name_Send_String();

	Flag_Serial_Debug_Enable = 0;
	Transmit_Config_Parameters_Single(Serial_Debug_Enable_Location,Flag_Serial_Debug_Enable);

	Temp_Mac_Address = Config_Parameters[Alt_Station_ID];
	TCP_Time_Update = HAL_GetTick();

	Permit_Set_Threshold_Limit_Count = Permit_Set_Threshold_Limit_Perc;
	Permit_Set_Threshold_Limit_Count = Permit_Set_Threshold_Limit_Count * Config_Parameters[Alt_ValidBaseLine] / 100;

	sprintf(Debug_String,"Threshold Limit Cnt : %u\n",Permit_Set_Threshold_Limit_Count);
	Send_String_UART_232((uint8_t *)Debug_String);

	Flag_Memory_Read_Done = 1;

	uint16_t Counter = 0;

	_Bool Flag_Prev_TCP_Connected = 0;
	_Bool Flag_Heart_Bit_Sent = 0;

	Encoder_Init();
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	tcp_queue *msgr;

  /* Infinite loop */
  for(;;)
  {
	  arc_execution_main_stage = 1;

	  if((Flag_Prev_BaseLine_Error == 0) && (Flag_BaseLine_Error != 0))
	  {
		  __HAL_ADC_DISABLE_IT(&hadc1, ADC_IT_AWD1);
		  Prev_Diff_BL_VBL = 0;
	  }
	  Flag_Prev_BaseLine_Error = Flag_BaseLine_Error;

	  //TIM3 Overflow, Set default Threshold
	  if(Flag_TIM3_Baglength_Set_Threshold != 0)	//TIM3 overflow, set default threshould
	  {
		  if(Temp_Diff_BL_VBL != 0)
		  {
			  if((Flag_BaseLine_Error == 0) && (Flag_Print_Mode != 0))
			  {
				  int Temp_Diff = Temp_Diff_BL_VBL;
				  Temp_Diff = Temp_Diff - Prev_Diff_BL_VBL;
				  if(Temp_Diff < 0)
				  {
					  Temp_Diff = Temp_Diff * (-1);
				  }

				  if(Temp_Diff > Permit_Set_Threshold_Limit_Count)
				  {
					  __HAL_ADC_DISABLE_IT(&hadc1, ADC_IT_AWD1);
					  ADC_ChangeThreshold(Temp_Diff_BL_VBL,Max_Count_Base_Line);	//Limits to detect Rising Edge
					  Flag_TIM3_Baglength_Set_Threshold = 0;
					  Flag_Mark_Start = 0;
					  __HAL_ADC_ENABLE_IT(&hadc1, ADC_IT_AWD1);
					  Prev_Diff_BL_VBL = Temp_Diff_BL_VBL;
					  Set_Threshold_Count++;
				  }
			  }
		  }
		  else	//If BaseLine is Zero, Keep Interrupt Disabled till get proper baseline
		  {
			  __HAL_ADC_DISABLE_IT(&hadc1, ADC_IT_AWD1);
			  Prev_Diff_BL_VBL = 0;
		  }
		  Flag_Mark_Start = 0;
		  Flag_TIM3_Baglength_Set_Threshold = 0;
	  }


	  arc_execution_main_stage = 2;
	  /*Termination and Creation of TCP Task ---------------------------------------------------------------------*/
	  if((HAL_GetTick() - TCP_Time_Update) > 10000)	//If no update from TCP thread for 10 Seconds
	  {
		  Send_String_UART_232((uint8_t *)"TCP Thread Timeout\n");
//		  TCP_STUCK = 0;
		  TCP_Time_Update = HAL_GetTick();

		  if(TCPTaskHandle != NULL)
		  {
			  Flag_BLE_Enable 		= 0;
			  Flag_BLE_Read_ON 		= 0;
			  Read_Input_Group_Mode = 0;
			  Flag_BLE_Graph_ON		= 0;
			  //TCP Disconnected, So at Next Connect, Have to wait for job name for access
			  Flag_Job_Name_Received = 0;

			  if(Flag_Use_With_PC != 0)
			  {
				  Flag_Permit_Actions = 0;
				  Flag_Update_Permit_Actions = 1;
			  }
			  Flag_OTA_Request_Received = 0;
			  Flag_TCP_Connected = 0;			//???
			  Flag_First_Time_TCP_Request_Done = 0;

			  if(osThreadTerminate(TCPTaskHandle) == osOK)
			  {
				  Send_String_UART_232((uint8_t *)"TCP_Terminate Success\n");
				  TCPTaskHandle = NULL;
			  }
			  else
			  {
				  Send_String_UART_232((uint8_t *)"TCP_Terminate Error\n");
			  }
		  }

		  if(TCPTaskHandle == NULL)	//TCP Task Terminated Successfully
		  {
			  TCPTaskHandle = osThreadNew(StartTCPTask, NULL, &TCPTask_attributes);
			  if(TCPTaskHandle == NULL)
			  {
				  Send_String_UART_232((uint8_t *)"TCP Recreate Error\n");
				  Flag_TCP_Recreate = 0;
			  }
			  else
			  {
				  Send_String_UART_232((uint8_t *)"TCP Recreate Success\n");
				  Flag_TCP_Recreate = 1;
			  }
		  }

		  if (TCPTaskHandle == NULL)
		  {
			  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin, GPIO_PIN_SET);
		  }
	  }

	  arc_execution_main_stage = 3;
	  Counter++;
	  if(Counter >= 500)
	  {
		  Counter = 0;
		  HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
	  }

	  /* LCD Input Identification*/
	  if((UART4->ISR & 0X00000010) != 0)
	  {
			Handle_Line_Idle_Event();
	  }

	  	 arc_execution_main_stage = 4;

		//Check TCP_Read Queue, if data available, process it
		 int statusr = osMessageQueueGet(TCP_ReadHandle, &msgr, NULL, 0);
		 if(statusr == osOK)
		 {
//			 HAL_UART_Transmit(&huart3, "#", 1, 10);
//			 HAL_UART_Transmit(&huart3, msgr->data, msgr->length, 400);
//			 HAL_UART_Transmit(&huart3, "#", 1, 10);
			 Process_TCP_Received_Data(msgr->data,msgr->length);
			 vPortFree(msgr->data);
			 vPortFree(msgr);
		 }

		 arc_execution_main_stage = 5;

		 //==================================

		if(Flag_Slave_DMA_Buffer_Write != Flag_Slave_DMA_Buffer_Read)
		{
			Slave_Handle_Line_Idle_Event();

			Flag_Slave_DMA_Buffer_Read++;
		    if(Flag_Slave_DMA_Buffer_Read >= Max_Count_Slave_DMA_Buffer_Write_Read)
		    {
		    	Flag_Slave_DMA_Buffer_Read = 0;
		    }
		}

		arc_execution_main_stage = 6;

		//===================================

		Check_For_LCD_Touch_Command();

		arc_execution_main_stage = 7;

		if((Flag_GateSet != 0) && (Flag_BaseLine_Error == 0) && (Flag_Print_Mode != 0) && (mpm_int > Config_Float[AltF_Min_Control_Speed]))
		{
			arc_execution_main_stage = 8;
			Common_Correction_Logic_Vertical();
			arc_execution_main_stage = 9;
			Common_Correction_Logic_Horizontal();
		}
		arc_execution_main_stage = 10;

		if(Flag_Home_Mode == 0)
		{
			if(Flag_Print_Mode == 0)
				Flag_Mark_Missing = 0;
			else
			{
				if((Flag_Auto_Horizontal != 0) && ((Flag_M0_Missing != 0) || (Flag_M2_Missing != 0)))
					Flag_Mark_Missing = 1;
				else if((Flag_Auto_Vertical != 0) && ((Flag_M1_Missing != 0) || (Flag_M2_Missing != 0)))
					Flag_Mark_Missing = 1;
				else
					Flag_Mark_Missing = 0;
			}

			if((Flag_Print_Mode != 0) && ((Flag_Over_Error_Horz != 0) || (Flag_Over_Error_Vert != 0)))
			{
				Flag_Over_Error = 1;
			}
			else
			{
				Flag_Over_Error = 0;
			}
		}

		if(Zero_Spd_Time >= Zero_Spd_Time_Limit)
		{
			Zero_Spd_Time 	= 0;
			mpm_int = 0;
		}
		Set_Alarm_Status();

		arc_execution_main_stage = 13;

		if(Flag_Home_Mode == 0)
		{
			Action_LCD_Commands();
		}
		else
		{
			Action_LCD_Commands_Home();
			if(Flag_Update_SetValue != 0)
			{
				Config_Float[AltF_Disp_SetValue_Vert] = SetValue_Home_Mode;
				Transmit_Config_Float(AltF_Disp_SetValue_Vert,1);
				Flag_Update_SetValue = 0;
			}
		}

		arc_execution_main_stage = 14;

		//===================================

		if(Perform_Good_Copy_Horz_Stage == 2)
		{
			Flash_Write_Config_Float_Single(AltF_Corr_Offset_Horz);
			Transmit_Config_Float_Single(AltF_Corr_Offset_Horz,Config_Float[AltF_Corr_Offset_Horz]);
			if(Flag_BLE_Enable != 0)	//30.07.2025
			{
				BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Horz,Config_Float[AltF_Corr_Offset_Horz]);
			}
			Perform_Good_Copy_Horz_Stage = 0;
		}

		arc_execution_main_stage = 15;

		if(Perform_Good_Copy_Vert_Stage == 2)
		{
			Flash_Write_Config_Float_Single(AltF_Corr_Offset_Vert);
			Transmit_Config_Float_Single(AltF_Corr_Offset_Vert,Config_Float[AltF_Corr_Offset_Vert]);
			if(Flag_BLE_Enable != 0)	//30.07.2025
			{
				BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Vert,Config_Float[AltF_Corr_Offset_Vert]);
			}
			Config_Float[AltF_Disp_SetValue_Plus_OffSetV] = Config_Float[AltF_SetValue] + Config_Float[AltF_Corr_Offset_Vert];
			Transmit_Config_Float_Single(AltF_Disp_SetValue_Plus_OffSetV,Config_Float[AltF_Disp_SetValue_Plus_OffSetV]);

			Perform_Good_Copy_Vert_Stage = 0;
		}

		arc_execution_main_stage = 16;

		ARC_LCD_Display();

		if(Flag_TCP_Connected != 0)
		{
			arc_execution_main_stage = 17;

			if(Flag_Prev_TCP_Connected == 0)	//Reconnection
			{
				Seconds_From_TCP_Connect = 0;
				Seconds_From_TCP_Recreate = 0;
				Seconds_From_TCP_Reconnect = 0;
				Flag_TCP_Reconnect = 1;
				Flag_Heart_Bit_Sent = 0;
			}

			if(Flag_EncError != 0)
			{
				if(Flag_Heart_Bit_Sent != 0)
				{
					Flag_EncError = 0;
					BLE_Info_Reset_Recreate_Reconnect_EncErr(Info_Encoder_Error);
//					sprintf(Debug_String,"Disp : %u, EncZ : %u, PrevZ : %u\n",EncError_DispCount, Temp_Disp_Enc_Count_at_Z, Temp_Disp_Prev_Enc_Count_at_Z);
//					Send_String_UART_232(Debug_String);
				}
			}

			if(Flag_Time_Second != 0)
			{
				Flag_Time_Second = 0;

				if(Flag_Reset_Info_Sent == 0)
				{
					Flag_TCP_Reconnect = 0;
					Seconds_From_TCP_Connect++;
					 if(Seconds_From_TCP_Connect >= 5)
					 {
						 Seconds_From_TCP_Connect = 0;
						 BLE_Info_Reset_Recreate_Reconnect_EncErr(Info_Reset);
						 Flag_Reset_Info_Sent = 1;
					 }
				}

				if(Flag_TCP_Recreate != 0)
				{
					Flag_TCP_Reconnect = 0;		//If Recreate, no need to send Reconnect
					Seconds_From_TCP_Recreate++;
					 if(Seconds_From_TCP_Recreate >= 5)
					 {
						 Seconds_From_TCP_Recreate = 0;
						 BLE_Info_Reset_Recreate_Reconnect_EncErr(Info_TCP_Recreate);
						 Flag_TCP_Recreate = 0;
					 }
				}

				if(Flag_TCP_Reconnect != 0)
				{
					Seconds_From_TCP_Reconnect++;
					 if(Seconds_From_TCP_Reconnect >= 5)
					 {
						 Seconds_From_TCP_Reconnect = 0;
						 BLE_Info_Reset_Recreate_Reconnect_EncErr(Info_TCP_Reconnect);
						 Flag_TCP_Reconnect = 0;
					 }
				}

				if((Read_Input_Group_Mode == 0x00) || ((Flag_BLE_Graph_ON != 0) && (mpm_int == 0)))
				{
					arc_execution_main_stage = 18;
					BLE_Send_Communication_Check_Frame();
					Flag_Heart_Bit_Sent = 1;
				}
			}
			arc_execution_main_stage = 19;
			BLE_ARC_LCD_Display();
		}
		else
		{
			arc_execution_main_stage = 20;

			if(Flag_Time_Second != 0)
			{
				Flag_Time_Second = 0;
			}

//			if(Flag_EncError != 0)
//			{
//				Flag_EncError = 0;
//				sprintf(Debug_String,"Disp : %u, EncZ : %u, PrevZ : %u\n",EncError_DispCount, Temp_Disp_Enc_Count_at_Z, Temp_Disp_Prev_Enc_Count_at_Z);
//				Send_String_UART_232(Debug_String);
//			}

			if(Flag_Prev_TCP_Connected != 0)	//Previously Connected, but currently disconnected
			{
				arc_execution_main_stage = 21;

				 Flag_BLE_Enable 		= 0;
				 Flag_BLE_Read_ON 		= 0;
				 Read_Input_Group_Mode 	= 0;
				 Flag_BLE_Graph_ON		= 0;

				 if(Flag_Use_With_PC != 0)
				 {
					 Flag_Permit_Actions = 0;
					 Flag_Update_Permit_Actions = 1;
				 }
				 //TCP Disconnected, So at Next Connect, Have to wait for job name for access
				 Flag_Job_Name_Received = 0;

				 arc_execution_main_stage = 22;
				 osMessageQueueReset(TCP_WriteHandle);
				 osMessageQueueReset(TCP_ReadHandle);

				 Send_String_UART_232((uint8_t *)"TCPSend Queue Reset\n");
			}
		}
		Flag_Prev_TCP_Connected = Flag_TCP_Connected;

		arc_execution_main_stage = 23;


		if(Flag_BLE_KeyPress_Event != 0)		//Key Press Received, But yet Key Release not received
		{
//			if(Time_BLE_RepeatKeyPress_ms >= Config_Parameters[Alt_BLE_Time_Key_Release])
			if(Time_BLE_RepeatKeyPress_ms >= 2000)
			{
				rec_function_byte = 0;
				Flag_TouchKeyPressed = 0;
				Flag_BLE_KeyPress_Event = 0;
				Time_BLE_RepeatKeyPress_ms = 0;
			}
		}

		if((Flag_Serial_Debug_Enable != 0) && (Flag_Debug != 0))
		{
			Send_String_UART_232((uint8_t *)Debug_String);
			Flag_Debug = 0;
		}

		if(Real_Baglength_Arrive_Count != Prev_Baglength_Arrive_Count)
		{
//			sprintf(Debug_String,"Real : %u, Exec : %u, Time : %lu\n",Real_Baglength_Arrive_Count,Executed_Baglength_Arrive_Count,BLE_Display_Time_Execution);
//			Send_String_UART_232(Debug_String);
			Prev_Baglength_Arrive_Count = Real_Baglength_Arrive_Count;
		}

		if(Flag_Update_Permit_Actions != 0)
		{
			Transmit_Config_Parameters_Single(LCD_Flag_Permit_Action_Location,Flag_Permit_Actions);
			Flag_Update_Permit_Actions = 0;
		}

		if(Flag_AutoSearch_Update != 0)
		{
			arc_execution_main_stage = 24;

			Transmit_Config_Parameters_Single(Alt_Index_mm,Config_Parameters[Alt_Index_mm]);
			Flag_AutoSearch_Update = 0;
			Flash_Write_Config_Parameters_Single(Alt_Index_mm);

			if(Flag_BLE_Enable != 0)
			{
				BLE_Info_Config_Register_Changed(Alt_Index_mm,Config_Parameters[Alt_Index_mm]);
			}

			if(Flag_AutoSearch_Error == 0)
			{
				arc_execution_main_stage = 25;

				uint16_t temp_100mm_ppr = Convert_mm_To_PPR * AutoSearch_Setmm;
				uint16_t temp_width_mark = 	AutoSearch_Falling_PPR - AutoSearch_Rising_PPR;
				uint16_t temp_diff_gate_start_to_mark_start = (GateWidth_PPR - temp_width_mark)/2;
				int16_t temp_gate_start = temp_100mm_ppr - temp_width_mark - temp_diff_gate_start_to_mark_start;
						//Rising edge new location ppr
				temp_gate_start = temp_gate_start / Skip_Limit;	//start gate graph count

				if(Config_Parameters[Alt_MasterMarkPosition] == Master_Mark_First)	//Master First, So Horz Master will cover 1st Mark
				{
					Position_Gate[Master_Horz] = temp_gate_start;
					Position_Gate[Slave] = Position_Gate[Master_Horz] + SetValue_Graph_Count[Horizontal];
					Position_Gate[Master_Vert] = Position_Gate[Slave] - SetValue_Graph_Count[Vertical];
				}
				else	//Slave First, So Slave will cover 1st Mark
				{
					Position_Gate[Slave] = temp_gate_start;
					Position_Gate[Master_Horz] 	= Position_Gate[Slave] + SetValue_Graph_Count[Horizontal];
					Position_Gate[Master_Vert] 	= Position_Gate[Slave] + SetValue_Graph_Count[Vertical];
				}
				Gate_Calculations();

				if(Flag_Serial_Debug_Enable != 0)
				{
					sprintf(Debug_String,"\n\nAutoSearch_PPR : %d,AutoSearch_mm : %d\n",AutoSearch_PPR,AutoSearch_mm);
					Send_String_UART_232((uint8_t *)Debug_String);
					sprintf(Debug_String,"Rising PPR : %d,Falling PPR : %d\n",AutoSearch_Rising_PPR,AutoSearch_Falling_PPR);
					Send_String_UART_232((uint8_t *)Debug_String);
					sprintf(Debug_String,"Start GCount : %d\n",temp_gate_start);
					Send_String_UART_232((uint8_t *)Debug_String);
				}
			}
			else
			{
				arc_execution_main_stage = 26;

				Flag_AutoSearch_Error = 0;
				if(Flag_Serial_Debug_Enable != 0)
				{
					sprintf(Debug_String,"\n\nAutoSearch Error\n");
					Send_String_UART_232((uint8_t *)Debug_String);
				}
			}
		}
    osDelay(1);
//    osThreadYield();
  }
  /* USER CODE END StartARCTask */
}

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x38000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  /* USER CODE BEGIN Callback 0 */
//
//  /* USER CODE END Callback 0 */
//  if (htim->Instance == TIM7)
//  {
//    HAL_IncTick();
//  }
//  /* USER CODE BEGIN Callback 1 */
//
//  /* USER CODE END Callback 1 */
//}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
//void Error_Handler(void)
//{
//  /* USER CODE BEGIN Error_Handler_Debug */
//	/* User can add his own implementation to report the HAL error return state */
//  __disable_irq();
//  while (1)
//  {
//	  break;
//  }
//  /* USER CODE END Error_Handler_Debug */
//}
#ifdef USE_FULL_ASSERT
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
