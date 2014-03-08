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

#ifndef MAXSONAR_H_
#define MAXSONAR_H_

#include "core/include/Pegasus.h"
#include "fc/include/SonarBase.h"
#include "hal/include/PWMListener.h"

namespace pegasus {
    namespace peripherals {

        class MaxSonar : public pegasus::fc::SonarBase, public pegasus::hal::PWMListener
        {
            public:
                MaxSonar(pegasus::hal::PWMInput* pwmIn);

                bool init();
                /**
                 * Get sonar distance in cm
                 */
                uint32_t getDistance();

                void pwmUpdate(uint32_t pulseTime);

            private:
                pegasus::hal::PWMInput* _mPwmIn;
                uint32_t _mDistance;
        };

        INLINE uint32_t MaxSonar::getDistance()
        {
            return _mDistance;
        }

    } /* namespace peripherals */
} /* namespace pegasus */

#endif /* MAXSONAR_H_ */
