/*
 * Gpio.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "pegasus/hal/stm32f4/include/Gpio.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            void Gpio::mode(pegasus::gpio::Mode mode)
            {
                uint32_t moder   = _mReg->MODER;
                uint32_t ospeedr = _mReg->OSPEEDR;
                uint32_t otyper  = _mReg->OTYPER;
                uint32_t pupdr   = _mReg->PUPDR;

                moder    &= ~(GPIO_MODER_MODER0 << (_mBitNumber * 2));
                ospeedr  &= ~(GPIO_OSPEEDER_OSPEEDR0  << (_mBitNumber * 2));
                otyper   &= ~(_mBitMask);
                pupdr    &= ~(GPIO_PUPDR_PUPDR0  << (_mBitNumber * 2));


                switch (mode) {
                    case gpio::Mode::OUTPUT: // Classic Ouput
                        moder   |= (0x01 << (_mBitNumber * 2));
                        ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                        // Output Type PP
                        // PuPd NOPULL
                        break;
                    case gpio::Mode::OUTPUT_OD:
                        moder   |= (0x01 << (_mBitNumber * 2));
                        ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                        otyper  |= _mBitMask;
                        // PuPd NOPULL
                        break;
                    case gpio::Mode::INPUT:
                    case gpio::Mode::INPUT_FLOATING:
                        moder |= (0x00 << (_mBitNumber * 2));
                        ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                        // Input Type PP
                        // PuPd NOPULL
                        break;
                    case gpio::Mode::INPUT_PU:
                        moder   |= (0x00 << (_mBitNumber * 2));
                        ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                        // Input Type PP
                        pupdr   |= (gpio::PuPd::PULL_UP  << (_mBitNumber * 2));
                        break;
                    case gpio::Mode::INPUT_PD:
                        moder |= (0x00 << (_mBitNumber * 2));
                        ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                        // Input Type PD
                        pupdr   |= (gpio::PuPd::PULL_DOWN  << (_mBitNumber * 2));
                        break;
                    case gpio::Mode::PWM:
                        moder |= (0x02 << (_mBitNumber * 2));
                        ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                        // Input Type PP
                        pupdr   |= (gpio::PuPd::PULL_UP  << (_mBitNumber * 2));
                        break;
                    case gpio::Mode::INPUT_ANALOG:
                        break;
                }

                _mReg->MODER   = moder;
                _mReg->OSPEEDR = ospeedr;
                _mReg->OTYPER  = otyper;
                _mReg->PUPDR   = pupdr;
            }
        }
    }
}


