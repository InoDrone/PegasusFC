/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author: Marc Jacquier <marc@inodrone.com>
 *  Project: InoDronePegasus
 */

#ifndef PWMINPUT_H_
#define PWMINPUT_H_

#include "hal/stm32f4/include/Defs.h"
#include "hal/stm32f4/include/Gpio.h"
#include "hal/stm32f4/include/TimerChannel.h"
#include "hal/include/InterruptListener.h"
#include "hal/include/PWMListener.h"

namespace pegasus {
    namespace hal {

        namespace pwm {
            struct PWMConfig {
                    pegasus::hal::gpio::pin                 pwmPin;
                    pegasus::hal::gpio::AlternateFunction   AF;
                    uint16_t                                freqHz;
            };
        }

        namespace stm32f4 {

            class PWMInput : public pegasus::hal::InterruptListener
            {

                public:
                    PWMInput(Gpio* io, pegasus::hal::gpio::AlternateFunction AF, TimerChannel* timerChannel, uint16_t freqHz);

                    void interrupt();
                    bool attachInterrupt(pegasus::hal::PWMListener* listener);

                protected:
                    TimerChannel* _mTimerChannel;
                    Gpio* _mPin;

                private:
                    pegasus::hal::PWMListener* _mTimerListener;
                    uint32_t _mStartTime;
            };

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */

#endif /* PWMINPUT_H_ */
