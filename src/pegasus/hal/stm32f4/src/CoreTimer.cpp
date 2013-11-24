/*
 * STM32Timer.cpp
 *
 *  Created on: 15 nov. 2013
 *      Author: alienx
 */

#include "pegasus/hal/stm32f4/include/CoreTimer.h"

namespace pegasus {

    pegasus::hal::stm32f4::CoreTimer coreTimer;


    namespace hal {
        namespace stm32f4 {

            CoreTimer::CoreTimer() {}

            void CoreTimer::init() {
              // init system tick to 1us
              // Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
              SysTick_Config(SystemCoreClock * 0.001);
            }

            /**
             *
             */
            void CoreTimer::delayMillis(uint32_t ms)
            {
                uint32_t start = millis();//systemMillis;

                while( (systemMillis - start) < ms);
            }

            /**
             *
             */
            void CoreTimer::delayMicro(uint32_t us)
            {
              uint32_t start = micros();

              while( (micros() - start) < us);
            }

            /**
             *
             */
            uint32_t CoreTimer::millis()
            {
                return systemMillis;
            }

            /**
             *
             */
            uint32_t CoreTimer::micros()
            {
                uint32_t ms;
                uint32_t sys_cnt;

                do {
                    ms = millis();
                    sys_cnt = SysTick->VAL;
                    asm volatile("nop");
                    asm volatile("nop");
                } while (ms != millis());

                if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
                    ms++;
                    sys_cnt = SysTick->VAL;
                }

                return (ms * 1000) + (SYSTICK_RELOAD_VAL + 1 - sys_cnt) / CYCLES_PER_MICROSECOND;
            }

            /**
             *
             */
            void CoreTimer::attach(Func, uint32_t us)
            {

            }

            /**
             *
             */
            void CoreTimer::detach(Func)
            {

            }

            void CoreTimer::interruptServiceHandler()
            {
                systemMillis++;
            }


           /* const STM32Timer::Timer TIMER1 = {TIM1, &RCC->APB2ENR, RCC_APB2Periph_TIM1, GPIO_AF_TIM1};
            const STM32Timer::Timer TIMER2 = {TIM2, &RCC->APB1ENR, RCC_APB1Periph_TIM2, GPIO_AF_TIM2};
            const STM32Timer::Timer TIMER3 = {TIM3, &RCC->APB1ENR, RCC_APB1Periph_TIM3, GPIO_AF_TIM3};
            const STM32Timer::Timer TIMER4 = {TIM4, &RCC->APB1ENR, RCC_APB1Periph_TIM4, GPIO_AF_TIM4};
            const STM32Timer::Timer TIMER5 = {TIM5, &RCC->APB1ENR, RCC_APB1Periph_TIM5, GPIO_AF_TIM5};
            const STM32Timer::Timer TIMER8 = {TIM8, &RCC->APB2ENR, RCC_APB2Periph_TIM8, GPIO_AF_TIM8};

            STM32Timer timer;*/

            namespace InterruptHandler {
              void SystemTick(void)
              {
                  pegasus::coreTimer.interruptServiceHandler();
              }
            }
        } /* namespace STM32 */
    } /* namespace arch */

} /* namespace Pegasus */
