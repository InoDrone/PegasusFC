/*
 * Pid.cpp
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */


#include "fc/include/Pid.h"
#include <string.h>

namespace pegasus {
    namespace fc {

        Pid::Pid()
        {
            memset(&level, 0, sizeof(PIDConfig));
            memset(&rate, 0, sizeof(PIDConfig));
        }

        void Pid::resetIntegral()
        {
                level.integral = 0;
                rate.integral = 0;
        }


        float Pid::calculate(float target, float current, float G_Dt, PIDConfig* pid) {

                float error = target - current;

                /* Calculate PTERM */
                pid->pTerm = pid->kP * error;
                if (pid->pTerm > pid->PMax) {
                    pid->pTerm = pid->PMax;
                } else if (pid->pTerm < -pid->PMax) {
                    pid->pTerm = -pid->PMax;
                }

                /* Calculate I term */
                pid->integral += (error * G_Dt);
                pid->iTerm = pid->kI * pid->integral;
                if (pid->iTerm > pid->IMax) {
                    pid->iTerm = pid->IMax;
                    pid->integral = pid->iTerm / pid->kI;
                } else if (pid->iTerm < -pid->IMax){
                    pid->iTerm = -pid->IMax;
                    pid->integral = pid->iTerm / pid->kI;
                }

                /* Calculate D Term */
                if (pid->kD) {
                    pid->dTerm = pid->kD * (error - pid->lastError);
                    pid->lastError = error;
                    if (pid->dTerm > pid->DMax) {
                        pid->dTerm = pid->DMax;
                    } else if (pid->dTerm < -pid->DMax) {
                        pid->dTerm = -pid->DMax;
                    }
                } else {
                    pid->dTerm = 0.0f;
                }

                pid->out = pid->pTerm + pid->iTerm + pid->dTerm;

                if (pid->out > pid->OMax) {
                    pid->out = pid->OMax;
                } else if (pid->out < -pid->OMax) {
                    pid->out = -pid->OMax;
                }

                return pid->out;
        }

        float Pid::calculate(float target, float angle, float gyro, float G_Dt, uint8_t type) {
            float out = 0.0f;

            if (type == PID_PARRA) {
                out = calculate(target, angle, G_Dt, &level);
                out += calculate(0.0f, gyro, G_Dt, &rate);
            } else if (type == PID_CASCADING) {
                out = calculate (calculate(target, angle, G_Dt, &level), gyro, G_Dt, &rate);
            } else if (type == PID_YAW_MANUAL ) {
                out = calculate (target, gyro, G_Dt, &rate);
            }

            return out;
        }

        void Pid::setConfig( uavlink_pid lvl, uavlink_pid rte)
        {
            memcpy(&level, &lvl, sizeof(uavlink_pid));
            memcpy(&rate, &rte, sizeof(uavlink_pid));
        }

        void Pid::setConfig(uavlink_pid rte)
        {
            memcpy(&rate, &rte, sizeof(uavlink_pid));
        }

        float Pid::calculate(float target, float current, float G_Dt)
        {
            return calculate(target, current, G_Dt, &rate);
        }
    }
}
