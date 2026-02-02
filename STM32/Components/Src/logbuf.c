/**
  ******************************************************************************
  * @file    logbuf.c
  * @brief   Implementation of a thread-safe log buffer.
  ******************************************************************************
  */

#include "logbuf.h"
#include "stm32f7xx_hal.h"

#define LOGBUF_SIZE 8192 /**< Size of the circular log buffer in bytes */

static volatile uint16_t w = 0;   /**< Write index */
static volatile uint16_t r = 0;   /**< Read index */
static uint8_t buf[LOGBUF_SIZE]; /**< Circular buffer storage */

/**
  * @brief  Calculates the next index in the circular buffer.
  * @param  i: Current index.
  * @retval uint16_t: Next index.
  */
static uint16_t next_idx(uint16_t i){ return (uint16_t)((i + 1U) % LOGBUF_SIZE); }

/**
  * @brief  Initializes the log buffer by resetting read and write indices.
  * @retval None
  */
void logbuf_init(void){ w = r = 0; }

/**
  * @brief  Writes data to the log buffer.
  * @note   Disables interrupts to ensure atomicity. ISR-safe.
  * @param  data: Pointer to the data to write.
  * @param  len: Length of the data in bytes.
  * @retval size_t: Number of bytes actually written.
  */
size_t logbuf_write(const uint8_t *data, size_t len)
{
    size_t written = 0;
    __disable_irq();
    while (written < len) {
        uint16_t nw = next_idx(w);
        if (nw == r) break; // full
        buf[w] = data[written++];
        w = nw;
    }
    __enable_irq();
    return written;
}

/**
  * @brief  Returns the number of bytes currently available in the log buffer.
  * @retval size_t: Number of available bytes.
  */
size_t logbuf_available(void)
{
    __disable_irq();
    uint16_t ww = w, rr = r;
    __enable_irq();
    if (ww >= rr) return (size_t)(ww - rr);
    return (size_t)(LOGBUF_SIZE - rr + ww);
}

/**
  * @brief  Reads data from the log buffer into a user-provided buffer.
  * @note   Disables interrupts to ensure atomicity.
  * @param  out: Pointer to the buffer where data will be stored.
  * @param  maxlen: Maximum number of bytes to read.
  * @retval size_t: Number of bytes actually read.
  */
size_t logbuf_read(uint8_t *out, size_t maxlen)
{
    size_t n = 0;
    __disable_irq();
    while (n < maxlen && r != w) {
        out[n++] = buf[r];
        r = next_idx(r);
    }
    __enable_irq();
    return n;
}

