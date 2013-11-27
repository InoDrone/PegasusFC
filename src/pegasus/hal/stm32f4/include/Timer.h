/*
 * Timer.h
 *
 *  Created on: 24 nov. 2013
 *      Author: alienx
 */

#ifndef TIMERSTM32F4_H_
#define TIMERSTM32F4_H_

#include "pegasus/hal/stm32f4/include/Defs.h"
#include "pegasus/hal/include/TimerBase.h"

#define TIMER_CCER_MASK         0x11    //0b1011
#define TIMER_CCMR_OCXM_MASK    0x70    //0b1110000
#define TIMER_CCMR_CCXS_MASK    0x03    //0b11

#define TIM_MODE_TIMING                  ((uint16_t)0x0000)
#define TIM_MODE_ACTIVE                  ((uint16_t)0x0010)
#define TIM_MODE_INACTIVE                ((uint16_t)0x0020)
#define TIM_MODE_TOGGLE                  ((uint16_t)0x0030)
#define TIM_MODE_PWM1                    ((uint16_t)0x0060)
#define TIM_MODE_PWM2                    ((uint16_t)0x0070)

#define TIM_OCPRELOAD_ENABLE            ((uint16_t)0x0008)
#define TIM_OCPRELOAD_DISABLE            ((uint16_t)0x0000)

#define RCC_TIM2              ((uint32_t)0x00000001)
#define RCC_TIM3              ((uint32_t)0x00000002)
#define RCC_TIM4              ((uint32_t)0x00000004)
#define RCC_TIM5              ((uint32_t)0x00000008)
#define RCC_TIM6              ((uint32_t)0x00000010)
#define RCC_TIM7              ((uint32_t)0x00000020)
#define RCC_TIM12             ((uint32_t)0x00000040)
#define RCC_TIM13             ((uint32_t)0x00000080)
#define RCC_TIM14             ((uint32_t)0x00000100)


namespace pegasus {
    namespace timer {
        enum Channel {
            CHANNEL_1,
            CHANNEL_2,
            CHANNEL_3,
            CHANNEL_4
        };
    }

    namespace hal {
        namespace stm32f4 {


            class Timer : public pegasus::hal::TimerBase<TIM_TypeDef>
            {
                public:
                    Timer (TIM_TypeDef* timer);

                    void enable();
                    void disable();
                    void setFreq(uint32_t freqHz);
                    void setMod(uint8_t channel, timer::Mode mode);
                    void setValue(uint16_t channel, uint16_t value);
            };

            INLINE void Timer::enable()
            {
                _mReg->CR1 |= TIM_CR1_CEN;
            }

            INLINE void Timer::disable()
            {
                _mReg->CR1 &= ~TIM_CR1_CEN;
            }

            INLINE void Timer::setValue (uint16_t channel, uint16_t value)
            {
                __IO uint32_t *CCR = &_mReg->CCR1 + channel;
                  *CCR = value;
            }

        }
    }
}



#endif /* TIMER_H_ */
