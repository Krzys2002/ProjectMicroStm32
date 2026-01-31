/*
 * control_state.c
 *
 *  Created on: Jan 31, 2026
 *      Author: krzysztofsawicki
 */

#include "control_state.h"
#include "stm32f7xx_hal.h"

static volatile log_out_t g_out = LOG_OUT_UART;
static volatile float g_sp_deg = 0.0f;

void control_init(void)
{
	logbuf_init();
	g_out = LOG_OUT_UART;
	g_sp_deg = 0.0f;
}

void control_set_out_uart(void){ g_out = LOG_OUT_UART; }
void control_set_out_eth(void){  g_out = LOG_OUT_ETH;  }
log_out_t control_get_out(void){ return g_out; }

void control_set_setpoint_deg(float deg){ g_sp_deg = deg; }
float control_get_setpoint_deg(void){ return g_sp_deg; }
