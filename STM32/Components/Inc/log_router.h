/*
 * log_router.h
 *
 *  Created on: Jan 30, 2026
 *      Author: krzysztofsawicki
 */

#ifndef INC_LOG_ROUTER_H_
#define INC_LOG_ROUTER_H_

typedef enum {
    LOG_OUT_UART = 1,   // SP
    LOG_OUT_ETH  = 0    // ETN
} log_out_t;

void log_router_init(void);

// Switch output
void log_set_uart(void); // SP
void log_set_eth(void);  // ETN
log_out_t log_get_output(void);

// Main API: call from anywhere
void log_json(const char *json_line);

#endif /* INC_LOG_ROUTER_H_ */
