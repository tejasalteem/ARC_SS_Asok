/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern _Bool Flag_Debug;
extern char debug_string[100];
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef struct
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
} ExceptionStackFrame;

typedef struct
{
	uint8_t FAULT;
	uint32_t PC;
}Program_Counter;

typedef enum{
	ERROR_HANDLER_NMI = 1,
	ERROR_HANDLER_HARDFAULT,
	ERROR_HANDLER_MEMMANGE,
	ERROR_HANDLER_BUSFAULT,
	ERROR_HANDLER_USAGEFAULT,
	ERROR_HANDLER_WATCHDOG,
	ERROR_HANDLER_INIT
}ERROR_HANDLER;

enum {
	RESET_UNKNOWN = 1,
	RESET_D1_STANDBY,
	RESET_WINDOW_WATCHDOG,
	RESET_INDEPENDENT_WATCHDOG,
	RESET_SYSTEM,
	RESET_POWER_ON,
	RESET_PIN,
	RESET_BROWNOUT,
	RESET_D2_POWER_ON,
	RESET_D1_POWER_ON,
	RESET_CPU
};

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern  _Bool TCP_STUCK;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
typedef  void (*pFunction)(void);
void fault_handler(ExceptionStackFrame *stacked_args, uint8_t fault);
extern Program_Counter err_pc;

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_SW_Pin GPIO_PIN_13
#define USER_SW_GPIO_Port GPIOC
#define DO_1_Pin GPIO_PIN_2
#define DO_1_GPIO_Port GPIOF
#define DO_2_Pin GPIO_PIN_3
#define DO_2_GPIO_Port GPIOF
#define DO_3_Pin GPIO_PIN_4
#define DO_3_GPIO_Port GPIOF
#define DO_4_Pin GPIO_PIN_5
#define DO_4_GPIO_Port GPIOF
#define DO_5_Pin GPIO_PIN_6
#define DO_5_GPIO_Port GPIOF
#define DO_6_Pin GPIO_PIN_7
#define DO_6_GPIO_Port GPIOF
#define DO_7_Pin GPIO_PIN_8
#define DO_7_GPIO_Port GPIOF
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_0
#define LED_GREEN_GPIO_Port GPIOB
#define MODE_PP_ALTIMATCH_Pin GPIO_PIN_15
#define MODE_PP_ALTIMATCH_GPIO_Port GPIOF
#define DETECT_Z_OP_Pin GPIO_PIN_10
#define DETECT_Z_OP_GPIO_Port GPIOB
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOB
#define DETECT_TIM3_OP_Pin GPIO_PIN_15
#define DETECT_TIM3_OP_GPIO_Port GPIOB
#define STLK_VCP_RX_Pin GPIO_PIN_8
#define STLK_VCP_RX_GPIO_Port GPIOD
#define STLK_VCP_TX_Pin GPIO_PIN_9
#define STLK_VCP_TX_GPIO_Port GPIOD
#define ENCODER_Z_Pin GPIO_PIN_8
#define ENCODER_Z_GPIO_Port GPIOC
#define ENCODER_Z_EXTI_IRQn EXTI9_5_IRQn
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define LED_YELLOW_Pin GPIO_PIN_1
#define LED_YELLOW_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

typedef struct
{
	uint16_t length;
	uint8_t *data;
}tcp_queue;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
