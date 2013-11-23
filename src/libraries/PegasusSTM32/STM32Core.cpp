/*
 * PegasusSTM32.cpp
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#include "STM32Core.h"

extern "C" {
  void Reset_Handler(void) {};
}

namespace Pegasus {
  namespace arch {
    namespace STM32 {

      STM32Core::STM32Core() {}

      void STM32Core::init()
      {
          io.init();
          timer.init();

      }

      static INLINE void initData()
      {
        u32 *pSrc;
        u32 *pDest;

        // Load initialized data from FLASH to RAM
        pSrc = &_etext;
        pDest = &_sdata;

        while(pDest < &_edata)
          *pDest++ = *pSrc++;

        // Clear uninitialized data
        pDest = &__bss_start__;

        while(pDest < &__bss_end__)
          *pDest++ = 0;

      }

      namespace InterruptHandler {
        void Reset(void)
        {
            initData();
            SystemInit();
            SystemCoreClock = F_CPU;//((HSE_VALUE / PLL_M) * PLL_N) / PLL_P;

            //SystemCoreClockUpdate();

            main();
        }
      }



      STM32Core core;
    }
  }
}

//const PegasusSTM32 pegasusSTM32;

