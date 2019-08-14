/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "usart.h"
#include "BY_log.h"
#include "application.h"
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
osThreadId taskconfigHandle;
osThreadId TaskU2_RVHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void TaskConfig(void const * argument);
void u2_rv(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
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
  /* definition and creation of taskconfig */
  osThreadDef(taskconfig, TaskConfig, osPriorityNormal, 0, 128);
  taskconfigHandle = osThreadCreate(osThread(taskconfig), NULL);

  /* definition and creation of TaskU2_RV */
  osThreadDef(TaskU2_RV, u2_rv, osPriorityNormal, 0, 128);
  TaskU2_RVHandle = osThreadCreate(osThread(TaskU2_RV), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_TaskConfig */
/**
  * @brief  Function implementing the taskconfig thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_TaskConfig */
void TaskConfig(void const * argument)
{

  /* USER CODE BEGIN TaskConfig */
	command_buffer_Queue = xQueueCreate(command_buffer_Queue_LEN, sizeof(uint8_t *));
  /* Infinite loop */
  for(;;)
  {
	 Command_msg_handler();
     osDelay(10);
  }
  /* USER CODE END TaskConfig */
}

/* USER CODE BEGIN Header_u2_rv */
/**
* @brief Function implementing the TaskU2_RV thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_u2_rv */
void u2_rv(void const * argument)
{
  /* USER CODE BEGIN u2_rv */
	uint8_t len;
  /* Infinite loop */
  for(;;)
  {
	  if(USART_RX_STA&0x8000)
	{					   
		len=USART_RX_STA&0x3fff;
		HAL_UART_Transmit(&huart2,(uint8_t*)USART_RX_BUF,len,1000);	
		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)!=SET);
		if(!command_msg_analysis(USART_RX_BUF,len)){
			log_debug("command handle success");
		
		}else{
			log_debug("command handle failure");

		}
		USART_RX_STA=0;
	}
    osDelay(1);
  }
  /* USER CODE END u2_rv */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
