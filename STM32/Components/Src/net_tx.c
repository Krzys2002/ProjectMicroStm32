/*
 * net_tx.c
 *
 *  Created on: Jan 30, 2026
 *      Author: krzysztofsawicki
 */

#include "net_tx.h"
#include "lwip/sockets.h"
#include "logbuf.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "control_state.h"

extern volatile int g_client_fd;


void NetTx(t)
{
  /* USER CODE BEGIN StartNetTxTask */
	extern volatile int g_client_fd;
  /* Infinite loop */
  for(;;)
  {
	  uint8_t out[512];

	      for (;;) {
	          if (control_get_out() != LOG_OUT_ETH) {
	              osDelay(50);
	              continue;
	          }

	          int c = g_client_fd;
	          if (c >= 0) {
	              size_t n = logbuf_read(out, sizeof(out));
	              if (n > 0) {
	                  int sent = send(c, out, (int)n, 0);
	                  if (sent <= 0) {
	                      // server task will close and reset g_client_fd
	                  }
	              } else {
	                  osDelay(10);
	              }
	          } else {
	              osDelay(50);
	          }
	      }
    osDelay(1);
  }
  /* USER CODE END StartNetTxTask */
}
