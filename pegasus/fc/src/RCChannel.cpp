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

#include "fc/include/RCChannel.h"
#include "core/include/MainTimer.h"
#include "core/include/Math.h"
#include "core/include/Trace.h"

namespace pegasus
{
    namespace fc
    {

        bool RCChannel::init(uint8_t portIdx) {
            _mIn = pegasus::hal::portMapping.getPWMInput(portIdx);
            if (!_mIn) {
                pegasus::core::trace.error("[RCCHANNEL] pin %d not found.", portIdx);
                return false;
            }

            _mIn->attachInterrupt(this);
            pegasus::core::trace.debug("[RCCHANNEL] pin %d added (addr: 0x%X)", portIdx, &_mIn);
            return true;
        }

        bool RCChannel::init(uint8_t portIdx, uint16_t min, uint16_t center, uint16_t max)
        {
            _mRadioMin = min;
            _mRadioMax = max;
            _mRadioCenter = center;

            return init(portIdx);
        }

        bool RCChannel::isValid()
        {
            uint32_t millis = pegasus::core::mainTimer.millis();

            // if last update > 20 ms ERROR
            if ((millis-_mLastUpdate) > 25) return false;

            // TODO Yes or Not ??
            if (_mRadioIn < _mRadioMin || _mRadioIn > _mRadioMax) return false;

            return true;
        }

        void RCChannel::setRange(uint16_t min, uint16_t max)
        {
            _mType = RCChannel::RANGE;
            _mHigh = max;
            _mLow = min;
        }

        void RCChannel::setAngle(uint16_t angleDeg)
        {
            _mType = RCChannel::ANGLE;
            _mAngle = angleDeg;
        }

        void RCChannel::setSwitch()
        {
            _mType = RCChannel::SWITCH;
        }


        uint16_t RCChannel::getRange()
        {
            int16_t rcIn = Math::constrain(_mRadioIn, _mRadioMin, _mRadioMax);

            if (rcIn > _mRadioMin) {
                    return (_mLow + ((long)(_mHigh - _mLow) * (long)(rcIn - _mRadioMin)) / (long)( _mRadioMax - _mRadioMin));
            } else {
                    return _mLow;
            }
        }

        uint16_t RCChannel::getAngle()
        {
            int16_t high_center = _mRadioCenter + 15;
            int16_t low_center = _mRadioCenter - 15;

            if ((low_center - _mRadioMin) == 0 || (_mRadioMax - high_center) ==0) {
                    return 0;
            }

            if (_mRadioIn > high_center) {
                    return ((long)_mAngle * (long)(_mRadioIn - high_center)) / (long)(_mRadioMax - high_center);
            } else if (_mRadioIn < low_center) {
                    return ((long)_mAngle * (long)(_mRadioIn - low_center)) / (long)(low_center - _mRadioMin);
            } else {
                    return 0;
            }
        }

        bool RCChannel::getBoolean()
        {
            return (_mRadioIn > (_mRadioCenter+150));
        }

        void RCChannel::pwmUpdate(uint32_t pulseTime)
        {
            _mLastUpdate = pegasus::core::mainTimer.millis();
            _mRadioIn = pulseTime;

            isOn = 0;

            if (_mType == RCChannel::RANGE) {
                value = getRange();
            } else if (_mType == RCChannel::ANGLE) {
                value = getAngle();
            } else if (_mType == RCChannel::SWITCH) {
                isOn = getBoolean();
            }
        }

    } /* namespace fc */
} /* namespace pegasus */
