/**
  ******************************************************************************
  * @file    log_router.h
  * @brief   Main API for routing JSON logs to communication interfaces.
  ******************************************************************************
  */

#ifndef INC_LOG_ROUTER_H_
#define INC_LOG_ROUTER_H_

/**
  * @brief  Routes a JSON formatted string to the logging backend.
  * @param  json_line: Pointer to the JSON string to be logged.
  * @retval None
  */
void log_json(const char *json_line);

#endif /* INC_LOG_ROUTER_H_ */
