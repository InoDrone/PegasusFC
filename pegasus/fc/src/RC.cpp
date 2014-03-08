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

#include "fc/include/RC.h"

namespace pegasus{
    namespace fc {

        RC::RC() :
        _mValidChannel(0),
        _mSignalError(false)
        {
            throttle.setRange(VAL_PWM_MIN, VAL_PWM_MAX);

            roll.setAngle(90); // -90 -> 90
            pitch.setAngle(90);
            yaw.setAngle(180); // -180 -> 180

        }

        void RC::check()
        {
            uint8_t valid = 0;
            if (throttle.isValid()) valid++;
            if (roll.isValid()) valid++;
            if (pitch.isValid()) valid++;
            if (yaw.isValid()) valid++;
            if (aux1.isValid()) valid++;
            if (aux2.isValid()) valid++;

            if (valid < _mValidChannel || valid == 0) {
                _mSignalError = true;
            } else {
                _mSignalError = false;
            }

            _mValidChannel = valid;
        }

        RC rc;

    } /* namespace fc */
} /* namespace pegasus */
