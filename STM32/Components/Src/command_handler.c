/**
  ******************************************************************************
  * @file    command_handler.c
  * @brief   Implementation of JSON-based command processing.
  ******************************************************************************
  */

#include <string.h>
#include <stdlib.h>  // strtof
#include <stdio.h>
#include "control_state.h"

/**
  * @brief  Helper function to find a key in a JSON string.
  * @param  s: Pointer to the JSON string.
  * @param  key: The key to search for.
  * @retval const char*: Pointer to the value following the key, or NULL if not found.
  */
static const char* find_key(const char *s, const char *key)
{
    const char *p = strstr(s, key);
    if (!p) return NULL;
    return p + strlen(key);
}

/**
  * @brief  Parses and executes a command from a single line of text.
  * @param  line: Pointer to the line of text (JSON).
  * @retval None
  */
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
