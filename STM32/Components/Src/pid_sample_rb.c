/**
  ******************************************************************************
  * @file    pid_sample_rb.c
  * @brief   Implementation of the PID sample ring buffer.
  ******************************************************************************
  */

#include "pid_sample_rb.h"
#include "stm32f7xx_hal.h"

#define PIDRB_N 128  /**< Size of the PID sample ring buffer */

static pid_sample_t rb[PIDRB_N];      /**< Ring buffer storage */
static volatile uint16_t w = 0, r = 0; /**< Write and read indices */
static volatile uint32_t dropped = 0;   /**< Counter for dropped samples */

/**
  * @brief  Initializes the PID sample ring buffer.
  * @retval None
  */
void pidrb_init(void) { w = r = 0; dropped = 0; }

/**
  * @brief  Calculates the next index in the ring buffer.
  * @param  i: Current index.
  * @retval uint16_t: Next index.
  */
static uint16_t next_i(uint16_t i){ return (uint16_t)((i + 1U) % PIDRB_N); }

/**
  * @brief  Pushes a sample into the ring buffer from an ISR.
  * @note   Fast operation, increments dropped counter if full.
  * @param  s: Pointer to the sample to push.
  * @retval None
  */
void pidrb_push_isr(const pid_sample_t *s)
{
  uint16_t nw = next_i(w);
  if (nw == r) { dropped++; return; }  // full
  rb[w] = *s;
  w = nw;
}

/**
  * @brief  Pops a sample from the ring buffer.
  * @note   Disables interrupts to ensure atomicity.
  * @param  out: Pointer to the structure where the sample will be stored.
  * @retval int: 1 if successful, 0 if buffer is empty.
  */
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

/**
  * @brief  Returns the number of dropped samples.
  * @retval uint32_t: Dropped sample count.
  */
uint32_t pidrb_dropped(void) { return dropped; }
