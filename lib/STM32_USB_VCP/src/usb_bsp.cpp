/**
  ******************************************************************************
  * @file    usb_bsp.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   This file is responsible to offer board support package and is
  *          configurable by user.
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

/* Includes ------------------------------------------------------------------*/
#include "usb_bsp.h"
#include "usbd_conf.h"
#include "hal/stm32f4/include/Gpio.h"
#define OTG_FS_IRQn 67

//#include "stm32f4_discovery.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
* @{
*/

/** @defgroup USB_BSP
* @brief This file is responsible to offer board support package
* @{
*/

/** @defgroup USB_BSP_Private_Defines
* @{
*/
/**
* @}
*/


/** @defgroup USB_BSP_Private_TypesDefinitions
* @{
*/
/**
* @}
*/





/** @defgroup USB_BSP_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup USBH_BSP_Private_Variables
* @{
*/

/**
* @}
*/

/** @defgroup USBH_BSP_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/

/** @defgroup USB_BSP_Private_Functions
* @{
*/


/**
* @brief  USB_OTG_BSP_Init
*         Initilizes BSP configurations
* @param  None
* @retval None
*/

void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{

        pegasus::hal::stm32f4::Gpio ioA11(pegasus::gpio::A, pegasus::gpio::PIN11);
        pegasus::hal::stm32f4::Gpio ioA12(pegasus::gpio::A, pegasus::gpio::PIN12);

        ioA11.mode(pegasus::gpio::Mode::AF_NOPULL);
        ioA12.mode(pegasus::gpio::Mode::AF_NOPULL);

        ioA11.setAlternateFunction(pegasus::gpio::AlternateFunction::AF10);
        ioA12.setAlternateFunction(pegasus::gpio::AlternateFunction::AF10);

        RCC->AHB2ENR |= 0x80; // Enable OTG FS RCC
}

void USB_OTG_BSP_DeInit(USB_OTG_CORE_HANDLE *pdev)
{

    pegasus::hal::stm32f4::Gpio ioA11(pegasus::gpio::A, pegasus::gpio::PIN11);
    pegasus::hal::stm32f4::Gpio ioA12(pegasus::gpio::A, pegasus::gpio::PIN12);

    ioA11.mode(pegasus::gpio::Mode::INPUT);
    ioA12.mode(pegasus::gpio::Mode::INPUT);

    ioA11.setAlternateFunction(pegasus::gpio::AlternateFunction::AF0);
    ioA12.setAlternateFunction(pegasus::gpio::AlternateFunction::AF0);

    RCC->AHB2ENR &= ~(0x80); // Enable OTG FS RCC
}


/**
* @brief  USB_OTG_BSP_EnableInterrupt
*         Enabele USB Global interrupt
* @param  None
* @retval None
*/
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
    NVIC->IP[OTG_FS_IRQn] = 0x0A << 0x04; // TODO change to dynamic prio
    NVIC->ISER[OTG_FS_IRQn >> 0x05] = (uint32_t)0x01 << (OTG_FS_IRQn & (uint8_t)0x1F);
}

void USB_OTG_BSP_DisableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{

}

/**
* @brief  USB_OTG_BSP_uDelay
*         This function provides delay time in micro sec
* @param  usec : Value of delay required in micro sec
* @retval None
*/
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
  uint32_t count = 0;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}


/**
* @brief  USB_OTG_BSP_mDelay
*          This function provides delay time in milli sec
* @param  msec : Value of delay required in milli sec
* @retval None
*/
void USB_OTG_BSP_mDelay (const uint32_t msec)
{
  USB_OTG_BSP_uDelay(msec * 1000);
}
/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
