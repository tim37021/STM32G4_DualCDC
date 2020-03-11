/**
  ******************************************************************************
  * @file    usbd_DCDC.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_DCDC.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DCDC_H
#define __USB_DCDC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup usbd_DCDC
  * @brief This file is the Header file for usbd_DCDC.c
  * @{
  */


/** @defgroup usbd_DCDC_Exported_Defines
  * @{
  */
#define CMCDC_IN_EP                                 0x81U  /* EP1 for data IN */
#define CMCDC_OUT_EP                                0x01U  /* EP1 for data OUT */
#define CMCDC_CMD_EP                                0x83U  /* EP2 for DCDC commands */

#define CMMIDI_IN_EP                                0x82U
#define CMMIDI_OUT_EP                               0x02U
#define CMCDC_CMD_EP2                               0x84U

#ifndef CM_HS_BINTERVAL
#define CM_HS_BINTERVAL                             0x10U
#endif /* CM_HS_BINTERVAL */

#ifndef CM_FS_BINTERVAL
#define CM_FS_BINTERVAL                             0x10U
#endif /* CM_FS_BINTERVAL */

/* DCDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define CM_DATA_HS_MAX_PACKET_SIZE                  512U  /* Endpoint IN & OUT Packet size */
#define CM_DATA_FS_MAX_PACKET_SIZE                  64U  /* Endpoint IN & OUT Packet size */
#define CM_CMD_PACKET_SIZE                          8U  /* Control Endpoint Packet size */

#define USB_CM_CONFIG_DESC_SIZ                      67U
#define CMCDC_DATA_HS_IN_PACKET_SIZE                CM_DATA_HS_MAX_PACKET_SIZE
#define CMCDC_DATA_HS_OUT_PACKET_SIZE               CM_DATA_HS_MAX_PACKET_SIZE

#define CMCDC_DATA_FS_IN_PACKET_SIZE                CM_DATA_FS_MAX_PACKET_SIZE
#define CMCDC_DATA_FS_OUT_PACKET_SIZE               CM_DATA_FS_MAX_PACKET_SIZE

#define CM_CONF_DESC_LENGTH                         0xC7

/*---------------------------------------------------------------------*/
/*  DCDC definitions                                                    */
/*---------------------------------------------------------------------*/
#define CM_SEND_ENCAPSULATED_COMMAND               0x00U
#define CM_GET_ENCAPSULATED_RESPONSE               0x01U
#define CM_SET_COMM_FEATURE                        0x02U
#define CM_GET_COMM_FEATURE                        0x03U
#define CM_CLEAR_COMM_FEATURE                      0x04U
#define CM_SET_LINE_CODING                         0x20U
#define CM_GET_LINE_CODING                         0x21U
#define CM_SET_CONTROL_LINE_STATE                  0x22U
#define CM_SEND_BREAK                              0x23U

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
} USBD_CM_LineCodingTypeDef;

typedef struct {
    uint32_t data[CM_DATA_HS_MAX_PACKET_SIZE / 4U];      /* Force 32bits alignment */
    uint8_t  CmdOpCode;
    uint8_t  CmdLength;
    uint8_t  *RxBuffer;
    uint8_t  *TxBuffer;
    uint32_t RxLength;
    uint32_t TxLength;

    __IO uint32_t TxState;
    __IO uint32_t RxState;
} USBD_CDC_HandleTypeDef;

typedef struct _USBD_CM_Itf
{
  int8_t (* Init)(USBD_CDC_HandleTypeDef *cdc);
  int8_t (* DeInit)(USBD_CDC_HandleTypeDef *cdc);
  int8_t (* Control)(USBD_CDC_HandleTypeDef *cdc, uint8_t cmd, uint8_t *pbuf, uint16_t length);
  int8_t (* Receive)(USBD_CDC_HandleTypeDef *cdc, uint8_t *Buf, uint32_t *Len);

} USBD_CM_ItfTypeDef;

typedef struct
{
  USBD_CDC_HandleTypeDef CDC1;
  USBD_CDC_HandleTypeDef CDC2;
}
USBD_CM_HandleTypeDef;



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef  USBD_CM;
#define USBD_CM_CLASS    &USBD_CM
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t  USBD_CM_RegisterInterface(USBD_HandleTypeDef   *pdev,
                                    USBD_CM_ItfTypeDef *fops);

uint8_t  USBD_CM_SetTxBuffer(USBD_HandleTypeDef   *pdev,
                              USBD_CDC_HandleTypeDef *cdc,
                              uint8_t  *pbuff,
                              uint16_t length);

uint8_t  USBD_CM_SetRxBuffer(USBD_HandleTypeDef   *pdev,
                              USBD_CDC_HandleTypeDef *cdc,
                              uint8_t  *pbuff);

uint8_t  USBD_CM_ReceivePacket(USBD_HandleTypeDef *pdev, USBD_CDC_HandleTypeDef *cdc);

uint8_t  USBD_CM_TransmitPacket(USBD_HandleTypeDef *pdev, USBD_CDC_HandleTypeDef *cdc);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_DCDC_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
