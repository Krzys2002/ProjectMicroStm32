/*
 * logbuf.c
 *
 *  Created on: Jan 30, 2026
 *      Author: krzysztofsawicki
 */
#include "logbuf.h"
#include "stm32f7xx_hal.h"

#define LOGBUF_SIZE 8192

static volatile uint16_t w = 0;
static volatile uint16_t r = 0;
static uint8_t buf[LOGBUF_SIZE];

static uint16_t next_idx(uint16_t i){ return (uint16_t)((i + 1U) % LOGBUF_SIZE); }

void logbuf_init(void){ w = r = 0; }

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

size_t logbuf_available(void)
{
    __disable_irq();
    uint16_t ww = w, rr = r;
    __enable_irq();
    if (ww >= rr) return (size_t)(ww - rr);
    return (size_t)(LOGBUF_SIZE - rr + ww);
}

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

