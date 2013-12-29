/*
 * Timer.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "hal/stm32f4/include/Timer.h"
#include "hal/stm32f4/include/Core.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            Timer::Timer(TIM_TypeDef* timer) :
                TimerBase<TIM_TypeDef>(timer) {

                if (_mReg == TIM3) {
                    RCC->APB1ENR |= RCC_TIM3;
                } else if (_mReg == TIM4 ) {
                    RCC->APB1ENR |= RCC_TIM4;
                } else if (_mReg == TIM6 ) {
                    RCC->APB1ENR |= RCC_TIM6;
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
                Clocks clocks;
                Core::getSystemClock(&clocks);

                uint16_t psc = (uint16_t)(( (clocks.PCLK1*2) / 1000000) -1);
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

            uint8_t Timer::getUniqId()
            {
                uint8_t pos = 0;
                if (_mReg == TIM2) {
                    pos = 1;
                } else if (_mReg == TIM3) {
                    pos = 2;
                } else if (_mReg == TIM4) {
                    pos = 3;
                } else if (_mReg == TIM5) {
                    pos = 4;
                } else if (_mReg == TIM6) {
                    pos = 5;
                } else if (_mReg == TIM7) {
                    pos = 6;
                } else if (_mReg == TIM8) {
                    pos = 7;
                } else if (_mReg == TIM9) {
                    pos = 8;
                } else if (_mReg == TIM10) {
                    pos = 9;
                } else {
                    pos = 10;
                }// TODO finish

                return (uint8_t)(pos * 5); // 5 interrupt 4 OC/IC , 1 Update Interrupt
            }
        }
    }
}


