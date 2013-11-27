/*
 * Mixing.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef MIXING_H_
#define MIXING_H_

#include "pegasus/core/include/Pegasus.h"
#include "pegasus/core/include/Trace.h"


#define MIX(X,Y,Z) thrust + (roll*X) + (pitch*Y) + (yaw*Z)

namespace pegasus
{
    namespace core
    {

        class Mixing
        {
            public:
                Mixing();

                void init(uint8_t frame, pegasus::hal::PortMapping* ports);
                void add(uint8_t index);
                void update(uint16_t thrust, float roll, float pitch, float yaw);
                void write();

            private:
                pegasus::hal::RCOutput*         _mRcout[MIXING_MAX_OUTPUT];
                uint16_t                        _mValues[MIXING_MAX_OUTPUT];
                pegasus::hal::PortMapping*      _mPortMapping;

                uint8_t _mFrameType;
                uint8_t _mMotorCount;
        };

        extern Mixing mix;

    } /* namespace core */
} /* namespace Pegasus */

#endif /* MIXING_H_ */
