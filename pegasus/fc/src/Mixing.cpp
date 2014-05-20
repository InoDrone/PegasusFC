/*
 * Mixing.cpp
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#include "fc/include/Mixing.h"
#include "core/include/Math.h"

namespace pegasus
{
    namespace fc
    {

        Mixing::Mixing() :
        _mMotorCount(4) {}

        void Mixing::init(uint8_t frame, pegasus::hal::PortMapping* ports)
        {
            _mFrameType = frame;
            _mPortMapping = ports;
        }

        void Mixing::add(uint8_t index)
        {
            if (index >= PWM_OUT_LENGTH) {
                pegasus::core::trace.error("[MIXING] Add output overflow, index >= MIXING_MAX_OUTPUT");
                return;
            }

            _mRcout[index] = _mPortMapping->getRCOutput(index);
            if (_mRcout[index] == 0) {
                pegasus::core::trace.error("[MIXING] No RCOUTPUT found");
                return;
            }
            _mRcout[index]->write(MIN_ESC); // Turn Off ESC
        }

        void Mixing::update(uint16_t thrust, float roll, float pitch, float yaw)
        {

            switch (_mFrameType) {
                case FRAME_QUAD:
                    _mValues[MOTOR1] = MIX(0 ,-1, -1); // Front
                    _mValues[MOTOR2] = MIX(-1, 0, +1); // Right
                    _mValues[MOTOR3] = MIX(0 ,+1, +1); // Rear
                    _mValues[MOTOR4] = MIX(+1, 0, -1); // Left

                    _mMotorCount = 4;
                    break;
                case FRAME_X4:
                    _mValues[MOTOR1] = MIX(-1,-1,-1); // Front Right
                    _mValues[MOTOR2] = MIX(+1,-1,+1); // Front Left
                    _mValues[MOTOR3] = MIX(+1,+1,-1); // Rear Left
                    _mValues[MOTOR4] = MIX(-1,+1,+1); // Rear Right

                    _mMotorCount = 4;
                    break;
            }

            for( uint8_t i=0;i<_mMotorCount;i++) {
                _mValues[i] = Math::constrain(_mValues[i], MIN_ESC, MAX_ESC);
                _mRcout[i]->write(_mValues[i]);
            }
        }

        void Mixing::write(uint16_t val, bool force) {

            if (!force) {
                val = Math::constrain(val, MIN_ESC, MAX_ESC);
            }

            for( uint8_t i=0;i<_mMotorCount;i++) {
                _mRcout[i]->write(val);
                //os::rcout.write(outPin[i], values[i]);
            }
        }

        Mixing mix;
    } /* namespace core */
} /* namespace Pegasus */
