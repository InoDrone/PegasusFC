/*
 * STM32Timer.h
 *
 *  Created on: 15 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSHALSTM32CORETIMER_H
#define PEGASUSHALSTM32CORETIMER_H

#include "hal/stm32f4/include/Defs.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            class CoreTimer
            {
                public:
                  CoreTimer();

                  void init();
                  void start();
                  void stop();
            };
        } /* namespace STM32 */
    } /* namespace arch */

    using CoreTimer = pegasus::hal::stm32f4::CoreTimer;
} /* namespace Pegasus */

#endif /* PEGASUSHALSTM32CORETIMER_H */
