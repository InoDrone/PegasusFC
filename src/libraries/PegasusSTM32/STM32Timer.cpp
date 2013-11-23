/*
 * STM32Timer.cpp
 *
 *  Created on: 15 nov. 2013
 *      Author: alienx
 */

#include "STM32Timer.h"

namespace Pegasus {
    namespace arch {
        namespace STM32 {

            STM32Timer::STM32Timer() {}

            void STM32Timer::init() {
              systemMillis = 0;
              // init system tick to 1us
              // Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
              SysTick_Config(SystemCoreClock * 0.001);
            }

            /**
             *
             */
            void STM32Timer::delayMillis(uint32_t ms)
            {
                uint32_t start = Pegasus::arch::STM32::timer.millis();//systemMillis;

                while( (systemMillis - start) < ms);
            }

            /**
             *
             */
            void STM32Timer::delayMicro(uint32_t us)
            {
              uint32_t start = Pegasus::arch::STM32::timer.micros();

              while( (micros() - start) < us);
            }

            /**
             *
             */
            uint32_t STM32Timer::millis()
            {
                return systemMillis;
            }

            /**
             *
             */
            uint32_t STM32Timer::micros()
            {
                /*uint32_t ms;
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

                return (ms * 1000) + (SYSTICK_RELOAD_VAL + 1 - sys_cnt) / CYCLES_PER_MICROSECOND;*/

                return 0;
            }

            /**
             *
             */
            void STM32Timer::attach(Func, uint32_t us)
            {

            }

            /**
             *
             */
            void STM32Timer::detach(Func)
            {

            }

            void STM32Timer::interruptServiceHandler()
            {
                systemMillis++;
            }


            const STM32Timer::Timer TIMER1 = {TIM1, &RCC->APB2ENR, RCC_APB2Periph_TIM1, GPIO_AF_TIM1};
            const STM32Timer::Timer TIMER2 = {TIM2, &RCC->APB1ENR, RCC_APB1Periph_TIM2, GPIO_AF_TIM2};
            const STM32Timer::Timer TIMER3 = {TIM3, &RCC->APB1ENR, RCC_APB1Periph_TIM3, GPIO_AF_TIM3};
            const STM32Timer::Timer TIMER4 = {TIM4, &RCC->APB1ENR, RCC_APB1Periph_TIM4, GPIO_AF_TIM4};
            const STM32Timer::Timer TIMER5 = {TIM5, &RCC->APB1ENR, RCC_APB1Periph_TIM5, GPIO_AF_TIM5};
            const STM32Timer::Timer TIMER8 = {TIM8, &RCC->APB2ENR, RCC_APB2Periph_TIM8, GPIO_AF_TIM8};

            STM32Timer timer;

            namespace InterruptHandler {
              void SystemTick(void)
              {
                  Pegasus::arch::STM32::timer.interruptServiceHandler();
              }
            }
        } /* namespace STM32 */
    } /* namespace arch */
} /* namespace Pegasus */
