#pragma once
#include <stdint.h>
#include <stddef.h>

void logbuf_init(void);
size_t logbuf_write(const uint8_t *data, size_t len); // ISR-safe
size_t logbuf_read(uint8_t *out, size_t maxlen);      // task
size_t logbuf_available(void);
