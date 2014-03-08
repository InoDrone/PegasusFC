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

#include "Led.h"

namespace pegasus
{
    namespace peripherals
    {

        Led::Led(pin _pin) :
                _io(_pin.port, _pin.bit),
                _mTime(0),
                _mRepeat(0),
                _mMillis(0),
                _mEndStatus(false),
                _mNoStop(false)
        {
            _io.mode(Mode::OUTPUT);
        }

        Led::Led(portNumber_t port, bitNumber_t bit) :
                _io(port, bit),
                _mTime(0),
                _mRepeat(0),
                _mMillis(0),
                _mEndStatus(false) {

            _io.mode(Mode::OUTPUT);

        }

        void Led::blink(uint16_t time, uint8_t repeat, bool endStatus)
        {
            _mTime = time;
            _mRepeat = repeat;
            _mMillis = pegasus::core::mainTimer.millis();
            _mEndStatus = endStatus;

            _io.low();
        }

        void Led::infiny(uint16_t time) {

            if (_mNoStop) return;

            _mTime = time;
            _mRepeat = 0xFF;
            _mMillis = pegasus::core::mainTimer.millis();
            _mNoStop = true;

            _io.low();
        }

        void Led::set(bool high)
        {
            _mRepeat = 0;
            _mNoStop = false;

            if (high) _io.high();
            else _io.low();
        }

        void Led::update()
        {
            if (!_mNoStop && _mRepeat == 0) return;
            uint32_t millis = pegasus::core::mainTimer.millis();

            if ( (millis-_mMillis) >= _mTime ) {
                    _io.toggle();
                    _mMillis = millis;
                    _mRepeat--;

                    if (_mRepeat == 0 ) {
                        set(_mEndStatus);
                        _mEndStatus = false;
                    }
            }
        }

    } /* namespace peripherals */
} /* namespace pegasus */
