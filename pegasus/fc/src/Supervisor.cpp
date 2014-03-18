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

#include "fc/include/Supervisor.h"
#include "peripherals/MaxSonar.h"
#include "core/include/CriticalSection.h"

namespace pegasus
{
    namespace fc
    {

        using namespace pegasus::core;

        Supervisor::Supervisor() :
                _mIsArmed(false)
        {
            _mAltHold = {false, 0.0f, 0, 5};
        }

        bool Supervisor::enableAltHold()
        {

            if (_mAltHold.enabled) return false; /* if enabled, quit */

            uint8_t type = ALTHOLD_BARO_TYPE;

           /* float alt = engine.sonar->getDistance();

            if (!engine.is(ENGINE_STATUS_SONAR_OK)
                    || alt < 100
                    || alt > MAX_SONAR_ALT) {  // if alt < 100m or alt > Max sonar range, quit

                type = ALTHOLD_BARO_TYPE;
            }*/

            if (!engine.is(ENGINE_STATUS_BARO_OK)) {
                engine.blinkLed(LED_RED, 50, 10, false);

                return false;
            }

            {   /* Enter critical section */
                CriticalSection cs;

                if (type == ALTHOLD_BARO_TYPE) {
                    _mAltHold.distance = engine.baro->getAltitude();
                } else {
                    _mAltHold.distance = engine.sonar->getDistance();
                }

                _mAltHold.throttle = engine.rc->throttle.getInput();
                _mAltHold.type = type;
                _mAltHold.enabled = true;
            }   /* Leave critical section */

            return true;
        }

        void Supervisor::disableAltHold()
        {
            _mAltHold.enabled = false;
        }

        float Supervisor::getAltHoldThrottle(Pid* pid, float G_Dt)
        {
            if (!_mAltHold.enabled) {
                return engine.rc->throttle.getInput();
            }

            if (_mAltHold.type == ALTHOLD_BARO_TYPE && !engine.is(ENGINE_STATUS_BARO_OK)) {
                return engine.rc->throttle.getInput();
            } else if (_mAltHold.type == ALTHOLD_SONAR_TYPE && !engine.is(ENGINE_STATUS_SONAR_OK)) {
                return engine.rc->throttle.getInput();
            }

            float throttle;
            switch(_mAltHold.type){
                case ALTHOLD_BARO_TYPE:
                    throttle = _mAltHold.throttle + pid->calculate (_mAltHold.distance,engine.baro->getAltitude() , G_Dt);
                    break;
                case ALTHOLD_SONAR_TYPE:
                    throttle = _mAltHold.throttle + pid->calculate (_mAltHold.distance, engine.sonar->getDistance(), G_Dt);
                    break;
                default:
                    throttle = engine.rc->throttle.getInput();
                    break;
            }

            return throttle;
        }

        bool Supervisor::arm()
        {
            if (_mIsArmed) return false; /* if armed, quit !*/

            if (engine.rc->aux1.isOn) {
                engine.blinkLed(LED_RED, 50, 20, false);
                engine.blinkLed(LED_WHITE, 100, 10, false);
                return false;
            }

            return true;
        }

        bool Supervisor::disarm()
        {
            if (!_mIsArmed) return false; /* if disarmed, quit !*/

            sv.disableAltHold();

            return true;
        }

        Supervisor sv;

    } /* namespace core */
} /* namespace pegasus */
