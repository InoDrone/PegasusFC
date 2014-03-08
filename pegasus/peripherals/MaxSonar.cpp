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

#include "MaxSonar.h"
#include "core/include/Trace.h"

namespace pegasus {
    namespace peripherals {

        MaxSonar::MaxSonar(pegasus::hal::PWMInput* pwmIn) :
                _mDistance(0),
                _mPwmIn(pwmIn)
        {

        }

        bool MaxSonar::init() {
            pegasus::core::trace.log("[MAXSONAR] Initialization");
            _mPwmIn->attachInterrupt(this);
            pegasus::core::trace.log("[MAXSONAR] Initialization done");

            return true;
        }

        void MaxSonar::pwmUpdate(uint32_t pulseTime)
        {
            _mDistance = (uint32_t)(pulseTime/58);
        }

    } /* namespace peripherals */
} /* namespace pegasus */
