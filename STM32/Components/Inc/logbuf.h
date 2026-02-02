/**
  ******************************************************************************
  * @file    logbuf.h
  * @brief   Thread-safe log buffer implementation.
  ******************************************************************************
  */

#ifndef INC_LOGBUF_H_
#define INC_LOGBUF_H_

#include <stdint.h>
#include <stddef.h>

/**
  * @brief  Initializes the log buffer.
  * @retval None
  */
void logbuf_init(void);

/**
  * @brief  Writes data to the log buffer.
  * @note   This function is ISR-safe.
  * @param  data: Pointer to the data to write.
  * @param  len: Length of the data in bytes.
  * @retval size_t: Number of bytes actually written.
  */
size_t logbuf_write(const uint8_t *data, size_t len);

/**
  * @brief  Reads data from the log buffer.
  * @param  out: Pointer to the buffer where data will be stored.
  * @param  maxlen: Maximum number of bytes to read.
  * @retval size_t: Number of bytes actually read.
  */
size_t logbuf_read(uint8_t *out, size_t maxlen);

/**
  * @brief  Returns the number of bytes available in the log buffer.
  * @retval size_t: Number of available bytes.
  */
size_t logbuf_available(void);

#endif /* INC_LOGBUF_H_ */
