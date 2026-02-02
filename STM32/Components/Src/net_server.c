/**
  ******************************************************************************
  * @file    net_server.c
  * @brief   Implementation of the TCP network server for command handling.
  ******************************************************************************
  */

#include "net_server.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include <string.h>
#include <stdio.h>
#include "logbuf.h"
#include "FreeRTOS.h"

volatile int g_client_fd = -1; /**< Global file descriptor for the connected client socket */

#define TCP_PORT 5000 /**< Listening port for the TCP server */

void handle_line(const char *line); // from command_handler.c

/**
  * @brief  Main network server task.
  *         Listens for incoming TCP connections and processes received commands.
  * @param  argument: Task argument (unused).
  * @retval None
  */
void NetServer(void const * argument)
{
	// Wait a bit so LwIP finishes init (important!)
	osDelay(500);
	g_client_fd = -1;


	int srv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int opt = 1;
	setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TCP_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(srv, (struct sockaddr*)&addr, sizeof(addr));
	listen(srv, 1);

	for (;;) {
	  int c = accept(srv, NULL, NULL);
	  g_client_fd = c;

	  // simple line buffer
	  char inbuf[512];
	  int inlen = 0;

	  for (;;) {
		  char tmp[128];
		  int n = recv(c, tmp, sizeof(tmp), 0);
		  if (n <= 0) break; // disconnected

		  for (int i = 0; i < n; i++) {
			  char ch = tmp[i];
			  if (inlen < (int)sizeof(inbuf)-1) {
				  inbuf[inlen++] = ch;
			  }
			  if (ch == '\n') {
				  inbuf[inlen] = 0;
				  handle_line(inbuf);
				  inlen = 0;
			  }
		  }
		  osDelay(1);
	  }
	  close(c);
	  g_client_fd = -1;

	}

}
