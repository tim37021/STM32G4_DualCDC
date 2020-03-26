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
#include "spi.h"
#include "main.h"
#include "protocol.h"
#include "protocol_spi.h"
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
osStreamId_t usbInputStream;


osThreadId_t spiIOHandle;
const osThreadAttr_t spiIOTask_attributes = {
        .name = "spi",
        .priority = (osPriority_t) osPriorityRealtime,
        .stack_size = 128 * 4
};

void SpiIOWorker(void const *);

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
  usbInputStream = osStreamNew(64);
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
  spiIOHandle = osThreadNew(SpiIOWorker, NULL, &spiIOTask_attributes);
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

  extern osStreamId_t usbInputStream;
  USBD_DCDC_HandleTypeDef *hcdc = (USBD_DCDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *)buf, sizeof(buf)/sizeof(uint16_t));

  uint8_t rx_buf[4]="DICK";
  uint8_t usb_tx[8];
  uint32_t len;
  /* Infinite loop */
  for(;;)
  {
    if (len = osStreamGet(usbInputStream, rx_buf, 1, osWaitForever)) {
        //CDC_Transmit_FS(&hcdc->CDC1, "fuck\n", 5);
        //HAL_SPI_Receive_DMA(&hspi2, rx_buf, 4);
        //while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
        switch(rx_buf[0]) {
            case OP_RETURN:
                // not necessary
                break;
            case OP_KNOCK:
                usb_tx[0] = OP_RETURN; usb_tx[1]=OP_KNOCK;
                CDC_Transmit_FS(&hcdc->CDC2, usb_tx, 2);
                break;
            case OP_MODE:
                usb_tx[0] = OP_RETURN; usb_tx[1]=APP;
                CDC_Transmit_FS(&hcdc->CDC2, usb_tx, 2);
                break;
            case OP_REBOOT:
                break;
            case OP_REBOOT_DFU:
                break;

        }

    }
    /*
    
    HAL_GPIO_WritePin(SOFT_NSS_GPIO_Port, SOFT_NSS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_DMA(&hspi1, "fuck", 4);
    while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
    HAL_GPIO_WritePin(SOFT_NSS_GPIO_Port, SOFT_NSS_Pin, GPIO_PIN_SET);
    
    osDelay(1000);
    */
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static uint8_t spi2_dma_rx_buf[CMD_SIZE] = "";
static uint32_t rx_len = CMD_SIZE;
static uint8_t spi2_dma_tx_buf[CMD_SIZE] = "ggg";

static void print_cmd_hex(const char *str, const uint8_t *arr)
{
    USBD_DCDC_HandleTypeDef *hcdc = (USBD_DCDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
    CDC_Transmit_FS(&hcdc->CDC2, str, strlen(str));
}

void SpiIOWorker(void const * argument)
{
    osDelay(4000);
    init_chip_stream();
    memset(spi2_dma_rx_buf, 0, CMD_SIZE);

    uint8_t tx_buf[CMD_SIZE] = "";
    uint8_t rx_buf[CMD_SIZE] = "";
    uint32_t len;
    HAL_SPI_TransmitReceive_DMA(&hspi2, spi2_dma_tx_buf, spi2_dma_rx_buf, rx_len);
    /* Infinite loop */
    for(;;)
    {
        if (len = osStreamGet(chip_output_stream, tx_buf, CMD_SIZE, 1)) {
            HAL_GPIO_WritePin(SOFT_NSS_GPIO_Port, SOFT_NSS_Pin, GPIO_PIN_RESET);
            //HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
            memset(rx_buf, 0, CMD_SIZE);
            HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buf, rx_buf, CMD_SIZE);
            print_cmd_hex("spi transmit data: ", tx_buf);
            while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
            HAL_GPIO_WritePin(SOFT_NSS_GPIO_Port, SOFT_NSS_Pin, GPIO_PIN_SET);

        }

        osDelay(1);
    }
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi == &hspi2)
    {
        //osStreamPut(chip_input_stream, spi2_dma_rx_buf, rx_len, 1);
        print_cmd_hex("spi receive data: ", spi2_dma_rx_buf);
        parse_chip_received_cmd(spi2_dma_rx_buf);
        memset(spi2_dma_rx_buf, 0, CMD_SIZE);
    }
}


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
