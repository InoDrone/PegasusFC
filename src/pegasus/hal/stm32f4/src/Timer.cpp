/*
 * Timer.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "pegasus/hal/stm32f4/include/Timer.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            Timer::Timer(TIM_TypeDef* timer) :
                TimerBase<TIM_TypeDef>(timer) {

                if (_mReg == TIM4 ) {
                    RCC->APB1ENR |= RCC_TIM4;
                }

            }

            void Timer::setFreq(uint32_t freqHz)
            {
                /*uint32_t periodCycle = SystemCoreClock / freqHz;
                uint16_t psc = (uint16_t) (periodCycle / (65536+1)); // Prescaler
                uint16_t arr = (uint16_t) (periodCycle / (psc+1));   // Reload (Overflow)
                */

                uint16_t psc = (uint16_t)((SystemCoreClock / 1000000) -1);
                uint16_t arr = (uint16_t)( 1000000 / freqHz);

                disable();
                _mReg->PSC = psc;           // set prescaler
                _mReg->ARR = arr;           // Set overflow
                _mReg->EGR |= TIM_EGR_UG;   // Update generation
                _mReg->CR1 |= TIM_CR1_ARPE; // ARR is set
                enable();
            }

            void Timer::setMod(uint8_t channel, timer::Mode mode) {

                __IO uint16_t* CCMRX = channel >= 2 ? &_mReg->CCMR2 :& _mReg->CCMR1;
                uint16_t ccmrx = *CCMRX;
                /* Disable channel Capture/Comapre */
                _mReg->CCER &= ~((TIM_CCER_CC1E & TIMER_CCER_MASK) << channel);


                switch (mode) {
                    case timer::Mode::IC:
                        break;
                    case timer::Mode::OC: // PWM 1
                        ccmrx &= (uint16_t)~((TIMER_CCMR_OCXM_MASK & TIM_CCMR1_OC1M) << channel); // Reset mode
                        ccmrx &= (uint16_t)~((TIMER_CCMR_CCXS_MASK & TIM_CCMR1_CC1S) << channel); // Reset CC
                        ccmrx |= (uint16_t)(TIM_MODE_PWM1 << channel); // Set mode to PWM 1
                        ccmrx |= (uint16_t)(TIM_OCPRELOAD_ENABLE << channel);

                        *CCMRX = ccmrx;

                        _mReg->CCER |= ((TIM_CCER_CC1E & TIMER_CCER_MASK) << channel); // Enable channel OC
                        break;
                }


            }

        }
    }
}


