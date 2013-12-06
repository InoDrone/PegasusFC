/*
 * Timer.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "hal/stm32f4/include/Timer.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            Timer::Timer(TIM_TypeDef* timer) :
                TimerBase<TIM_TypeDef>(timer) {

                if (_mReg == TIM3) {
                    RCC->APB1ENR |= RCC_TIM3;
                } else if (_mReg == TIM4 ) {
                    RCC->APB1ENR |= RCC_TIM4;
                }

                // Reset timer
                _mReg->CR1 &= ~(TIM_CR1_CKD | TIM_CR1_DIR | TIM_CR1_CMS);
            }

            void Timer::setFreq(uint32_t freqHz)
            {
                /*uint32_t periodCycle = SystemCoreClock / freqHz;
                uint16_t psc = (uint16_t) (periodCycle / (65536+1)); // Prescaler
                uint16_t arr = (uint16_t) (periodCycle / (psc+1));   // Reload (Overflow)
                */

                uint16_t psc = (uint16_t)((SystemCoreClock / 1000000) -1);
                uint16_t arr = 0xFFFF;
                if (freqHz < 0xFFFF) {
                    arr = (uint16_t)( 1000000 / freqHz);
                }

                _mReg->PSC = psc;           // set prescaler
                _mReg->ARR = arr;           // Set overflow
                _mReg->CR1 |= TIM_CR1_ARPE; // ARR is set
                _mReg->EGR |= TIM_EGR_UG;   // Update generation

            }

            void Timer::setMod(uint8_t channel, timer::Mode mode) {

                __IO uint16_t* CCMRX = channel >= 2 ? &_mReg->CCMR2 : &_mReg->CCMR1;
                uint16_t ccmrx = *CCMRX;
                uint16_t ccmrChannel = ((channel&0x01) * 8);

                /* Disable channel Capture/Compare */
                _mReg->CCER &= ~(TIM_CCER_CC1E << (channel * 4));

                switch (mode) {
                    case timer::Mode::IC:
                        ccmrx &= (uint16_t)~(TIMER_CCMR_MASK  << ccmrChannel); // Reset register
                        ccmrx |= (uint16_t)(TIM_CCXS_INPUT << ccmrChannel); // Set mode to IC
                        //ccmrx |= (uint16_t)(0x30 << ccmrChannel); // fSAMPLING=fDTS/4, N=61110: fSAMPLING=fDTS/32, N=6

                        *CCMRX = ccmrx;

                        _mReg->CCER &= ~(0xE << (channel *4)); // reset polarity
                        //_mReg->CCER |= (TIM_CCER_RISING_EDGE << (channel *4));    // Both edges
                        _mReg->CCER |= (TIM_CCER_CC1E << (channel *4));         // Enable channel IC
                        break;
                    case timer::Mode::OC: // PWM 1
                        ccmrx &= (uint16_t)~(TIMER_CCMR_MASK  << ccmrChannel); // Reset register
                        ccmrx |= (uint16_t)(TIM_MODE_PWM1 << ccmrChannel); // Set mode to PWM 1
                        ccmrx |= (uint16_t)(TIM_OCPRELOAD_ENABLE << ccmrChannel);

                        *CCMRX = ccmrx;

                        _mReg->CCER |= (TIM_CCER_CC1E << (channel *4)); // Enable channel OC
                        break;
                }
            }
        }
    }
}


