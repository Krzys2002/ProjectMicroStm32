/**
  ******************************************************************************
  * @file    pid_sample_rb.h
  * @brief   Ring buffer for PID telemetry samples.
  ******************************************************************************
  */

#ifndef INC_PID_SAMPLE_RB_H_
#define INC_PID_SAMPLE_RB_H_

#include <stdint.h>
#include <stddef.h>

/**
  * @brief PID sample structure for telemetry.
  */
typedef struct {
  uint32_t t_ms;      /**< Timestamp in milliseconds */
  float pos_deg;      /**< Current position in degrees */
  float spd_deg_s;    /**< Current speed in degrees per second */
  float sp_deg;       /**< Current setpoint in degrees */
  float out;          /**< Controller output (PWM duty or normalized) */
} pid_sample_t;

/**
  * @brief  Initializes the PID sample ring buffer.
  * @retval None
  */
void pidrb_init(void);

/**
  * @brief  Pushes a new sample into the ring buffer from an ISR.
  * @note   Fast operation, drops the sample if the buffer is full.
  * @param  s: Pointer to the PID sample to push.
  * @retval None
  */
void pidrb_push_isr(const pid_sample_t *s);

/**
  * @brief  Pops a sample from the ring buffer (Task-side).
  * @param  out: Pointer to a structure where the popped sample will be stored.
  * @retval int: 1 if a sample was successfully popped, 0 otherwise.
  */
int pidrb_pop(pid_sample_t *out);

/**
  * @brief  Returns the number of dropped samples due to a full buffer.
  * @retval uint32_t: Number of dropped samples.
  */
uint32_t pidrb_dropped(void);

#endif /* INC_PID_SAMPLE_RB_H_ */
