/*
 * uart_rx.c
 *
 *  Created on: Jan 31, 2026
 *      Author: krzysztofsawicki
 */

#include "uart_rx.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdint.h>

extern void handle_line(const char *line);

#define RX_DMA_BUF_SZ 512
static uint8_t rx_dma_buf[RX_DMA_BUF_SZ];

static UART_HandleTypeDef *g_huart = NULL;
static volatile uint16_t last_pos = 0;

static void process_new_bytes(uint16_t pos_now)
{
  // Pull bytes from last_pos up to pos_now (with wrap)
  static char line[256];
  static uint16_t line_len = 0;

  while (last_pos != pos_now) {
    uint8_t b = rx_dma_buf[last_pos];
    last_pos = (uint16_t)((last_pos + 1U) % RX_DMA_BUF_SZ);

    if (b == '\r') continue;

    if (b == '\n') {
      line[line_len] = 0;
      if (line_len > 0) handle_line(line);
      line_len = 0;
    } else {
      if (line_len < sizeof(line) - 1) {
        line[line_len++] = (char)b;
      } else {
        // line too long -> drop/reset
        line_len = 0;
      }
    }
  }
}

void uart_rx_dma_init(UART_HandleTypeDef *huart)
{
  g_huart = huart;
  last_pos = 0;

  // Start circular DMA
  HAL_UART_Receive_DMA(g_huart, rx_dma_buf, RX_DMA_BUF_SZ);
}



// Task fallback (polling) in case you don’t want IDLE interrupt
void UartRx()
{
  for (;;) {
    if (g_huart) {
      uint16_t pos_now = (uint16_t)(RX_DMA_BUF_SZ - __HAL_DMA_GET_COUNTER(g_huart->hdmarx));
      process_new_bytes(pos_now);
    }
    osDelay(10);
  }
}
