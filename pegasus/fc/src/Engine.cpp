/*
 * Engine.cpp
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#include "fc/include/Engine.h"
#include "fc/include/Mixing.h"
#include "core/include/Trace.h"
#include "core/include/Shell.h"

namespace pegasus
{
    namespace fc
    {

        Engine::Engine() {}

        void Engine::init(GyroAccBase* gyroAcc)
        {
            _mGyroAcc = gyroAcc;

            initCommunication();
            initMixing(0); // TODO
            initSensors();
        }

        void Engine::initMixing(uint8_t frameType)
        {
            pegasus::core::trace.debug("[MIXING] Initialization");
            pegasus::fc::mix.init(frameType, &pegasus::hal::portMapping);
            switch (frameType) {
                case FRAME_QUAD:
                case FRAME_X4:
                    pegasus::core::trace.debug("[MIXING] FRAME X4");
                    /*pegasus::hal::portMapping.add(MOTOR1_PIN , MOTOR1);
                    pegasus::hal::portMapping.add(MOTOR2_PIN , MOTOR2);
                    pegasus::hal::portMapping.add(MOTOR3_PIN , MOTOR3);
                    pegasus::hal::portMapping.add(MOTOR4_PIN , MOTOR4);*/
                    /*Pegasus::core::mix.add(MOTOR1);
                    Pegasus::core::mix.add(MOTOR2);
                    Pegasus::core::mix.add(MOTOR3);
                    Pegasus::core::mix.add(MOTOR4);*/
                    break;
                default:
                    pegasus::core::trace.error("[MIXING] Error no frame found");
            }
            pegasus::core::trace.debug("[MIXING] Initialization done");
        }

        void Engine::initCommunication()
        {
            if (pegasus::core::shell.isConnected()) {
                pegasus::core::trace.debug("[SHELL] Started/Connected");
            } else {
                pegasus::core::trace.debug("[SHELL] Not implemented");
            }

            //TODO init debug serial
            pegasus::core::trace.debug("[BLUETOOTH] Not implemented");
            //TODO init serial over Bluetooth and UavLink
        }

        void Engine::initSensors()
        {
            _mGyroAcc->init();
        }

    } /* namespace fc */
} /* namespace pegasus */
