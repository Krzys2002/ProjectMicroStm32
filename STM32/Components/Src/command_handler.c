/*
 * command_handler.c
 *
 *  Created on: Jan 31, 2026
 *      Author: krzysztofsawicki
 */

#include <string.h>
#include <stdlib.h>  // strtof
#include <stdio.h>
#include "control_state.h"

static const char* find_key(const char *s, const char *key)
{
    const char *p = strstr(s, key);
    if (!p) return NULL;
    return p + strlen(key);
}

void handle_line(const char *line)
{
    if (!line || !line[0]) return;

    // cmd out
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

    // cmd setpoint
    if (strstr(line, "\"cmd\":\"setpoint\"")) {
        const char *p = find_key(line, "\"deg\":");
        if (p) {
            float deg = strtof(p, NULL);   // expects number right after "deg":
            control_set_setpoint_deg(deg);

            // Avoid float snprintf if you want speed:
            // send scaled integer (centi-deg)
            int deg_c = (int)(deg * 100.0f);
            char ack[96];
            snprintf(ack, sizeof(ack),
                     "{\"type\":\"ack\",\"setpoint_cdeg\":%d}\n", deg_c);
            log_json(ack);
            return;
        }
    }

    log_json("{\"type\":\"err\",\"msg\":\"unknown_cmd\"}\n");
}
