/*
 * log_router.c
 *
 *  Created on: Jan 30, 2026
 *      Author: krzysztofsawicki
 */

#include "log_router.h"
#include "logbuf.h"
#include "stm32f7xx_hal.h"
#include <string.h>

static volatile log_out_t g_out = LOG_OUT_UART;

void log_router_init(void)
{
    logbuf_init();
    g_out = LOG_OUT_UART;
}

void log_set_uart(void) { g_out = LOG_OUT_UART; }
void log_set_eth(void)  { g_out = LOG_OUT_ETH;  }
log_out_t log_get_output(void) { return g_out; }

// Enqueue only (ISR-safe). Add newline if missing.
void log_json(const char *json_line)
{
    if (!json_line) return;
    size_t len = strlen(json_line);
    logbuf_write((const uint8_t*)json_line, len);

    if (len == 0 || json_line[len-1] != '\n') {
        const char nl = '\n';
        logbuf_write((const uint8_t*)&nl, 1);
    }
}
