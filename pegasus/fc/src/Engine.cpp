/*
 * Engine.cpp
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#include "fc/include/Engine.h"
#include "fc/include/Mixing.h"
#include "core/include/Trace.h"

namespace pegasus
{
    namespace fc
    {

        Engine::Engine() :
                attitude() {}

        void Engine::init(pegasus::hal::TimerBase_t* timer,GyroAccBase* gyroAcc, SonarBase* sonar)
        {
            this->gyroacc = gyroAcc;
            this->sonar = sonar;
            _mTimer = timer;


            initCommunication();
            initMixing(0); // TODO
            initSensors();


            // Init Engine Timer
            _mTimer->disable();
            _mTimer->setFreq(200); // 200hz
            _mTimer->enable();
            pegasus::hal::InterruptRegister::attachTimerInt(this, _mTimer);
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
            /*if (pegasus::core::shell.isConnected()) {
                pegasus::core::trace.debug("[SHELL] Started/Connected");
            } else {
                pegasus::core::trace.debug("[SHELL] Not implemented");
            }*/

            pegasus::core::trace.debug("[BLUETOOTH] Not implemented");
            //TODO init serial over Bluetooth and UavLink
        }

        void Engine::initSensors()
        {
            gyroacc->init();
            sonar->init();
            //mag->init();
            //rc->init();

            gyroacc->setAutoSampling(true);
        }

        uint8_t Engine::uavlinkReceive(const uavlink_message_t msg)
        {
            return 0;
        }

        /**
         * Timer interrupt 200hz
         *
         * - Sum sensors
         * - Compute DCM
         * - Calculate PID
         * - Write to out (motor, servos, ...)
         *
         */
        void Engine::interrupt()
        {
            attitude.update();
        }



        Engine engine;

    } /* namespace fc */
} /* namespace pegasus */
