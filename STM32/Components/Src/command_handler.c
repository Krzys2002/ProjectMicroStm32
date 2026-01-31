/*
 * command_handler.c
 *
 *  Created on: Jan 31, 2026
 *      Author: krzysztofsawicki
 */

#include "control_state.h"
#include "log_router.h"
#include <string.h>
#include <stdio.h>

void handle_line(const char *line)
{
  if (!line || !line[0]) return;

  // Output switch
  if (strstr(line, "\"cmd\":\"out\"")) {
    if (strstr(line, "\"value\":\"UART\"")) {
      control_set_out_uart();
      log_json("{\"type\":\"ack\",\"out\":\"UART\"}\n");
      return;
    }
    if (strstr(line, "\"value\":\"ETH\"")) {
      control_set_out_eth();
      log_json("{\"type\":\"ack\",\"out\":\"ETH\"}\n");
      return;
    }
  }

  // Setpoint: {"cmd":"setpoint","deg":123.4}
  if (strstr(line, "\"cmd\":\"setpoint\"")) {
    float deg = 0.0f;
    if (sscanf(line, "%*[^0-9.-]%f", &deg) == 1) {
      control_set_setpoint_deg(deg);
      char ack[96];
      snprintf(ack, sizeof(ack), "{\"type\":\"ack\",\"setpoint\":%.2f}\n", deg);
      log_json(ack);
      return;
    }
  }

  // Unknown command
  log_json("{\"type\":\"err\",\"msg\":\"unknown_cmd\"}\n");
}
