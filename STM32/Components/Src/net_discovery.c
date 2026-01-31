/*
 * net_discovery.c
 *
 *  Created on: Jan 30, 2026
 *      Author: krzysztofsawicki
 */
#include "net_discovery.h"
#include "cmsis_os.h"
#include "lwip/sockets.h"
#include <string.h>
#include <stdio.h>

#define DISCOVERY_PORT 40000
#define TCP_PORT       5000
#define MY_NAME        "stm32-f767zi"

void NetDiscover()
{

  // Wait a bit so LwIP finishes init (important!)
  osDelay(500);

  int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (s < 0) {
    for(;;) osDelay(1000);
  }

  int opt = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(DISCOVERY_PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s, (struct sockaddr*)&addr, sizeof(addr));

  char rx[128];
  for (;;) {
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    int n = recvfrom(s, rx, sizeof(rx)-1, 0, (struct sockaddr*)&from, &fromlen);
    if (n <= 0) continue;
    rx[n] = 0;

    if (strstr(rx, "WHO_IS_STM32?") != NULL) {
      char reply[160];
      // Reply without hardcoding IP: PC already knows sender and can use that
      // But we include TCP port + name
      snprintf(reply, sizeof(reply), "STM32F767ZI;TCP=%d;NAME=%s\n", TCP_PORT, MY_NAME);
      sendto(s, reply, (int)strlen(reply), 0, (struct sockaddr*)&from, fromlen);
    }
  }
}
