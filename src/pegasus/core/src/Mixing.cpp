/*
 * Mixing.cpp
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#include "pegasus/core/include/Mixing.h"

namespace pegasus
{
    namespace core
    {

        Mixing::Mixing(){}

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
            _mRcout[index]->write(VAL_PWM_MAX);
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
                    _mValues[MOTOR1] = MIX(+1,-1,-1); // Front Left
                    _mValues[MOTOR2] = MIX(-1,-1,+1); // Front Right
                    _mValues[MOTOR3] = MIX(+1,+1,+1); // Rear Left
                    _mValues[MOTOR4] = MIX(-1,+1,-1); // Rear Right

                    _mMotorCount = 4;
                    break;
            }

            for( uint8_t i=0;i<_mMotorCount;i++) {
                //os::rcout.write(outPin[i], values[i]);
            }
        }
    } /* namespace core */
} /* namespace Pegasus */
