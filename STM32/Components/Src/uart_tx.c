/**
  ******************************************************************************
  * @file    uart_tx.c
  * @brief   Implementation of the UART transmission task.
  ******************************************************************************
  */

#include "cmsis_os.h"
#include "stm32f7xx_hal.h"
#include "logbuf.h"
#include "control_state.h"
#include "uart_tx.h"

extern UART_HandleTypeDef huart3;   /**< UART handle used for transmission */
static volatile int uart_busy = 0;   /**< Flag indicating UART transmission is in progress */
static uint8_t uart_tx_buf[256];    /**< Local buffer for UART transmission */

/**
  * @brief  UART Tx Transfer completed callback.
  * @param  huart: Pointer to UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart3) {
        uart_busy = 0;
    }
}

/**
  * @brief  Main UART transmission task.
  *         Reads data from log buffer and transmits it via UART DMA.
  * @retval None
  */
void UartTx()
{
    for (;;) {
        if (control_get_out() != LOG_OUT_UART) {
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
                osDelay(1);
            }
        } else {
            osDelay(1);
        }
    }
}
