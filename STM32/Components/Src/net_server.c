/*
 * net_server.h
 *
 *  Created on: Jan 30, 2026
 *      Author: krzysztofsawicki
 */

#include "net_server.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include <string.h>
#include <stdio.h>
#include "logbuf.h"
#include "FreeRTOS.h"

// Expose client socket to TX task
volatile int g_client_fd = -1;

#define TCP_PORT 5000

static void handle_line(const char *line)
{
    if (strstr(line, "\"cmd\":\"set_out\"") && strstr(line, "\"value\":\"ETH\"")) {
        log_set_eth();
        log_json("{\"type\":\"info\",\"msg\":\"output=ETH\"}\n");
    }
    if (strstr(line, "\"cmd\":\"set_out\"") && strstr(line, "\"value\":\"UART\"")) {
        log_set_uart();
        log_json("{\"type\":\"info\",\"msg\":\"output=UART\"}\n");
    }
}

void StartNetServerTask(void const * argument)
{
	(void)argument;

	// Wait a bit so LwIP finishes init (important!)
	osDelay(500);
	volatile int g_client_fd = -1;


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
