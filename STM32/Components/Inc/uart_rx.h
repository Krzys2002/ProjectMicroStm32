/**
  ******************************************************************************
  * @file    uart_rx.h
  * @brief   UART DMA receiver module.
  ******************************************************************************
  */

#ifndef INC_UART_RX_H_
#define INC_UART_RX_H_

#include "stm32f7xx_hal.h"

/**
  * @brief  Initializes UART DMA reception.
  * @param  huart: Pointer to UART handle.
  * @retval None
  */
void uart_rx_dma_init(UART_HandleTypeDef *huart);

/**
  * @brief  Main task or callback handler for processing received UART data.
  * @retval None
  */
void UartRx();

#endif /* INC_UART_RX_H_ */
