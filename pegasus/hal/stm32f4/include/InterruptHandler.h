/*
 * STM32InterruptHandler.h
 *
 *  Created on: 7 nov. 2013
 *      Author: alienx
 */

#ifndef STM32INTERRUPTHANDLER_H_
#define STM32INTERRUPTHANDLER_H_

#include "hal/stm32f4/include/Defs.h"

extern "C" {

  // From LD Script
  extern unsigned int _estack;
}

namespace pegasus {
    namespace hal {
        namespace stm32f4
        {
            namespace InterruptHandler
            {

              extern void Reset(void);
              extern void NMI(void);
              extern void HardFault(void);
              extern void MemManage(void);
              extern void BusFault(void);
              extern void UsageFault(void);
              extern void ServiceCall(void);
              extern void PendService(void);
              extern void SystemTick(void);
              extern void DebugMon(void);
              extern void DefaultHandler(void);

              extern void EXTIx(void);
              extern void TIM1_9Int(void);
              extern void TIM1_10Int(void);
              extern void TIM1_11Int(void);
              extern void TIM1CCInt(void);
              extern void TIM2Int(void);
              extern void TIM3Int(void);
              extern void TIM4Int(void);
              extern void TIM5Int(void);
              extern void TIM6Int(void);
              extern void TIM7Int(void);
              extern void UART1Int(void);
              extern void UART2Int(void);
              extern void UART3Int(void);
              extern void UART4Int(void);
              extern void UART5Int(void);

              extern void OTGFSInt(void);

              extern void DMA1_Stream6_IRQ(void); // UART 2 DM TX
            }


            __attribute__((section(".core_isr_vectors")))
            uint32_t interruptVectors[] = {
                // Top
                reinterpret_cast<uint32_t>(&_estack),
                // Reset
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::Reset),
                // NMI
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::NMI),
                // HardFault
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::HardFault),
                // MemManage
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::MemManage),
                // BusFault
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::BusFault),
                // UsageFault
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::UsageFault),
                0, 0, 0, 0,
                // Service handler
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::ServiceCall),
                // Debug Monitor
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DebugMon),
                0,
                // PendService
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::PendService),
                // System Tick Handler
                reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::SystemTick)
            };

            __attribute__((section(".isr_peripherals_vectors")))
            uint32_t interruptPeriVectors[] = {
                    /* WWDG_IRQHandler          Window WatchDog */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* PVD_IRQHandler           PVD through EXTI Line detection */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* TAMP_STAMP_IRQHandler    Tamper and TimeStamps through the EXTI line */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* RTC_WKUP_IRQHandler      RTC Wakeup through the EXTI line */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* FLASH_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* RCC_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* EXTI0_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::EXTIx),
                    /* EXTI1_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::EXTIx),
                    /* EXTI2_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::EXTIx),
                    /* EXTI3_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::EXTIx),
                    /* EXTI4_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::EXTIx),
                    /* DMA1_Stream0_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA1_Stream1_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA1_Stream2_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA1_Stream3_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA1_Stream4_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA1_Stream5_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA1_Stream6_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DMA1_Stream6_IRQ),
                    /* ADC_IRQHandler ADC1-3  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* CAN1_TX_IRQHandler CAN1 TX  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* CAN1_RX0_IRQHandler CAN1 RX0  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* CAN1_RX1_IRQHandler CAN1 RX1  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* CAN1_SCE_IRQHandler CAN1 SCE  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* EXTI9_5_IRQHandler        External Line[9:5]s  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::EXTIx),
                    /* TIM1_BRK_TIM9_IRQHandler         TIM1 Break and TIM9     */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM1_9Int),
                    /* TIM1_UP_TIM10_IRQHandler         TIM1 Update and TIM10     */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM1_10Int),
                    /* TIM1_TRG_COM_TIM11_IRQHandler    TIM1 Trigger and Commutation and TIM11     */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM1_11Int),
                    /* TIM1_CC_IRQHandler               TIM1 Capture/Compare INT    */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM1CCInt),
                    /* TIM2_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM2Int),
                    /* TIM3_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM3Int),
                    /* TIM4_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM4Int),
                    /* I2C1_EV_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* I2C1_ER_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* I2C2_EV_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* I2C2_ER_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* SPI1_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* SPI2_IRQHandler */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* UART1 */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::UART1Int),
                    /* UART2 */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::UART2Int),
                    /* UART3 */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::UART3Int),
                    /* EXTI15_10_IRQHandler             External Line[15:10]s */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::EXTIx),
                    /* RTC_Alarm_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* OTG_FS_WKUP_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* TIM8_BRK_TIM12_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* TIM8_UP_TIM13_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* TIM8_TRG_COM_TIM14_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* TIM8_CC_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA1_Stream7_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* FSMC_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* SDIO_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* TIM5_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM5Int),
                    /* SPI3_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* UART4_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::UART4Int),
                    /* UART5_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::UART5Int),
                    /* TIM6_DAC_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM6Int),
                    /* TIM7_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::TIM7Int),
                    /* DMA2_Stream0_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA2_Stream1_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA2_Stream2_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA2_Stream3_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA2_Stream4_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* ETH_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* ETH_WKUP_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* CAN2_TX_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* CAN2_RX0_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* CAN2_RX1_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* CAN2_SCE_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* OTG_FS_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::OTGFSInt),
                    /* DMA2_Stream5_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA2_Stream6_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DMA2_Stream7_IRQHandler  */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* USART6_IRQHandler   */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* I2C3_EV_IRQHandler    */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* I2C3_ER_IRQHandler    */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* OTG_HS_EP1_OUT_IRQHandler    */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* OTG_HS_EP1_IN_IRQHandler    */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* OTG_HS_WKUP_IRQHandler     */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* OTG_HS_IRQHandler     */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* DCMI_IRQHandler     */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* CRYP_IRQHandler     */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* HASH_RNG_IRQHandler     */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler),
                    /* FPU_IRQHandler      */
                    reinterpret_cast<uint32_t>(pegasus::hal::stm32f4::InterruptHandler::DefaultHandler)
            };


        } /* namespace STM32 */
    }
} /* namespace IDPegasus */

#endif /* STM32INTERRUPTHANDLER_H_ */
