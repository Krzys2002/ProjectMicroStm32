/*
 * pid_sample_rb.h
 *
 *  Created on: Jan 31, 2026
 *      Author: krzysztofsawicki
 */

#ifndef INC_PID_SAMPLE_RB_H_
#define INC_PID_SAMPLE_RB_H_

#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint32_t t_ms;
  float pos_deg;
  float spd_deg_s;
  float sp_deg;
  float out;       // controller output 0..1 (or PWM duty)
} pid_sample_t;

void pidrb_init(void);

// ISR-side push (fast, drops if full)
void pidrb_push_isr(const pid_sample_t *s);

// Task-side pop; returns 1 if got a sample
int pidrb_pop(pid_sample_t *out);

// Optional stats
uint32_t pidrb_dropped(void);

#endif /* INC_PID_SAMPLE_RB_H_ */
