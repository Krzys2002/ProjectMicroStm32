/*
 * control_state.h
 *
 *  Created on: Jan 31, 2026
 *      Author: krzysztofsawicki
 */

#ifndef INC_CONTROL_STATE_H_
#define INC_CONTROL_STATE_H_

#include <stdint.h>

typedef enum { LOG_OUT_UART = 0, LOG_OUT_ETH = 1 } log_out_t;

void control_init(void);

void control_set_out_uart(void);  // SP
void control_set_out_eth(void);   // ENT
log_out_t control_get_out(void);

void control_set_setpoint_deg(float deg);
float control_get_setpoint_deg(void);

#endif /* INC_CONTROL_STATE_H_ */
