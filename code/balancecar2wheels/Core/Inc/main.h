/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define MOTOR_PWMA_Pin GPIO_PIN_0
#define MOTOR_PWMA_GPIO_Port GPIOA
#define MOTOR_PWMB_Pin GPIO_PIN_1
#define MOTOR_PWMB_GPIO_Port GPIOA
#define BT_USART2_TX_Pin GPIO_PIN_2
#define BT_USART2_TX_GPIO_Port GPIOA
#define BT_USART2_RX_Pin GPIO_PIN_3
#define BT_USART2_RX_GPIO_Port GPIOA
#define MOTOR_A_EA_Pin GPIO_PIN_6
#define MOTOR_A_EA_GPIO_Port GPIOA
#define MOTOR_A_EB_Pin GPIO_PIN_7
#define MOTOR_A_EB_GPIO_Port GPIOA
#define WIFI_USART3_TX_Pin GPIO_PIN_10
#define WIFI_USART3_TX_GPIO_Port GPIOB
#define WIFI_USART3_RX_Pin GPIO_PIN_11
#define WIFI_USART3_RX_GPIO_Port GPIOB
#define WIFI_RST_Pin GPIO_PIN_12
#define WIFI_RST_GPIO_Port GPIOB
#define MOTOR_STBY_Pin GPIO_PIN_13
#define MOTOR_STBY_GPIO_Port GPIOB
#define MOTOR_AIN2_Pin GPIO_PIN_14
#define MOTOR_AIN2_GPIO_Port GPIOB
#define MOTOR_AIN1_Pin GPIO_PIN_15
#define MOTOR_AIN1_GPIO_Port GPIOB
#define INFRARED_OUT_Pin GPIO_PIN_8
#define INFRARED_OUT_GPIO_Port GPIOA
#define MSG_USART1_TX_Pin GPIO_PIN_9
#define MSG_USART1_TX_GPIO_Port GPIOA
#define MSG_USART1_RX_Pin GPIO_PIN_10
#define MSG_USART1_RX_GPIO_Port GPIOA
#define Echo_Pin GPIO_PIN_11
#define Echo_GPIO_Port GPIOA
#define Trig_Pin GPIO_PIN_12
#define Trig_GPIO_Port GPIOA
#define MPU6050_INT_Pin GPIO_PIN_15
#define MPU6050_INT_GPIO_Port GPIOA
#define MPU6050_INT_EXTI_IRQn EXTI15_10_IRQn
#define MOTOR_BIN2_Pin GPIO_PIN_4
#define MOTOR_BIN2_GPIO_Port GPIOB
#define MOTOR_BIN1_Pin GPIO_PIN_5
#define MOTOR_BIN1_GPIO_Port GPIOB
#define MOTOR_B_EA_Pin GPIO_PIN_6
#define MOTOR_B_EA_GPIO_Port GPIOB
#define MOTOR_B_EB_Pin GPIO_PIN_7
#define MOTOR_B_EB_GPIO_Port GPIOB
#define MPU6050_SCL_Pin GPIO_PIN_8
#define MPU6050_SCL_GPIO_Port GPIOB
#define MPU6050_SDA_Pin GPIO_PIN_9
#define MPU6050_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
