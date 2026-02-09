/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void fault_handler(ExceptionStackFrame *stacked_args, uint8_t fault);
#define Flash_Address_PC	0x0300		//Also defined in ARCSS_variables. any change here should be reflected there also.
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ETH_HandleTypeDef heth;
extern DMA_HandleTypeDef hdma_adc3;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;
extern UART_HandleTypeDef huart4;
extern TIM_HandleTypeDef htim7;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
__attribute__((naked)) void NMI_Handler(void) {
    __asm volatile (
    		" movs r0, #4 \n"
    		" movs r1, lr \n"
    		" tst r0, r1 \n"
    		" beq _MSP_1 \n"
    		" mrs r0, psp \n"
    		" b _HALT_1 \n"
    		"_MSP_1: \n"
    		" mrs r0, msp \n"
    		"_HALT_1: \n"
    		" mov r1, #1 \n"
    		" b fault_handler \n"
    );
}

/**
  * @brief This function handles Hard fault interrupt.
  */
__attribute__((naked)) void HardFault_Handler(void) {
    __asm volatile (
    		" movs r0, #4 \n"
    		" movs r1, lr \n"
    		" tst r0, r1 \n"
    		" beq _MSP \n"
    		" mrs r0, psp \n"
    		" b _HALT \n"
    		"_MSP: \n"
    		" mrs r0, msp \n"
    		"_HALT: \n"
    		"mov r1, #2 \n"
    		" b fault_handler \n"
    );
}

/**
  * @brief This function handles Memory management fault.
  */
__attribute__((naked)) void MemManage_Handler(void) {
    __asm volatile (
    		" movs r0, #4 \n"
    		" movs r1, lr \n"
    		" tst r0, r1 \n"
    		" beq _MSP_2 \n"
    		" mrs r0, psp \n"
    		" b _HALT_2 \n"
    		"_MSP_2: \n"
    		" mrs r0, msp \n"
    		"_HALT_2: \n"
    		"mov r1, #3 \n"
    		" b fault_handler\n"
    );
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
__attribute__((naked)) void BusFault_Handler(void) {
    __asm volatile (
    		" movs r0, #4 \n"
    		" movs r1, lr \n"
    		" tst r0, r1 \n"
    		" beq _MSP_3 \n"
    		" mrs r0, psp \n"
    		" b _HALT_3 \n"
    		"_MSP_3: \n"
    		" mrs r0, msp \n"
    		"_HALT_3: \n"
    		"mov r1, #4 \n"
    		" b fault_handler\n"
    );
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
__attribute__((naked)) void UsageFault_Handler(void) {
    __asm volatile (
    		" movs r0, #4 \n"
    		" movs r1, lr \n"
    		" tst r0, r1 \n"
    		" beq _MSP_4 \n"
    		" mrs r0, psp \n"
    		" b _HALT_4 \n"
    		"_MSP_4: \n"
    		" mrs r0, msp \n"
    		"_HALT_4: \n"
    		"mov r1, #5 \n"
    		" b fault_handler \n"
    );
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc3);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(ENCODER_Z_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt.
  */
void TIM1_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_IRQn 0 */

  /* USER CODE END TIM1_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_IRQn 1 */

  /* USER CODE END TIM1_UP_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */

  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles Ethernet global interrupt.
  */
void ETH_IRQHandler(void)
{
  /* USER CODE BEGIN ETH_IRQn 0 */

  /* USER CODE END ETH_IRQn 0 */
  HAL_ETH_IRQHandler(&heth);
  /* USER CODE BEGIN ETH_IRQn 1 */

  /* USER CODE END ETH_IRQn 1 */
}

/**
  * @brief This function handles TIM15 global interrupt.
  */
void TIM15_IRQHandler(void)
{
  /* USER CODE BEGIN TIM15_IRQn 0 */

  /* USER CODE END TIM15_IRQn 0 */
  HAL_TIM_IRQHandler(&htim15);
  /* USER CODE BEGIN TIM15_IRQn 1 */

  /* USER CODE END TIM15_IRQn 1 */
}

/**
  * @brief This function handles TIM16 global interrupt.
  */
void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */

  /* USER CODE END TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim16);
  /* USER CODE BEGIN TIM16_IRQn 1 */

  /* USER CODE END TIM16_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void fault_handler(ExceptionStackFrame *stacked_args, uint8_t fault)
{
    err_pc.PC = stacked_args->pc;
    err_pc.FAULT = fault;

    osDelay(20);
    Flash_Write_Buffer(Flash_Address_PC, (uint8_t*)&err_pc, sizeof(err_pc));

    char temp_fault[50];
    sprintf(temp_fault, "INSIDE FAULT, PC: 0x%08X, ERR: %d\n", err_pc.PC, err_pc.FAULT);
    Send_String_UART_232((uint8_t *)temp_fault);

    // Halt here to allow inspection with a debugger
    while(1)
    {
        // You can use a debugger breakpoint here
//    	HAL_IWDG_Refresh(&hiwdg1);
//		HAL_WWDG_Refresh(&hwwdg1);
//		osDelay(25);
    }
}


//__attribute__((naked)) void NMI_Handler(void) {
//    __asm volatile (
//    		" movs r0, #4 \n"
//    		" movs r1, lr \n"
//    		" tst r0, r1 \n"
//    		" beq _MSP_1 \n"
//    		" mrs r0, psp \n"
//    		" b _HALT_1 \n"
//    		"_MSP_1: \n"
//    		" mrs r0, msp \n"
//    		"_HALT_1: \n"
//    		" mov r1, #1 \n"
//    		" b fault_handler \n"
//    );
//}
//
//__attribute__((naked)) void HardFault_Handler(void) {
//    __asm volatile (
//    		" movs r0, #4 \n"
//    		" movs r1, lr \n"
//    		" tst r0, r1 \n"
//    		" beq _MSP \n"
//    		" mrs r0, psp \n"
//    		" b _HALT \n"
//    		"_MSP: \n"
//    		" mrs r0, msp \n"
//    		"_HALT: \n"
//    		"mov r1, #2 \n"
//    		" b fault_handler \n"
//    );
//}
//
//__attribute__((naked)) void MemManage_Handler(void) {
//    __asm volatile (
//    		" movs r0, #4 \n"
//    		" movs r1, lr \n"
//    		" tst r0, r1 \n"
//    		" beq _MSP_2 \n"
//    		" mrs r0, psp \n"
//    		" b _HALT_2 \n"
//    		"_MSP_2: \n"
//    		" mrs r0, msp \n"
//    		"_HALT_2: \n"
//    		"mov r1, #3 \n"
//    		" b fault_handler\n"
//    );
//}
//
//
//__attribute__((naked)) void BusFault_Handler(void) {
//    __asm volatile (
//    		" movs r0, #4 \n"
//    		" movs r1, lr \n"
//    		" tst r0, r1 \n"
//    		" beq _MSP_3 \n"
//    		" mrs r0, psp \n"
//    		" b _HALT_3 \n"
//    		"_MSP_3: \n"
//    		" mrs r0, msp \n"
//    		"_HALT_3: \n"
//    		"mov r1, #4 \n"
//    		" b fault_handler\n"
//    );
//}
//
//__attribute__((naked)) void UsageFault_Handler(void) {
//    __asm volatile (
//    		" movs r0, #4 \n"
//    		" movs r1, lr \n"
//    		" tst r0, r1 \n"
//    		" beq _MSP_4 \n"
//    		" mrs r0, psp \n"
//    		" b _HALT_4 \n"
//    		"_MSP_4: \n"
//    		" mrs r0, msp \n"
//    		"_HALT_4: \n"
//    		"mov r1, #5 \n"
//    		" b fault_handler \n"
//    );
//}
//
//__attribute__((naked)) void WWDG_IRQHandler(void) {
//    __asm volatile (
//    		" movs r0, #4 \n"
//    		" movs r1, lr \n"
//    		" tst r0, r1 \n"
//    		" beq _MSP_5 \n"
//    		" mrs r0, psp \n"
//    		" b _HALT_5 \n"
//    		"_MSP_5: \n"
//    		" mrs r0, msp \n"
//    		"_HALT_5: \n"
//    		"mov r1, #6 \n"
//    		" b fault_handler \n"
//    );
//}

/* USER CODE END 1 */
