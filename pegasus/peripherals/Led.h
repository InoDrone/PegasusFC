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

#ifndef LED_H_
#define LED_H_

#include "core/include/Pegasus.h"
#include "core/include/MainTimer.h"

namespace pegasus {
    namespace peripherals {

        using namespace pegasus::gpio;

        class Led
        {
            public:
                Led(pin);
                Led(portNumber_t, bitNumber_t);

                void blink(uint16_t time, uint8_t repeat, bool endStatus=false);
                void infiny(uint16_t time);
                void set(bool high);

                void update();

            private:
                pegasus::hal::Gpio _io;
                uint16_t _mTime;
                uint8_t _mRepeat;
                uint32_t _mMillis;
                bool _mEndStatus;
                bool _mNoStop;
        };

    } /* namespace peripherals */
} /* namespace pegasus */

#endif /* LED_H_ */
