/**
  ******************************************************************************
  * @file    control_state.c
  * @brief   Implementation of global control state management.
  ******************************************************************************
  */

#include "control_state.h"
#include "stm32f7xx_hal.h"

static volatile log_out_t g_out = LOG_OUT_UART; /**< Global logging output destination */
static volatile float g_sp_deg = 0.0f;           /**< Global control setpoint in degrees */

/**
  * @brief  Initializes the control state and log buffer.
  * @retval None
  */
void control_init(void)
{
	logbuf_init();
	g_out = LOG_OUT_UART;
	g_sp_deg = 0.0f;
}

/**
  * @brief  Sets the logging output to UART.
  * @retval None
  */
void control_set_out_uart(void){ g_out = LOG_OUT_UART; }

/**
  * @brief  Sets the logging output to Ethernet.
  * @retval None
  */
void control_set_out_eth(void){  g_out = LOG_OUT_ETH;  }

/**
  * @brief  Returns the current logging output destination.
  * @retval log_out_t: Current logging destination.
  */
log_out_t control_get_out(void){ return g_out; }

/**
  * @brief  Sets the control setpoint in degrees.
  * @param  deg: Desired setpoint in degrees.
  * @retval None
  */
void control_set_setpoint_deg(float deg){ g_sp_deg = deg; }

/**
  * @brief  Returns the current control setpoint in degrees.
  * @retval float: Current setpoint in degrees.
  */
float control_get_setpoint_deg(void){ return g_sp_deg; }
