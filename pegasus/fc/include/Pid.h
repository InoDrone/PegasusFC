/*
 * Pid.h
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#ifndef PID_H_
#define PID_H_

#include "fc/include/Config.h"

#define PID_PARRA            0
#define PID_CASCADING        1
#define PID_YAW_MANUAL       2
#define PID_YAW_HEADING      3

namespace pegasus {
    namespace fc {

        class Pid {
            public:
                Pid();
                Pid(float kP, float kI, float kD, float kPStab, float kIStab);
                ~Pid() {};

                void resetIntegral();


                float calculate(float target, float acc, float gyro, float G_Dt, uint8_t type=PID_PARRA);

                void setConfig(uavlink_pid lvl, uavlink_pid rate);
                void setConfig(uavlink_pid rate);
                float calculate(float target, float current, float G_Dt);

            private:

                struct PIDConfig {
                    float kP;
                    float kI;
                    float kD;
                    float PMax;
                    float IMax;
                    float DMax;
                    float OMax;
                    float integral;
                    float lastError;

                    float pTerm;
                    float iTerm;
                    float dTerm;

                    float out;
                };

                PIDConfig level;
                PIDConfig rate;


                float calculate(float target, float current, float G_Dt, PIDConfig* pid);
        };

    }
}


#endif /* PID_H_ */
