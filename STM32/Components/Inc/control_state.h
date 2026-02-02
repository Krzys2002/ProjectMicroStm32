/**
  ******************************************************************************
  * @file    control_state.h
  * @brief   Management of global control states (setpoints and logging output).
  ******************************************************************************
  */

#ifndef INC_CONTROL_STATE_H_
#define INC_CONTROL_STATE_H_

#include <stdint.h>

/**
  * @brief Logging output destination enumeration.
  */
typedef enum { 
  LOG_OUT_UART = 0, /**< Log to UART */
  LOG_OUT_ETH = 1   /**< Log to Ethernet */
} log_out_t;

/**
  * @brief  Initializes the control state.
  * @retval None
  */
void control_init(void);

/**
  * @brief  Sets the logging output to UART.
  * @retval None
  */
void control_set_out_uart(void);

/**
  * @brief  Sets the logging output to Ethernet.
  * @retval None
  */
void control_set_out_eth(void);

/**
  * @brief  Returns the current logging output destination.
  * @retval log_out_t: Current logging destination.
  */
log_out_t control_get_out(void);

/**
  * @brief  Sets the control setpoint in degrees.
  * @param  deg: Desired setpoint in degrees.
  * @retval None
  */
void control_set_setpoint_deg(float deg);

/**
  * @brief  Returns the current control setpoint in degrees.
  * @retval float: Current setpoint in degrees.
  */
float control_get_setpoint_deg(void);

#endif /* INC_CONTROL_STATE_H_ */
