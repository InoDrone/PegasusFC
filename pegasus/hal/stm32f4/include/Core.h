/*
 * PegasusSTM32.h
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSHALSTM32CORE_H
#define PEGASUSHALSTM32CORE_H

#include "hal/include/HalBase.h"
#include "hal/stm32f4/include/Defs.h"

int main();
extern "C" {
  extern u32    _etext;
  extern u32    _sdata;
  extern u32    _sidata;
  extern u32    _edata;
  extern u32    __bss_start__;
  extern u32    __bss_end__;
  extern u32   __init_array_start;
  extern u32   __init_array_end;

  extern void __libc_init_array();
}

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            struct Clocks {
                    uint32_t SYSCLK; /*!<  SYSCLK clock frequency expressed in Hz */
                    uint32_t HCLK;   /*!<  HCLK clock frequency expressed in Hz   */
                    uint32_t PCLK1;  /*!<  PCLK1 clock frequency expressed in Hz  */
                    uint32_t PCLK2;  /*!<  PCLK2 clock frequency expressed in Hz  */
            };

            typedef pegasus::hal::HalBase HalBase;

            class Core : public HalBase {
                public:
                    static void init();
                    static void reset();

                    static void yield();

                    static void enterISR();
                    static void exitISR();
                    static bool inISR();

                    static uint8_t isrCount;

                    static void initDataEndBss();
                    static void callStaticConstructors();

                    static void getSystemClock(Clocks* clock);

                    static void initThread();
            };
        }
    }
}

#endif /* PEGASUSHALSTM32CORE_H */
