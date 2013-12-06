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

#include "hal/stm32f4/include/PWMInput.h"
#include "hal/stm32f4/include/InterruptRegister.h"
#include "core/include/Trace.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            PWMInput::PWMInput(pegasus::hal::pwm::PWMConfig pwmConfig, TimerChannel* timerChannel) :
                    _mTimerChannel(timerChannel),
                    _mPin(pwmConfig.pwmPin.port, pwmConfig.pwmPin.bit)
            {

                _mPin.mode(pegasus::gpio::Mode::AF_PD);
                _mPin.setAlternateFunction(pwmConfig.AF);

                _mTimerChannel->setModeIC(pwmConfig.freqHz);
                InterruptRegister::attachTimerInt(this, _mTimerChannel);

            }

            void PWMInput::interrupt()
            {
                TIM_TypeDef* reg = _mTimerChannel->getTimer()->getReg();
                uint8_t channel = _mTimerChannel->getChannel();
                __IO uint32_t* ccrx = &reg->CCR1 + channel;
                uint16_t time = *ccrx;
                bool isRising = (reg->CCER & (0x02 << (channel * 4))) == 0;

                if (isRising) {
                    _mStartTime = time;
                } else {
                    uint16_t period;
                    if (_mStartTime > time) {
                        period = ((time + reg->ARR) - _mStartTime); // Overflow timer
                    } else {
                        period = (time - _mStartTime);
                    }

                    if (_mTimerListener) {
                        pegasus::hal::PWMListener::trampoline(_mTimerListener, period);
                    }
                }

                reg->CCER ^= (0x02 << (channel * 4));
            }

            bool PWMInput::attachInterrupt(pegasus::hal::PWMListener* listener)
            {
                if (_mTimerListener) return false;

                _mTimerListener = listener;

                return true;
            }

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */
