/*
 * PegasusSTM32.cpp
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#include "pegasus/core/include/MainThread.h"
#include "pegasus/hal/stm32f4/include/Core.h"
#include "pegasus/hal/stm32f4/include/Processor.h"
#include "pegasus/hal/stm32f4/include/CoreTimer.h"
#include "pegasus/hal/stm32f4/include/ThreadContext.h"

extern "C" {
  void Reset_Handler(void) {};
}

namespace pegasus {
  namespace hal {
    namespace stm32f4 {

      void Core::init()
      {
          SystemInit();
          SystemCoreClock = F_CPU;//((HSE_VALUE / PLL_M) * PLL_N) / PLL_P;

          // Timer Prescaler Enable
          RCC->DCKCFGR |= RCC_DCKCFGR_TIMPRE;

          // Enable threading
          ThreadContext::_sStack = pegasus::core::mainThread.getContext().getPStack();
          (*((volatile unsigned long *) 0xE000ED20)) |= (PENDSV_INTERRUPT_PRIORITY << 0x10);
      }

      void Core::reset()
      {
          NVIC_SystemReset();
      }

      void Core::yield()
      {
          (*((volatile unsigned long *) 0xE000ED04)) = (1 << 0x1C);
      }

      void Core::initThread() {

          /*Processor::disableInterrupts();
          asm volatile(
                  "ldr r1, %[stack]             \n"
                  "ldr r0, [r1]                 \n"
                  "                             \n"
                  //"ldmia r0!, {r4-r11}          \n"
                  "add r0, r0, #32              \n"
                  "msr psp, r0                  \n"
                  :
                  : [stack] "m"(ThreadContext::_sStack)
                  :
          );

          Processor::setCONTROL(0x02);

          //yield();
          //Processor::toMSP();
          Processor::enableInterrupts();*/

      }

      void Core::initDataEndBss()
      {
          u32 *pSrc;
          u32 *pDest;

          // Load initialized data from FLASH to RAM
          pSrc = &_etext;
          pDest = &_sdata;

          while(pDest < &_edata)
            *pDest++ = *pSrc++;

          // Clear bss uninitialized data
          pDest = &__bss_start__;

          while(pDest < &__bss_end__)
            *pDest++ = 0;
      }

      void Core::callStaticConstructors()
      {
          Func* p = (Func*) &__init_array_start;
          for(; p < (Func*) &__init_array_end; ++p) {
              (*(*p))();
          }
      }

      namespace InterruptHandler {
        void Reset(void)
        {

            Core::initDataEndBss();
            Core::callStaticConstructors();

            pegasus::core::mainThread.start();


            /* Set main thread */
            uint32_t* stack = pegasus::core::mainThread.getTopStackAligned();
            __set_PSP((uint32_t) stack);
            __ISB();

            __set_CONTROL( __get_CONTROL() | 0x02 );
            __ISB();

          /*  Processor::disableInterrupts();
            asm volatile(
                    "ldr r1, %[stack]             \n"
                    "ldr r0, [r1]                 \n"
                    "                             \n"
                    //"ldmia r0!, {r4-r11}          \n"
                    "add r0, r0, #32              \n"
                    "msr psp, r0                  \n"
                    :
                    : [stack] "m"(ThreadContext::_sStack)
                    :
            );*/





            //yield();
            //Processor::toMSP();
            Processor::enableInterrupts();


            //__get_MSP();
            //__ISB();

            main();

            while(1) {}
        }
      }
    }
  }
}

//const PegasusSTM32 pegasusSTM32;

