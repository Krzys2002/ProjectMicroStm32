/**
  ******************************************************************************
  * @file    motor_control.h
  * @brief   Header for motor_control.c module.
  * This file contains the common defines and function prototypes
  * for the PID-based position controller.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported constants --------------------------------------------------------*/

/** @defgroup MOTOR_CONFIG Motor Configuration Constants
  * @{
  */
#define PULSES_PER_REV    20.0f    /**< Encoder pulses per full rotation */
#define PID_FREQUENCY_HZ  1000.0f  /**< Control loop frequency in Hz */
#define MAX_OUT           1000.0f  /**< Maximum PWM duty cycle (CCR value) */
/**
  * @}
  */

/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Initializes the PID instance, ring buffer, and PWM peripherals.
  * @retval None
  */
void MotorControl_Init(void);

/**
  * @brief  Execution step for the motor control loop.
  * @note   Must be called at a fixed frequency (typically from TIM7 ISR).
  * @retval None
  */
void MotorControl_Step(void);

#ifdef __cplusplus
}
#endif

#endif /* MOTOR_CONTROL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
