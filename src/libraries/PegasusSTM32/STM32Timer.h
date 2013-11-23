/*
 * STM32Timer.h
 *
 *  Created on: 15 nov. 2013
 *      Author: alienx
 */

#ifndef STM32TIMER_H_
#define STM32TIMER_H_

#include "STM32Defs.h"
#include "Timer.h"

#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

namespace Pegasus {
    namespace arch {
        namespace STM32 {

            class STM32Timer : public Pegasus::Timer
            {
                public:

                  struct Timer {
                      TIM_TypeDef*    reg;
                      __IO uint32_t*  rcc;
                      uint8_t         rcc_bit;
                      uint8_t         AF;
                  };

                  STM32Timer();

                  void init();

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

            extern const STM32Timer::Timer TIMER1;
            extern const STM32Timer::Timer TIMER2;
            extern const STM32Timer::Timer TIMER3;
            extern const STM32Timer::Timer TIMER4;
            extern const STM32Timer::Timer TIMER5;
            extern const  STM32Timer::Timer TIMER8;

            extern STM32Timer timer;
        } /* namespace STM32 */
    } /* namespace arch */
} /* namespace Pegasus */

#endif /* STM32TIMER_H_ */
