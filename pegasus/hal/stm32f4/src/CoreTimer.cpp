/*
 * STM32Timer.cpp
 *
 *  Created on: 15 nov. 2013
 *      Author: alienx
 */

#include "hal/stm32f4/include/CoreTimer.h"
#include "core/include/MainTimer.h"


namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            CoreTimer::CoreTimer() {}

            void CoreTimer::init() {
              // init system tick to 1ms
              // Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
              //SysTick_Config(SystemCoreClock * 0.001);

                // Set SHP register for PRI_15 (SysTick to low prio)
                (*((volatile unsigned long *) 0xE000ED20)) |= (SYSTICK_INTERRUPT_PRIORITY << 0x18);

               SysTick->LOAD = (uint32_t)((SystemCoreClock * 0.001) - 1);
               SysTick->VAL  = 0;
               SysTick->CTRL = ( SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk );

            }

            void CoreTimer::start()
            {
                SysTick->CTRL |= ( SysTick_CTRL_ENABLE_Msk );
            }

            void CoreTimer::stop()
            {
                SysTick->CTRL &= ~( SysTick_CTRL_ENABLE_Msk );
            }

            /**
             *
             *
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
//            }*/

            namespace InterruptHandler {
              void SystemTick(void)
              {
                  //steBasePRI
                  pegasus::core::mainTimer.interruptServiceHandler();

                  //Core::yield();
                  //setBasePRI 0
              }
            }
        } /* namespace STM32 */
    } /* namespace arch */

} /* namespace Pegasus */
