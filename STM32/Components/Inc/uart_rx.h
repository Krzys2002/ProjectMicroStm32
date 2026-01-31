/*
 * uart_rx.h
 *
 *  Created on: Jan 31, 2026
 *      Author: krzysztofsawicki
 */

#ifndef INC_UART_RX_H_
#define INC_UART_RX_H_

#include "stm32f7xx_hal.h"

void uart_rx_dma_init(UART_HandleTypeDef *huart);
void UartRx();

#endif /* INC_UART_RX_H_ */
