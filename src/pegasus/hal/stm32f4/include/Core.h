/*
 * PegasusSTM32.h
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSHALSTM32CORE_H
#define PEGASUSHALSTM32CORE_H

#include "pegasus/hal/include/HalBase.h"
#include "pegasus/hal/stm32f4/include/Defs.h"

int main();
extern "C" {
  extern u32    _etext;
  extern u32    _sdata;
  extern u32    _edata;
  extern u32    __bss_start__;
  extern u32    __bss_end__;
  extern u32   __init_array_start;
  extern u32   __init_array_end;
}

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            typedef pegasus::hal::HalBase HalBase;

            class Core : public HalBase {
                public:
                    Core(void) = delete;
                    static void init();
                    static void reset();

                    static void initDataEndBss();
                    static void callStaticConstructors();
                    static void initSystem();
            };
        }
    }
}

#endif /* PEGASUSHALSTM32CORE_H */
