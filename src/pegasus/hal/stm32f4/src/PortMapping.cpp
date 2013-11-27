/*
 * PortMapping.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "pegasus/hal/stm32f4/include/PortMapping.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            void PortMapping::add(PinInfo pinInfo, uint8_t index)
            {
                Pin pin;
                pin.pinInfo = pinInfo;
                Gpio io(pinInfo.port, pinInfo.bit);

                pin.io = &io;

                if (pinInfo.timer && pinInfo.channel != 0xFF) {
                    Timer tm(pinInfo.timer);
                    TimerChannel tc(&tm, pinInfo.channel);
                    pin.timerChannel = &tc;
                }

                _mPins[index] = pin;
            }

            RCOutput* PortMapping::getRCOutput(uint8_t index)
            {
                if (!_mPins[index].timerChannel) {
                    return 0;
                }

                if (!_mPins[index].rcout) {
                    RCOutput rc(_mPins[index].io, _mPins[index].timerChannel, _mPins[index].pinInfo.freqHz);
                    _mPins[index].rcout = &rc;
                }

                return _mPins[index].rcout;
            }

        }
    }
}


