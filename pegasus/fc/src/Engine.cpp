/*
 * Engine.cpp
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#include <string.h>

#include "fc/include/Engine.h"
#include "fc/include/Mixing.h"
#include "fc/include/Supervisor.h"
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
                _mCounter(0) {}

        void Engine::init( pegasus::hal::TimerBase_t* timer,
                RC* rc,
                GyroAccBase* gyroAcc,
                BaroBase* baro,
                SonarBase* sonar,
                GPSBase* gpsPeriph)
        {
            status |= ENGINE_STATUS_INIT;
            setLed(LED_YELLOW, true);

            this->rc = rc;
            this->gyroacc = gyroAcc;
            this->baro = baro;
            this->sonar = sonar;
            this->gps = gpsPeriph;
            _mTimer = timer;

            initConfig(); // Load config

            altHold = {0.0f, 0};

            _mRollPID.setConfig(p.lvlRollPID, p.rateRollPID);
            _mPitchPID.setConfig(p.lvlPitchPID, p.ratePitchPID);
            _mYawPID.setConfig(p.lvlYawPID, p.rateYawPID);
            _mAltSonarPID.setConfig(p.altSonarPID);

            rc->throttle.setConfig(p.calRcThrottle.min, p.calRcThrottle.center, p.calRcThrottle.max);
            rc->roll.setConfig(p.calRcRoll.min, p.calRcRoll.center, p.calRcRoll.max);
            rc->pitch.setConfig(p.calRcPitch.min, p.calRcPitch.center, p.calRcPitch.max);
            rc->yaw.setConfig(p.calRcYaw.min, p.calRcYaw.center, p.calRcYaw.max);

            initMixing(p.frameType);

            initSensors();

            // Init Engine Timer
            _mTimer->disable();
            _mTimer->setFreq(ATT_LOOP_FREQ); // 200hz or high speed 400Hz
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

            if (tmp.version > p.version && tmp.version != 0xFF && p.version != 1) {
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

            if (baro->init()) {
                status |= ENGINE_STATUS_BARO_OK;
            }

            if (sonar->init()) {
                status |= ENGINE_STATUS_SONAR_OK;
            }

            if (gpsTask.init(gps)) {
            	status |= ENGINE_STATUS_GPS_OK;
            	gpsTask.start(); // Start GPS thread
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

                    memcpy(&p.lvlPitchPID, &pid.level, sizeof(pid.level));
                    memcpy(&p.lvlRollPID, &pid.level, sizeof(pid.level));
                    memcpy(&p.ratePitchPID, &pid.tilt, sizeof(pid.tilt));
                    memcpy(&p.rateRollPID, &pid.tilt, sizeof(pid.tilt));
                    memcpy(&p.rateYawPID, &pid.yaw, sizeof(pid.yaw));
                    memcpy(&p.altSonarPID, &pid.altSonar, sizeof(pid.altSonar));

                    _mRollPID.setConfig(p.lvlRollPID, p.rateRollPID);
                    _mPitchPID.setConfig(p.lvlPitchPID, p.ratePitchPID);
                    _mYawPID.setConfig(p.lvlYawPID, p.rateYawPID);

                    _mAltSonarPID.setConfig(p.altSonarPID);

                    ack = msg.cmd + msg.len;
                    blinkLed(LED_YELLOW, 50, 10, false);
                    break;
                case UAVLINK_MSG_RC_CALIBRATION:
                    uavlink_message_rc_calibration_t rcCalib;
                    uavlink_message_rc_calibration_decode(&msg, &rcCalib);

                    memcpy(&p.calRcThrottle, &rcCalib.throttle, sizeof(rcCalib.throttle));
                    memcpy(&p.calRcRoll, &rcCalib.roll, sizeof(rcCalib.roll));
                    memcpy(&p.calRcPitch, &rcCalib.pitch, sizeof(rcCalib.pitch));
                    memcpy(&p.calRcYaw, &rcCalib.yaw, sizeof(rcCalib.yaw));

                    rc->throttle.setConfig(p.calRcThrottle.min, p.calRcThrottle.center, p.calRcThrottle.max);
                    rc->roll.setConfig(p.calRcRoll.min, p.calRcRoll.center, p.calRcRoll.max);
                    rc->pitch.setConfig(p.calRcPitch.min, p.calRcPitch.center, p.calRcPitch.max);
                    rc->yaw.setConfig(p.calRcYaw.min, p.calRcYaw.center, p.calRcYaw.max);

                    ack = msg.cmd + msg.len;
                    blinkLed(LED_YELLOW, 50, 10, false);
                    break;
                case UAVLINK_STATE:
                    switch(msg.datas[0]) {
                        case UAVLINK_SYS_SAVECONFIG:
                            if (!is(ENGINE_ARMED)) {
                                pegasus::hal::Processor::disableInterrupts();
                                p.version++;
                                pegasus::hal::ds.save((uint32_t*)&p, sizeof(p));
                                pegasus::hal::Processor::enableInterrupts();
                                blinkLed(LED_WHITE, 50, 5, false);
                            } else {
                                blinkLed(LED_RED, 50, 10, false);
                            }
                            break;
                        case UAVLINK_SYS_PID:
                            uavlink_message_pid_t pids;
                            memcpy(&pids.level, &p.lvlRollPID, sizeof(pids.level));
                            memcpy(&pids.tilt, &p.ratePitchPID, sizeof(pids.tilt));
                            memcpy(&pids.yaw, &p.rateYawPID, sizeof(pids.yaw));
                            memcpy(&pids.altSonar, &p.altSonarPID, sizeof(pids.altSonar));

                            pegasus::core::com.send(uavlink_message_pid_encode(&pids));
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
        	sv.beginAttitudeLoop(_mTimer);

            attitude.update();

            Attitude_t att = attitude.getAttitude();

            _mRoll  = _mRollPID.calculate(DEG2RAD(rc->roll.value), att.euler.roll, att.gyro.x, ATT_LOOP_SEC, PID_PARRA);
            _mPitch = _mPitchPID.calculate(DEG2RAD(rc->pitch.value), att.euler.pitch, att.gyro.y, ATT_LOOP_SEC, PID_PARRA);
            _mYaw   = _mYawPID.calculate(DEG2RAD(rc->yaw.value), att.euler.yaw, att.gyro.z, ATT_LOOP_SEC, PID_YAW_MANUAL);

            if (sv.isArmed()) {
                float thrust;

                /* If alt Hold thrust is softward set */
                if (sv.altHoldEnabled()) {
                    thrust = sv.getAltHoldThrottle(&_mAltSonarPID, ATT_LOOP_SEC);
                } else {
                    thrust = rc->throttle.getInput();
                }

                if (thrust <= p.escIDLE) {
                    mix.write(p.escIDLE);
                } else {
                    mix.update(Math::constrain(thrust, (float)p.escIDLE, (float)p.calRcThrottle.max) , _mRoll, _mPitch, _mYaw);
                }
            } else {
                mix.write(p.calRcThrottle.min);
            }

			#ifdef HIGH_SPEED_ATT
            if (_mCounter % 4 == 0) {
			#else
            if (_mCounter % 2 == 0) { // 100Hz task
			#endif
                task100Hz();
            }

            _mCounter++;
            sv.endAttitudeLoop(_mTimer);
        }

        /**
         * Check engine flags status
         */
        bool Engine::is(uint32_t flags)
        {
            return ((status & flags) == flags);
        }

        /**
         * Exec by Main Thread
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

            if (sv.isArmed()) {
                setLed(LED_RED, true);
                if (sv.altHoldEnabled()) {
                    infinyBlinkLed(LED_WHITE, 100);
                } else {
                    setLed(LED_WHITE, false);
                }
            } else {
                if (!is(ENGINE_RCERROR)) {
                    setLed(LED_RED, false);
                }
                setLed(LED_WHITE, false);
            }

        }

        void Engine::task100Hz()
        {
        	/* Check rc signal */
        	rc->check();

            /* Calculate baro */
            if (is(ENGINE_STATUS_BARO_OK)) {
                baro->update();
                if (_mCounter % 4 == 0) {
                    baro->calculate();
                }
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
