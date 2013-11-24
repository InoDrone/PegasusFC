/*
 * PegasusSTM32.cpp
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#include "pegasus/hal/stm32f4/include/Core.h"
#include "pegasus/hal/stm32f4/include/CoreTimer.h"

extern "C" {
  void Reset_Handler(void) {};
}

namespace pegasus {
  namespace hal {
    namespace stm32f4 {

      void Core::init()
      {
          // Not implemented Yet
      }

      void Core::reset()
      {
          NVIC_SystemReset();
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

      void Core::initSystem()
      {
          CoreTimer::init();
      }

      namespace InterruptHandler {
        void Reset(void)
        {
            Core::initDataEndBss();
            Core::callStaticConstructors();

            SystemInit();
            SystemCoreClock = F_CPU;//((HSE_VALUE / PLL_M) * PLL_N) / PLL_P;

            Core::initSystem();
            //SystemCoreClockUpdate();

            main();
        }
      }
    }
  }
}

//const PegasusSTM32 pegasusSTM32;

