/**
  ******************************************************************************
  * @file    telemetry.c
  * @brief   Implementation of the telemetry task.
  ******************************************************************************
  */

#include "telemetry.h"
#include "cmsis_os.h"
#include "pid_sample_rb.h"
#include "control_state.h"
#include "log_router.h"   // your log_json(...)
#include <stdio.h>

/**
  * @brief  Main telemetry task.
  *         Pops samples from the PID ring buffer, formats them as JSON,
  *         and sends them to the log router.
  * @retval None
  */
void Telemetry()
{
  // Wait until LwIP ready if you use ENT; safe anyway
  osDelay(500);

  pid_sample_t s;
  char line[192];

  for (;;) {
    if (pidrb_pop(&s)) {
      int n = snprintf(line, sizeof(line),
        "{\"type\":\"pid\",\"t\":%lu,\"pos\":%.2f,\"spd\":%.2f,\"sp\":%.2f,\"out\":%.3f}\n",
        (unsigned long)s.t_ms, s.pos_deg, s.spd_deg_s, s.sp_deg, s.out);

      if (n > 0) log_json(line);
    } else {
      osDelay(50);
    }
  }
}
