/**
  ******************************************************************************
  * @file    log_router.c
  * @brief   Implementation of the log routing system.
  ******************************************************************************
  */

#include "control_state.h"
#include "logbuf.h"
#include "stm32f7xx_hal.h"
#include <string.h>

static volatile log_out_t g_out = LOG_OUT_UART; /**< Local copy of the output destination */

/**
  * @brief  Initializes the log router.
  * @retval None
  */
void log_router_init(void)
{
    logbuf_init();
    g_out = LOG_OUT_UART;
}

/**
  * @brief  Sets the logging destination to UART.
  * @retval None
  */
void log_set_uart(void) { g_out = LOG_OUT_UART; }

/**
  * @brief  Sets the logging destination to Ethernet.
  * @retval None
  */
void log_set_eth(void)  { g_out = LOG_OUT_ETH;  }

/**
  * @brief  Returns the current logging destination.
  * @retval log_out_t: Current destination.
  */
log_out_t log_get_output(void) { return g_out; }

/**
  * @brief  Enqueues a JSON string into the log buffer.
  * @note   This function is ISR-safe. It ensures the line ends with a newline.
  * @param  json_line: Pointer to the JSON string.
  * @retval None
  */
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
