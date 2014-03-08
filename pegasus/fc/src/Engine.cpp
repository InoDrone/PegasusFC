/*
 * Engine.cpp
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#include <string.h>

#include "fc/include/Engine.h"
#include "fc/include/Mixing.h"
#include "core/include/Trace.h"
#include "core/include/Math.h"

namespace pegasus
{
    namespace fc
    {

        Engine::Engine() :
                attitude(),
                _mThrust(0),
                _mRoll(0),
                _mPitch(0),
                _mYaw(0),
                _mCounter(0),
                telemetryEnable(false) {}

        void Engine::init( pegasus::hal::TimerBase_t* timer,
                RC* rc,
                GyroAccBase* gyroAcc,
                SonarBase* sonar)
        {
            status |= ENGINE_STATUS_INIT;
            setLed(LED_YELLOW, true);

            this->rc = rc;
            this->gyroacc = gyroAcc;
            this->sonar = sonar;
            _mTimer = timer;

            initConfig(); // Load config

            _mRollPID.setConfig(p.lvlRollPID, p.rateRollPID);
            _mPitchPID.setConfig(p.lvlPitchPID, p.ratePitchPID);
            _mYawPID.setConfig(p.lvlYawPID, p.rateYawPID);

            initMixing(p.frameType);

            initSensors();


            // Init Engine Timer
            _mTimer->disable();
            _mTimer->setFreq(200); // 200hz
            _mTimer->enable();
            pegasus::hal::InterruptRegister::attachTimerInt(this, _mTimer);

            rm(ENGINE_STATUS_INIT);
            set(ENGINE_STATUS_INIT_OK);


            setLed(LED_YELLOW, false);
            blinkLed(LED_BLUE, 100, 20, true);
        }

        bool Engine::addLed(uint8_t idx, pegasus::peripherals::Led* led)
        {
            if (idx >= MAX_LED) {
                pegasus::core::trace.error("[ENGINE] Error adding led %d > MAX_LED(%d)", idx, MAX_LED);
                return false;
            }

            leds[idx] = led;

            return true;
        }

        void Engine::initConfig()
        {
            Config_t tmp;
            memset((uint32_t *)&p, 0, sizeof(p));
            loadDefaultConfig(&p);

            pegasus::hal::ds.read( (uint32_t*)&tmp, sizeof(tmp));

            if (tmp.version > p.version) {
                p = tmp;
            } else {
                pegasus::hal::ds.save( (uint32_t*)&p, sizeof(p));
            }
        }

        void Engine::initMixing(uint8_t frameType)
        {
            pegasus::core::trace.debug("[MIXING] Initialization");
            pegasus::fc::mix.init(frameType, &pegasus::hal::portMapping);
            switch (frameType) {
                case FRAME_QUAD:
                case FRAME_X4:
                    pegasus::core::trace.debug("[MIXING] QUAD or X4");
                    mix.add(MOTOR1);
                    mix.add(MOTOR2);
                    mix.add(MOTOR3);
                    mix.add(MOTOR4);
                    break;
                default:
                    pegasus::core::trace.error("[MIXING] Error no frame found");
            }
            pegasus::core::trace.debug("[MIXING] Initialization done");
        }

        void Engine::initSensors()
        {
            if (gyroacc->init()) {
                status |= ENGINE_STATUS_GYROACC_OK;
            }

            if (sonar->init()) {
                status |= ENGINE_STATUS_SONAR_OK;
            }
            //mag->init();
            //rc->init();

            gyroacc->setAutoSampling(true);
        }

        uint8_t Engine::uavlinkReceive(const uavlink_message_t msg)
        {
            uint8_t ack = 0;
            switch(msg.cmd) {
                case UAVLINK_MSG_PID:
                    uavlink_message_pid_t pid;
                    uavlink_message_pid_decode(&msg, &pid);

                    p.lvlPitchPID.kP = (pid.stabKP / 100.0f);
                    p.lvlPitchPID.kI = (pid.stabKI / 100.0f);
                    p.ratePitchPID.kP = (pid.pitchKP / 100.0f);
                    p.ratePitchPID.kI = (pid.pitchKI / 100.0f);
                    p.ratePitchPID.kD = (pid.pitchKD / 100.0f);

                    p.lvlRollPID.kP = (pid.stabKP / 100.0f);
                    p.lvlRollPID.kI = (pid.stabKI / 100.0f);
                    p.rateRollPID.kP = (pid.rollKP / 100.0f);
                    p.rateRollPID.kI = (pid.rollKI / 100.0f);
                    p.rateRollPID.kD = (pid.rollKD / 100.0f);

                    //p.lvlYawPID.kP = (pid.stabKP / 100.0f);
                    //p.lvlYawPID.kI = (pid.stabKI / 100.0f);
                    p.rateYawPID.kP = (pid.yawKP / 100.0f);
                    p.rateYawPID.kI = (pid.yawKI / 100.0f);
                    p.rateYawPID.kD = (pid.yawKD / 100.0f);

                    _mRollPID.setConfig(p.lvlRollPID, p.rateRollPID);
                    _mPitchPID.setConfig(p.lvlPitchPID, p.ratePitchPID);
                    _mYawPID.setConfig(p.lvlYawPID, p.rateYawPID);

                    ack = msg.cmd + msg.len;
                    blinkLed(LED_YELLOW, 50, 10, false);
                    break;

                case UAVLINK_STATE:
                    switch(msg.datas[0]) {
                        case UAVLINK_RCCALIBRATION:
                            set(ENGINE_RCCALIBRATION);
                            ack = msg.cmd + ENGINE_RCCALIBRATION;
                            break;
                        case UAVLINK_ESCCALIBRATION:
                            if (!is(ENGINE_ARMED)) {
                                set(ENGINE_ESCCALIBRATION);
                                ack = msg.cmd + ENGINE_ESCCALIBRATION;
                            } else {
                                blinkLed(LED_RED, 50, 10, false);
                            }
                            break;
                        case UAVLINK_SYS_SAVECONFIG:
                            if (!is(ENGINE_ARMED)) {
                                pegasus::hal::ds.save((uint32_t*)&p, sizeof(p));
                                blinkLed(LED_YELLOW, 10, 10, false);
                            } else {
                                blinkLed(LED_RED, 50, 10, false);
                            }
                            break;
                        case UAVLINK_PING:
                            telemetryEnable = true;
                            break;
                    }
                    break;
            }

            return ack;
        }

        /**
         * Timer interrupt 200hz
         *
         * - Sum sensors
         * - Compute DCM
         * - Calculate PID
         * - Write to out (motor, servos, ...)
         *
         * G_Dt = 1/200 = 0.005s
         *
         */
        void Engine::interrupt()
        {
            rc->check();
            attitude.update();

            Attitude_t att = attitude.getAttitude();

            _mRoll  = _mRollPID.calculate(DEG2RAD(rc->roll.value), att.euler.roll, att.gyro.x, 0.005f);
            _mPitch = _mPitchPID.calculate(DEG2RAD(rc->pitch.value), att.euler.pitch, att.gyro.y, 0.005f);
            _mYaw   = _mYawPID.calculate(DEG2RAD(rc->yaw.value), att.gyro.z, 0.005f);

            if (is(ENGINE_ARMED)) {
                mix.update(Math::constrain(rc->throttle.value, IDLE_ESC, MAX_ESC) , _mRoll, _mPitch, _mYaw);
            } else if (!is(ENGINE_ESCCALIBRATION)) {
                mix.write(VAL_PWM_MIN);
            }

            _mCounter++;
        }

        /**
         * Check engine flags status
         */
        bool Engine::is(uint32_t flags)
        {
            return ((status & flags) == flags);
        }

        /**
         * Exec by FC Thread
         *
         * - Blink led
         */
        void Engine::lowPriorityTask()
        {
            uint8_t i =0;
            for(;i<MAX_LED;i++) {
                if (leds[i]) leds[i]->update();
            }

            // Check RC ERROR
            if (rc->hasError()) {
                infinyBlinkLed(LED_RED, 100);
                setLed(LED_BLUE, false);
                status |= ENGINE_RCERROR;
            } else if (is(ENGINE_RCERROR)) {
                setLed(LED_RED, false);
                blinkLed(LED_BLUE, 50, 5, true);
                status &= ~(ENGINE_RCERROR);
            }

            if (is(ENGINE_ARMED)) {
                setLed(LED_RED, true);
            } else if (is(ENGINE_ESCCALIBRATION)) {
                infinyBlinkLed(LED_RED, 500);
            } else if (!is(ENGINE_RCERROR)) {
                setLed(LED_RED, false);
            }

        }

        void Engine::blinkLed(uint8_t led, uint16_t time, uint8_t repeat, bool endStatus)
        {
            if (leds[led]) {
                leds[led]->blink(time, repeat, endStatus);
            }
        }

        void Engine::infinyBlinkLed(uint8_t led, uint16_t time)
        {
            if (leds[led]) {
                leds[led]->infiny(time);
            }
        }

        void Engine::setLed(uint8_t led, bool status)
        {
            if (leds[led]) {
                leds[led]->set(status);
            }
        }

        Engine engine;

    } /* namespace fc */
} /* namespace pegasus */
