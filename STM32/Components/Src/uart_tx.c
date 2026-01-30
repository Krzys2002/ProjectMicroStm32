/*
 * uart_tx.c
 *
 *  Created on: Jan 30, 2026
 *      Author: krzysztofsawicki
 */

#include "cmsis_os.h"
#include "stm32f7xx_hal.h"
#include "logbuf.h"
#include "log_router.h"
#include "uart_tx.h"

extern UART_HandleTypeDef huart3;   // change to your UART
static volatile int uart_busy = 0;
static uint8_t uart_tx_buf[256];

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart3) {
        uart_busy = 0;
    }
}

void StartUartTxTask(void const * argument)
{
    for (;;) {
        if (log_get_output() != LOG_OUT_UART) {
            osDelay(50);
            continue;
        }

        if (!uart_busy) {
            size_t n = logbuf_read(uart_tx_buf, sizeof(uart_tx_buf));
            if (n > 0) {
                uart_busy = 1;
                if (HAL_UART_Transmit_DMA(&huart3, uart_tx_buf, (uint16_t)n) != HAL_OK) {
                    uart_busy = 0; // try again later
                }
            } else {
                osDelay(10);
            }
        } else {
            osDelay(1);
        }
    }
}
