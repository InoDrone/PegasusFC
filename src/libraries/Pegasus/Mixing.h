/*
 * Mixing.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef MIXING_H_
#define MIXING_H_

#include <Pegasus.h>


#define MIX(X,Y,Z) thrust + (roll*X) + (pitch*Y) + (yaw*Z)

namespace Pegasus
{
    namespace core
    {

        class Mixing
        {
            public:
                Mixing();

                void init(uint8_t frame);
                void add(uint8_t pin, uint8_t index, uint8_t speed);
                void update(uint16_t thrust, float roll, float pitch, float yaw);
                void write();

            private:
                uint8_t outPin[MIXING_MAX_OUTPUT];
                uint16_t values[MIXING_MAX_OUTPUT];

                uint8_t frameType;
                uint8_t motorCount;
        };

        extern Mixing mix;

    } /* namespace core */
} /* namespace Pegasus */

#endif /* MIXING_H_ */
