/*
 * Engine.h
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <stdint.h>
#include "fc/include/GyroAccBase.h"
#include "fc/include/SonarBase.h"

namespace pegasus
{
    namespace fc
    {
        namespace service {
            enum Service {
                ACCGYRO_UPDATE,
                SERVICE_SIZE
            };
        }

        class Engine
        {
            public:
                Engine();

                void init(GyroAccBase* gyroAcc, SonarBase* sonar);

            private:
                void initMixing(uint8_t frameType);
                void initCommunication();
                void initSensors();


                GyroAccBase* _mGyroAcc;
                SonarBase* _mSonar;
        };

        extern Engine engine;

    } /* namespace fc */
} /* namespace pegasus */

#endif /* ENGINE_H_ */
