/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM2_1_ENC1A_Pin GPIO_PIN_0
#define TIM2_1_ENC1A_GPIO_Port GPIOA
#define TIM2_2_ENC1B_Pin GPIO_PIN_1
#define TIM2_2_ENC1B_GPIO_Port GPIOA
#define USART2_TX_BT1_Pin GPIO_PIN_2
#define USART2_TX_BT1_GPIO_Port GPIOA
#define USART2_RX_BT1_Pin GPIO_PIN_3
#define USART2_RX_BT1_GPIO_Port GPIOA
#define TIM3_1_ENC2A_Pin GPIO_PIN_6
#define TIM3_1_ENC2A_GPIO_Port GPIOA
#define TIM3_2_ENC2B_Pin GPIO_PIN_7
#define TIM3_2_ENC2B_GPIO_Port GPIOA
#define PB1_AN_5_0V_Pin GPIO_PIN_0
#define PB1_AN_5_0V_GPIO_Port GPIOB
#define PB1_AN_3_3V_Pin GPIO_PIN_1
#define PB1_AN_3_3V_GPIO_Port GPIOB
#define USART3_TX_BT2_Pin GPIO_PIN_10
#define USART3_TX_BT2_GPIO_Port GPIOB
#define USART3_RX_BT2_Pin GPIO_PIN_11
#define USART3_RX_BT2_GPIO_Port GPIOB
#define PB12_MA_IN2_Pin GPIO_PIN_12
#define PB12_MA_IN2_GPIO_Port GPIOB
#define PB13_MA_IN1_Pin GPIO_PIN_13
#define PB13_MA_IN1_GPIO_Port GPIOB
#define PB14_MB_IN1_Pin GPIO_PIN_14
#define PB14_MB_IN1_GPIO_Port GPIOB
#define PB15_MB_IN2_Pin GPIO_PIN_15
#define PB15_MB_IN2_GPIO_Port GPIOB
#define TIM1_1_MA_PWM_Pin GPIO_PIN_8
#define TIM1_1_MA_PWM_GPIO_Port GPIOA
#define TIM1_2_MB_PWM_Pin GPIO_PIN_9
#define TIM1_2_MB_PWM_GPIO_Port GPIOA
#define TIM1_3_SER1_Pin GPIO_PIN_10
#define TIM1_3_SER1_GPIO_Port GPIOA
#define TIM1_4_SER2_Pin GPIO_PIN_11
#define TIM1_4_SER2_GPIO_Port GPIOA
#define PA12_EXTI12_MPU_Pin GPIO_PIN_12
#define PA12_EXTI12_MPU_GPIO_Port GPIOA
#define PA15_LED1R_Pin GPIO_PIN_15
#define PA15_LED1R_GPIO_Port GPIOA
#define PB3_LED1G_Pin GPIO_PIN_3
#define PB3_LED1G_GPIO_Port GPIOB
#define PB4_LED1B_Pin GPIO_PIN_4
#define PB4_LED1B_GPIO_Port GPIOB
#define PB5_LED2R_Pin GPIO_PIN_5
#define PB5_LED2R_GPIO_Port GPIOB
#define PB6_LED2G_Pin GPIO_PIN_6
#define PB6_LED2G_GPIO_Port GPIOB
#define PB7_LED2B_Pin GPIO_PIN_7
#define PB7_LED2B_GPIO_Port GPIOB
#define I2C1_SCL_MPU_Pin GPIO_PIN_8
#define I2C1_SCL_MPU_GPIO_Port GPIOB
#define I2C1_SDA_MPU_Pin GPIO_PIN_9
#define I2C1_SDA_MPU_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
