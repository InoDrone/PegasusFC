/*
 * Pid.h
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#ifndef PID_H_
#define PID_H_

#define PID_I_GUARD 1000.0f

#include "fc/include/Config.h"

namespace pegasus {
    namespace fc {

        class Pid {
            public:
                Pid();
                Pid(float kP, float kI, float kD, float kPStab, float kIStab);
                ~Pid() {};

                void resetIntegral();

                float calculate(float target, float current, float G_Dt);
                float calculate(float target, float acc, float gyro, float G_Dt);

                void setKp(float kP);
                void setKi(float kI);
                void setKd(float kD);

                void setKpStab(float kP);
                void setKiStab(float kI);

                float getKp();
                float getKi();
                float getKd();

                float getKpStab();
                float getKiStab();

                void setConfig( PIDConfig_t lvl, PIDConfig_t rate);

            private:
                    float kP;
                    float kI;
                    float kD;

                    float kPStab;
                    float kIStab;

                    float integral;
                    float lastError;
        };

    }
}


#endif /* PID_H_ */
