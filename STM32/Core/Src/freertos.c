/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "net_discovery.h"
#include "net_server.h"
#include "net_tx.h"
#include "uart_tx.h"
#include "uart_rx.h"
#include "telemetry.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId NetDiscoverTaskHandle;
osThreadId NetServerTaskHandle;
osThreadId NetTxTaskHandle;
osThreadId UartTxTaskHandle;
osThreadId TelemetryTaskHandle;
osThreadId UartRxTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartNetDiscoverTask(void const * argument);
void StartNetServerTask(void const * argument);
void StartNetTxTask(void const * argument);
void StartUartTxTask(void const * argument);
void StartTelemetryTask(void const * argument);
void StartUartRxTask(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	log_router_init();
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of NetDiscoverTask */
  osThreadDef(NetDiscoverTask, StartNetDiscoverTask, osPriorityIdle, 0, 256);
  NetDiscoverTaskHandle = osThreadCreate(osThread(NetDiscoverTask), NULL);

  /* definition and creation of NetServerTask */
  osThreadDef(NetServerTask, StartNetServerTask, osPriorityIdle, 0, 256);
  NetServerTaskHandle = osThreadCreate(osThread(NetServerTask), NULL);

  /* definition and creation of NetTxTask */
  osThreadDef(NetTxTask, StartNetTxTask, osPriorityIdle, 0, 256);
  NetTxTaskHandle = osThreadCreate(osThread(NetTxTask), NULL);

  /* definition and creation of UartTxTask */
  osThreadDef(UartTxTask, StartUartTxTask, osPriorityIdle, 0, 256);
  UartTxTaskHandle = osThreadCreate(osThread(UartTxTask), NULL);

  /* definition and creation of TelemetryTask */
  osThreadDef(TelemetryTask, StartTelemetryTask, osPriorityBelowNormal, 0, 256);
  TelemetryTaskHandle = osThreadCreate(osThread(TelemetryTask), NULL);

  /* definition and creation of UartRxTask */
  osThreadDef(UartRxTask, StartUartRxTask, osPriorityNormal, 0, 256);
  UartRxTaskHandle = osThreadCreate(osThread(UartRxTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  char buf[64];
  int i = 0;

  for(;;)
  {
	  snprintf(buf, sizeof(buf), "{\"i\": %d}", i);
	  log_json(buf);
	  i++;
	  osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartNetDiscoverTask */
/**
* @brief Function implementing the NetDiscoverTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartNetDiscoverTask */
void StartNetDiscoverTask(void const * argument)
{
  /* USER CODE BEGIN StartNetDiscoverTask */
  /* Infinite loop */
  NetDiscover();
  /* USER CODE END StartNetDiscoverTask */
}

/* USER CODE BEGIN Header_StartNetServerTask */
/**
* @brief Function implementing the NetServerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartNetServerTask */
void StartNetServerTask(void const * argument)
{
  /* USER CODE BEGIN StartNetServerTask */
  /* Infinite loop */
  NetServer();
  /* USER CODE END StartNetServerTask */
}

/* USER CODE BEGIN Header_StartNetTxTask */
/**
* @brief Function implementing the NetTxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartNetTxTask */
void StartNetTxTask(void const * argument)
{
  /* USER CODE BEGIN StartNetTxTask */
  /* Infinite loop */
  NetTx();
  /* USER CODE END StartNetTxTask */
}

/* USER CODE BEGIN Header_StartUartTxTask */
/**
* @brief Function implementing the UartTxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartTxTask */
void StartUartTxTask(void const * argument)
{
  /* USER CODE BEGIN StartUartTxTask */
  /* Infinite loop */
  UartTx();
  /* USER CODE END StartUartTxTask */
}

/* USER CODE BEGIN Header_StartTelemetryTask */
/**
* @brief Function implementing the TelemetryTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTelemetryTask */
void StartTelemetryTask(void const * argument)
{
  /* USER CODE BEGIN StartTelemetryTask */
  /* Infinite loop */
	Telemetry();
  /* USER CODE END StartTelemetryTask */
}

/* USER CODE BEGIN Header_StartUartRxTask */
/**
* @brief Function implementing the UartRxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartRxTask */
void StartUartRxTask(void const * argument)
{
  /* USER CODE BEGIN StartUartRxTask */
  /* Infinite loop */
	UartRx();
  /* USER CODE END StartUartRxTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

