/*
 * Mixing.cpp
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#include "Mixing.h"

namespace Pegasus
{
    namespace core
    {

        Mixing::Mixing(){}

        void Mixing::init(uint8_t frame)
        {
            frameType = frame;
        }

        void Mixing::add(uint8_t pin, uint8_t index, uint8_t speed)
        {
            if (index >= MIXING_MAX_OUTPUT) {
                Pegasus::core::trace.error("[MIXING] Add output overflow, index >= MIXING_MAX_OUTPUT");
                return;
            }

            outPin[index] = pin;
            os::rcout.add(pin, speed);

            os::rcout.write(pin, VAL_PWM_MAX);
        }

        void Mixing::update(uint16_t thrust, float roll, float pitch, float yaw)
        {
            switch (frameType) {
                case FRAME_QUAD:
                    values[0] = MIX(0 ,-1, -1); // Front
                    values[1] = MIX(-1, 0, +1); // Right
                    values[2] = MIX(0 ,+1, +1); // Rear
                    values[3] = MIX(+1, 0, -1); // Left

                    motorCount = 4;
                    break;
                case FRAME_X4:
                    values[0] = MIX(+1,-1,-1); // Front Left
                    values[1] = MIX(-1,-1,+1); // Front Right
                    values[2] = MIX(+1,+1,+1); // Rear Left
                    values[3] = MIX(-1,+1,-1); // Rear Right

                    motorCount = 4;
                    break;
            }

            for( uint8_t i=0;i<motorCount;i++) {
                os::rcout.write(outPin[i], values[i]);
            }
        }

        Mixing mix;
    } /* namespace core */
} /* namespace Pegasus */
