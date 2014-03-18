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
#include "fc/include/Config.h"
#include "fc/include/RC.h"
#include "fc/include/GyroAccBase.h"
#include "fc/include/BaroBase.h"
#include "fc/include/SonarBase.h"
#include "fc/include/Attitude.h"
#include "fc/include/Pid.h"
#include "peripherals/Led.h"
#include "hal/include/InterruptListener.h"
#include "UAVLink.h"

#define ENGINE_STATUS_INIT              _BIT(1)
#define ENGINE_STATUS_INIT_OK           _BIT(2)
#define ENGINE_STATUS_GYROACC_OK        _BIT(3)
#define ENGINE_STATUS_MAG_OK            _BIT(4)
#define ENGINE_STATUS_SONAR_OK          _BIT(5)
#define ENGINE_STATUS_BARO_OK           _BIT(6)

// Error
#define ENGINE_RCERROR                  _BIT(10)

// Flight status
#define ENGINE_ARMED                    _BIT(20)
#define ENGINE_INFLIGHT                 _BIT(21)
#define ENGINE_TAKEOFF                  _BIT(22)
#define ENGINE_LANDING                  _BIT(23)
#define ENGINE_ALT_HOLD                 _BIT(24)

#define MAX_LED 4
#define LED_YELLOW         0
#define LED_WHITE          1
#define LED_RED            2
#define LED_BLUE           3

namespace pegasus
{
    namespace fc
    {
        class Engine : public pegasus::hal::InterruptListener
        {
            public:
                Engine();

                void init(pegasus::hal::TimerBase_t* timer, RC* _rc, GyroAccBase* gyroAcc, BaroBase* baro, SonarBase* sonar);
                bool addLed(uint8_t idx, pegasus::peripherals::Led* led);

                uint8_t uavlinkReceive(const uavlink_message_t msg);

                void blinkLed(uint8_t led, uint16_t time, uint8_t repeat, bool endStatus);
                void infinyBlinkLed(uint8_t led, uint16_t time);
                void setLed(uint8_t led, bool status);

                void interrupt();

                bool is(uint32_t flag);
                void lowPriorityTask();

                void set(uint32_t flag) {
                    status |= flag;
                }

                void rm(uint32_t flag) {
                    status &= ~(flag);
                }

                Attitude attitude;

                GyroAccBase* gyroacc;
                SonarBase* sonar;
                BaroBase* baro;
                uint32_t status;

                pegasus::peripherals::Led* leds[MAX_LED];
                RC* rc;

                struct  {
                        float distance;
                        uint16_t throttle;
                } altHold;

            private:
                void initConfig();
                void initMixing(uint8_t frameType);
                void initSensors();
                void task100Hz();

                pegasus::hal::TimerBase_t* _mTimer;

                float _mThrust;
                float _mRoll;
                float _mPitch;
                float _mYaw;

                Pid _mRollPID;
                Pid _mPitchPID;
                Pid _mYawPID;

                Pid _mAltSonarPID;

                uint32_t _mCounter;
        };

        extern Engine engine;

    } /* namespace fc */
} /* namespace pegasus */

#endif /* ENGINE_H_ */
