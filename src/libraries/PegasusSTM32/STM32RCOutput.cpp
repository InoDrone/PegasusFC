/*
 * STM32RCOutput.cpp
 *
 *  Created on: 21 nov. 2013
 *      Author: alienx
 */

#include "STM32RCOutput.h"

namespace Pegasus{
    namespace arch {
        namespace STM32{

            STM32RCOutput::STM32RCOutput(){}

            void STM32RCOutput::add(const uint8_t pin, uint16_t freqHz)
            {
                const STM32GPIO::Pin io = PINS[pin];

                if (io.timer == TIM_NONE) return;

                //uint32_t periodCycle = SystemCoreClock / freqHz;
                uint16_t psc = (uint16_t) ((SystemCoreClock / 1000000) -1);//(uint16_t) (periodCycle / (65536+1)); // Prescaler
                uint16_t arr = (uint16_t) (1000000 / freqHz);//(uint16_t) (periodCycle / (psc+1));   // Reload (Overflow)

                *io.timer->rcc |= io.timer->rcc_bit; // Enable rcc

                Pegasus::arch::STM32::io.pinMode(pin, PWM);
                Pegasus::arch::STM32::io.pinAFMode(io, io.timer->AF);


                (io.timer->reg)->CCER  &= ~TIM_CCER_CC1E;
                (io.timer->reg)->CCMR1 &= ~TIM_CCMR1_OC1M; // Reset
                (io.timer->reg)->CCMR1 &= ~TIM_CCMR1_CC1S; // Reset
                (io.timer->reg)->CCMR1 |= (TIM_OCMode_PWM1 | TIM_OCPreload_Enable); // Set PWM1 mode
                (io.timer->reg)->CCER  |= TIM_CCER_CC1E;


                (io.timer->reg)->CR1 &= ~TIM_CR1_CEN;  // disable timer
                (io.timer->reg)->PSC = psc;
                (io.timer->reg)->ARR = arr;
                (io.timer->reg)->EGR |= TIM_EGR_UG;   // Update generation
                (io.timer->reg)->CR1 |= TIM_CR1_ARPE;
                (io.timer->reg)->CR1 |= TIM_CR1_CEN; // Enable timer

                write(pin, 800);

            }

            void STM32RCOutput::write(const uint8_t pin, uint16_t value)
            {
                const STM32GPIO::Pin io = PINS[pin];
                if (io.timer == TIM_NONE || io.timer_channel == TIM_NONE) return;

                __IO uint32_t *CCR = &(io.timer->reg)->CCR1 + (io.timer_channel -1);
                *CCR = value;
            }

            STM32RCOutput rcout;

        } /* namespace STM32 */
    } /* namespace arch */
} /* namespace Pegasus */
