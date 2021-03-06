/*
 * RCOutput.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "hal/stm32f4/include/RCOutput.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            RCOutput::RCOutput(Gpio* pin, pegasus::hal::gpio::AlternateFunction AF, TimerChannel* timer, uint16_t freqHz) :
                   RCOutputBase<Gpio, TimerChannel>(pin, timer, freqHz) {

                _mPin->mode(pegasus::gpio::Mode::AF_PU);
                _mPin->setAlternateFunction(AF);

                _mTimerChannel->setModeOC(freqHz); // Auto enable is function

                write(1500);
            }

            void RCOutput::write(uint16_t value) {
                _mTimerChannel->setValue(value);
            }
        }
    }
}


