/*
 * PegasusSTM32.cpp
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#include "core/include/MainThread.h"
#include "hal/stm32f4/include/Core.h"
#include "hal/stm32f4/include/Processor.h"
#include "hal/stm32f4/include/CoreTimer.h"
#include "hal/stm32f4/include/ThreadContext.h"

extern "C" {
  void Reset_Handler(void) {};
}

namespace pegasus {
  namespace hal {
    namespace stm32f4 {

      const uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

      void Core::init()
      {
          SystemInit();
          SystemCoreClock = F_CPU;//((HSE_VALUE / PLL_M) * PLL_N) / PLL_P;

          // Timer Prescaler Enable
          RCC->DCKCFGR |= RCC_DCKCFGR_TIMPRE;

          // Enable threading
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

      void Core::getSystemClock(Clocks* clocks)
      {
          uint32_t tmp = 0, presc = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;
          tmp = RCC->CFGR & RCC_CFGR_SWS;

          switch (tmp)
          {
            case 0x00:  /* HSI used as system clock source */
              clocks->SYSCLK = HSI_VALUE;
              break;
            case 0x04:  /* HSE used as system clock  source */
              clocks->SYSCLK = HSE_VALUE;
              break;
            case 0x08:  /* PLL used as system clock  source */

              /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
                 SYSCLK = PLL_VCO / PLLP
                 */
              pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
              pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

              if (pllsource != 0)
              {
                /* HSE used as PLL clock source */
                pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
              }
              else
              {
                /* HSI used as PLL clock source */
                pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
              }

              pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
              clocks->SYSCLK = pllvco/pllp;
              break;
            default:
              clocks->SYSCLK = HSI_VALUE;
              break;
          }

          /* Get HCLK prescaler */
          tmp = RCC->CFGR & RCC_CFGR_HPRE;
          tmp = tmp >> 4;
          presc = APBAHBPrescTable[tmp];
          /* HCLK clock frequency */
          clocks->HCLK = clocks->SYSCLK >> presc;

          /* Get PCLK1 prescaler */
          tmp = RCC->CFGR & RCC_CFGR_PPRE1;
          tmp = tmp >> 10;
          presc = APBAHBPrescTable[tmp];
          /* PCLK1 clock frequency */
          clocks->PCLK1 = clocks->HCLK >> presc;

          /* Get PCLK2 prescaler */
          tmp = RCC->CFGR & RCC_CFGR_PPRE2;
          tmp = tmp >> 13;
          presc = APBAHBPrescTable[tmp];
          /* PCLK2 clock frequency */
          clocks->PCLK2 = clocks->HCLK >> presc;
      }

      void Core::initDataEndBss()
      {
          u32 *pSrc;
          u32 *pDest;

          // Load initialized data from FLASH to RAM
          pSrc = &_sidata;

          for (pDest = &_sdata; pDest < &_edata;) {
              *(pDest++) = *(pSrc++);
          }

          /*while(pDest < &_edata)
            *pDest++ = *pSrc++;*/


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
            __libc_init_array();
            //Core::callStaticConstructors(); <- replaced by __libc_init_array()

            pegasus::core::mainThread.start();

            __set_CONTROL(0x0);
            Processor::enableInterrupts();

            main();

            while(1) {}
        }
      }
    }
  }
}

//const PegasusSTM32 pegasusSTM32;

