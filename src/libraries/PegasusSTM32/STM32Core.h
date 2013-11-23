/*
 * PegasusSTM32.h
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSSTM32_H_
#define PEGASUSSTM32_H_

#include "STM32Defs.h"
#include "STM32Timer.h"
#include "STM32GPIO.h"

int main();
extern "C" {
  extern u32 _etext;
  extern u32 _sdata;
  extern u32 _edata;
  extern u32 __bss_start__;
  extern u32 __bss_end__;
}

namespace Pegasus {
    namespace arch {
        namespace STM32 {

            class STM32Core {
                public:
                    STM32Core();
                    static void init();
            };


            extern STM32Core core;
        }
    }
}

#endif /* PEGASUSSTM32_H_ */
