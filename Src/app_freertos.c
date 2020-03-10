/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "usbd_cdc_if.h"
#include "sai.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FRAME_SIZE 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
// 8 channel, buffer size, double buffer
uint16_t buf[8*FRAME_SIZE*2];
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

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
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  osDelay(4000);
  
  USBD_DCDC_HandleTypeDef *hcdc = (USBD_DCDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *)buf, sizeof(buf)/sizeof(uint16_t));

  /* Infinite loop */
  for(;;)
  {
    //CDC_Transmit_FS(&hcdc->CDC1, "fuck\n", 5);
    CDC_Transmit_FS(&hcdc->CDC2, "ggin\n", 5);
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
extern USBD_HandleTypeDef hUsbDeviceFS;
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SAI_RxHalfCpltCallback could be implemented in the user file
   */

  USBD_DCDC_HandleTypeDef *hcdc = (USBD_DCDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  volatile int ret = CDC_Transmit_FS(&hcdc->CDC1, (uint8_t *)buf, 8*FRAME_SIZE*sizeof(uint16_t));
  ret *= 1;
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SAI_RxCpltCallback could be implemented in the user file
   */

  USBD_DCDC_HandleTypeDef *hcdc = (USBD_DCDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  volatile int ret = CDC_Transmit_FS(&hcdc->CDC1, (uint8_t *)(buf+8*FRAME_SIZE), 8*FRAME_SIZE*sizeof(uint16_t));
  ret *= 1;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
