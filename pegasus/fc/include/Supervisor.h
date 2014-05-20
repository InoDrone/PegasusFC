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

#ifndef SUPERVISOR_H_
#define SUPERVISOR_H_

#include "fc/include/Defs.h"
#include "fc/include/Engine.h"
#include "fc/include/Pid.h"

#define ALTHOLD_SONAR_TYPE 0
#define ALTHOLD_BARO_TYPE  1

struct altHold_t {
        bool enabled;

        float distance;
        uint16_t throttle;

        uint8_t type;
};


namespace pegasus
{
    namespace fc
    {

        class Supervisor
        {
            public:
                Supervisor();


                /* Altitude Hold functions */
                bool enableAltHold();
                void disableAltHold();
                bool altHoldEnabled() {
                    return _mAltHold.enabled;
                }
                float getAltHoldThrottle(Pid* pid, float G_Dt);


                /* ARM ENGINE */
                bool isArmed() {
                    return _mIsArmed;
                }
                void setArmed(bool farm) {
                    bool exec = farm ? arm() : disarm();

                    if (!exec) return;

                    _mIsArmed = farm;
                }

                uint16_t getAttitudeLoopTime() const
                {
                	return attitudeLoopTime;
                }

                void beginAttitudeLoop(pegasus::hal::TimerBase_t* timer);
                void endAttitudeLoop(pegasus::hal::TimerBase_t* timer);


            private:
                altHold_t _mAltHold;

                bool _mIsArmed;



                bool arm();
                bool disarm();

                uint16_t attitudeLoopTime;
                uint32_t attitudeCalculatorStart;
        };

        extern Supervisor sv;

    } /* namespace core */
} /* namespace pegasus */

#endif /* SUPERVISOR_H_ */
