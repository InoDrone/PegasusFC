/**
  ******************************************************************************
  * @file    usbd_core.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   Header file for usbd_core.c
  ******************************************************************************
  * @attention  
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CORE_H
#define __USBD_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usb_dcd.h"
#include "usbd_def.h"
#include "usbd_conf.h"
#include "usb_dcd_int.h"

#include <stdint.h>

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_CORE
  * @brief This file is the Header file for usbd_core.c file
  * @{
  */ 


/** @defgroup USBD_CORE_Exported_Defines
  * @{
  */ 

typedef enum {
  USBD_OK   = 0,
  USBD_BUSY,
  USBD_FAIL,
}USBD_Status;
/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
 

/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern const USBD_DCD_INT_cb_TypeDef USBD_DCD_INT_cb;

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_FunctionsPrototype
  * @{
  */ 
void USBD_Init(USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID, 
               USBD_DEVICE *pDevice,                  
               USBD_Class_cb_TypeDef *class_cb, 
               USBD_Usr_cb_TypeDef *usr_cb);

USBD_Status USBD_DeInit(USB_OTG_CORE_HANDLE *pdev);

USBD_Status USBD_ClrCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx);

USBD_Status USBD_SetCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx);


uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev);
uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev);
#ifdef VBUS_SENSING_ENABLED
uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev);
#endif
uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev);

/**
  * @}
  */ 

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __USBD_CORE_H */

/**
  * @}
  */ 

/**
* @}
*/ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/



