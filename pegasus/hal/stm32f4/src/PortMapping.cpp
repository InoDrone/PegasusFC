/*
 * PortMapping.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "hal/stm32f4/include/PortMapping.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            void PortMapping::add(uint8_t index)
            {
                PinInfo inf = PORTMAP[index];

                Pin pin;
                pin.pinInfo = inf;
               // Gpio io(inf.port, inf.bit);

                pin.io = new Gpio(inf.port, inf.bit);

                if (inf.timer && inf.channel != 0xFF) {
                    //TimerChannel tc(inf.timer, inf.channel);
                    pin.timerChannel = new TimerChannel(inf.timer, inf.channel);// tc;
                }

                _mPins[index] = pin;
            }

            RCOutput* PortMapping::getRCOutput(uint8_t index)
            {
                if (!_mPins[index].timerChannel) {
                    return 0;
                }

                if (!_mPins[index].out) {
                    //RCOutput rc(_mPins[index].io, _mPins[index].timerChannel, _mPins[index].pinInfo.freqHz);
                    _mPins[index].out = new RCOutput(_mPins[index].io, _mPins[index].pinInfo.AF, _mPins[index].timerChannel, _mPins[index].pinInfo.freqHz); //&rc;
                }

                return _mPins[index].out;
            }

            PWMInput* PortMapping::getPWMInput(uint8_t index)
            {
                if (!_mPins[index].timerChannel) {
                    return 0;
                }

                if (!_mPins[index].in) {
                    //RCOutput rc(_mPins[index].io, _mPins[index].timerChannel, _mPins[index].pinInfo.freqHz);
                    _mPins[index].in = new PWMInput(_mPins[index].io, _mPins[index].pinInfo.AF, _mPins[index].timerChannel, _mPins[index].pinInfo.freqHz); //&rc;
                }

                return _mPins[index].in;
            }

        }
    }
}


