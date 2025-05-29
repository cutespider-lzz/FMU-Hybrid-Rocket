/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "integer.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern unsigned long FreeRTOSRunTimeTicks;
extern BYTE work[];
extern uint8_t Rocket[];

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

extern ADC_HandleTypeDef hadc3;

extern I2C_HandleTypeDef hi2c1;

extern SD_HandleTypeDef hsd1;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_uart8_tx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IMU_D0_Pin GPIO_PIN_2
#define IMU_D0_GPIO_Port GPIOE
#define IMU_D1_Pin GPIO_PIN_3
#define IMU_D1_GPIO_Port GPIOE
#define IMU_D2_Pin GPIO_PIN_4
#define IMU_D2_GPIO_Port GPIOE
#define IMU_D3_Pin GPIO_PIN_5
#define IMU_D3_GPIO_Port GPIOE
#define SPI2_CS_Pin GPIO_PIN_6
#define SPI2_CS_GPIO_Port GPIOE
#define SPI2_IO_Pin GPIO_PIN_13
#define SPI2_IO_GPIO_Port GPIOC
#define VOLTAGE_Pin GPIO_PIN_0
#define VOLTAGE_GPIO_Port GPIOC
#define CURRENT_Pin GPIO_PIN_3
#define CURRENT_GPIO_Port GPIOC
#define S_EN_Pin GPIO_PIN_4
#define S_EN_GPIO_Port GPIOA
#define ADXL_CS_Pin GPIO_PIN_4
#define ADXL_CS_GPIO_Port GPIOC
#define ADXL_INT_Pin GPIO_PIN_5
#define ADXL_INT_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_0
#define BUZZER_GPIO_Port GPIOB
#define TELEM2_IO1_Pin GPIO_PIN_1
#define TELEM2_IO1_GPIO_Port GPIOB
#define TELEM2_IO2_Pin GPIO_PIN_2
#define TELEM2_IO2_GPIO_Port GPIOB
#define SIGNAL_Pin GPIO_PIN_9
#define SIGNAL_GPIO_Port GPIOE
#define RS232_IO1_Pin GPIO_PIN_10
#define RS232_IO1_GPIO_Port GPIOE
#define RS232_IO2_Pin GPIO_PIN_11
#define RS232_IO2_GPIO_Port GPIOE
#define RS232_VALID_Pin GPIO_PIN_12
#define RS232_VALID_GPIO_Port GPIOE
#define RS232_RDY_Pin GPIO_PIN_13
#define RS232_RDY_GPIO_Port GPIOE
#define RS232_OFF_Pin GPIO_PIN_14
#define RS232_OFF_GPIO_Port GPIOE
#define RS232_ON_Pin GPIO_PIN_15
#define RS232_ON_GPIO_Port GPIOE
#define BAT4_Pin GPIO_PIN_14
#define BAT4_GPIO_Port GPIOB
#define BAT3_Pin GPIO_PIN_15
#define BAT3_GPIO_Port GPIOB
#define BAT2_Pin GPIO_PIN_8
#define BAT2_GPIO_Port GPIOD
#define BAT1_Pin GPIO_PIN_9
#define BAT1_GPIO_Port GPIOD
#define TELEM1_IO2_Pin GPIO_PIN_10
#define TELEM1_IO2_GPIO_Port GPIOD
#define TELEM1_IO1_Pin GPIO_PIN_11
#define TELEM1_IO1_GPIO_Port GPIOD
#define SDIO_CD_Pin GPIO_PIN_15
#define SDIO_CD_GPIO_Port GPIOA
#define TRIGGER_Pin GPIO_PIN_5
#define TRIGGER_GPIO_Port GPIOB
#define TELEM3_IO2_Pin GPIO_PIN_8
#define TELEM3_IO2_GPIO_Port GPIOB
#define TELEM3_IO1_Pin GPIO_PIN_9
#define TELEM3_IO1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
