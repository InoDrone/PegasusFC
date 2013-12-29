/*
 * Engine.h
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <stdint.h>
#include "core/include/Pegasus.h"
#include "fc/include/Defs.h"
#include "fc/include/GyroAccBase.h"
#include "fc/include/SonarBase.h"
#include "fc/include/Attitude.h"
#include "hal/include/InterruptListener.h"
#include "UAVLink.h"

namespace pegasus
{
    namespace fc
    {
        class Engine : public pegasus::hal::InterruptListener
        {
            public:
                Engine();

                void init(pegasus::hal::TimerBase_t* timer, GyroAccBase* gyroAcc, SonarBase* sonar);

                uint8_t uavlinkReceive(const uavlink_message_t msg);

                void interrupt();

                Attitude attitude;

                GyroAccBase* gyroacc;
                SonarBase* sonar;
                //RCInput* rc;

            private:
                void initMixing(uint8_t frameType);
                void initCommunication();
                void initSensors();

                pegasus::hal::TimerBase_t* _mTimer;
        };

        extern Engine engine;

    } /* namespace fc */
} /* namespace pegasus */

#endif /* ENGINE_H_ */
