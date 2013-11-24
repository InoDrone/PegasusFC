/*
 * STM32Timer.h
 *
 *  Created on: 15 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSHALSTM32CORETIMER_H
#define PEGASUSHALSTM32CORETIMER_H

#include "pegasus/hal/stm32f4/include/Defs.h"
//#include "pegasus/hal/include/CoreTimer.h"

/*#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"*/

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            class CoreTimer
            {
                public:
                  CoreTimer();

                  static void init();

                  void delayMillis(uint32_t ms);
                  void delayMicro(uint32_t us);
                  uint32_t millis();
                  uint32_t micros();



                  void attach(Func, uint32_t us);
                  void detach(Func);

                  void interruptServiceHandler();


                private:
                    volatile uint32_t systemMillis;
            };
        } /* namespace STM32 */
    } /* namespace arch */
} /* namespace Pegasus */

#endif /* PEGASUSHALSTM32CORETIMER_H */
