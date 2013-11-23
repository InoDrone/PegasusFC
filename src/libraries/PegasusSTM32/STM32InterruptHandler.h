/*
 * STM32InterruptHandler.h
 *
 *  Created on: 7 nov. 2013
 *      Author: alienx
 */

#ifndef STM32INTERRUPTHANDLER_H_
#define STM32INTERRUPTHANDLER_H_

#include "STM32Defs.h"

extern "C" {

  // From LD Script
  extern unsigned int _estack;
}

namespace Pegasus
{
    namespace arch {
        namespace STM32
        {
            namespace InterruptHandler
            {

            void **HARDFAULT_PSP;
            register void *stack_pointer asm("sp");

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
            }


            __attribute__((section(".core_isr_vectors")))
            uint32_t interruptVectors[] = {
                // Top
                reinterpret_cast<uint32_t>(&_estack),
                // Reset
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::Reset),
                // NMI
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::NMI),
                // HardFault
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::HardFault),
                // MemManage
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::MemManage),
                // BusFault
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::BusFault),
                // UsageFault
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::UsageFault),
                0, 0, 0, 0,
                // Service handler
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::ServiceCall),
                // Debug Monitor
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::DebugMon),
                0,
                // PendService
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::PendService),
                // System Tick Handler
                reinterpret_cast<uint32_t>(Pegasus::arch::STM32::InterruptHandler::SystemTick)
            };

        } /* namespace STM32 */
    }
} /* namespace IDPegasus */

#endif /* STM32INTERRUPTHANDLER_H_ */
