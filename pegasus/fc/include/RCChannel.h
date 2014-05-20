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

#ifndef RCCHANNEL_H_
#define RCCHANNEL_H_

#include "core/include/Pegasus.h"
#include "hal/include/PWMListener.h"

namespace pegasus {
    namespace fc {

        class RCChannel : public pegasus::hal::PWMListener
        {
            public:
                RCChannel() :
                    _mRadioIn(0),
                    _mLastUpdate(0),
                    _mRadioMin(VAL_PWM_MIN),
                    _mRadioMax(VAL_PWM_MAX),
                    _mRadioCenter(VAL_PWM_CENTER),
                    _mType(DISABLE) {};

                bool init(uint8_t portIdx);
                bool init(uint8_t portIdx, uint16_t min, uint16_t center, uint16_t max);
                void pwmUpdate(uint32_t pulseTime);

                bool isValid();

                void setRange(uint16_t min, uint16_t max);
                void setAngle(uint16_t angleDeg);
                void setSwitch();

                void setConfig (uint16_t min, uint16_t center, uint16_t max);

                uint16_t getInput() {
                    return _mRadioIn;
                }

                int16_t value;
                bool isOn;

                enum Type {
                    DISABLE,
                    RANGE,
                    ANGLE,
                    SWITCH
                };

            private:
                pegasus::hal::PWMInput* _mIn;
                uint16_t _mRadioIn;
                uint32_t _mLastUpdate;

                uint16_t _mRadioMin;
                uint16_t _mRadioMax;
                uint16_t _mRadioCenter;
                uint16_t _mHigh;
                uint16_t _mLow;
                uint16_t _mAngle;

                uint8_t _mType;

                uint16_t getRange();
                uint16_t getAngle();
                bool getBoolean();
        };

    } /* namespace fc */
} /* namespace pegasus */

#endif /* RCCHANNEL_H_ */
