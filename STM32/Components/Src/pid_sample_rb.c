/*
 * pid_sample_rb.c
 *
 *  Created on: Jan 31, 2026
 *      Author: krzysztofsawicki
 */

#include "pid_sample_rb.h"
#include "stm32f7xx_hal.h"

#define PIDRB_N 128  // power-of-two not required, but keep it small

static pid_sample_t rb[PIDRB_N];
static volatile uint16_t w = 0, r = 0;
static volatile uint32_t dropped = 0;

void pidrb_init(void) { w = r = 0; dropped = 0; }

static uint16_t next_i(uint16_t i){ return (uint16_t)((i + 1U) % PIDRB_N); }

void pidrb_push_isr(const pid_sample_t *s)
{
  uint16_t nw = next_i(w);
  if (nw == r) { dropped++; return; }  // full
  rb[w] = *s;
  w = nw;
}

int pidrb_pop(pid_sample_t *out)
{
  int ok = 0;
  __disable_irq();
  if (r != w) {
    *out = rb[r];
    r = next_i(r);
    ok = 1;
  }
  __enable_irq();
  return ok;
}

uint32_t pidrb_dropped(void) { return dropped; }
