/*
 * Pid.cpp
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */


#include "fc/include/Pid.h"

namespace pegasus {
    namespace fc {

        Pid::Pid(float kP, float kI, float kD, float kPStab, float kIStab)
        {
                this->kP = kP;
                this->kI = kI;
                this->kD = kD;

                this->kPStab = kPStab;
                this->kIStab = kIStab;

                integral = 0;
                lastError = 0;

        }

        void Pid::resetIntegral()
        {
                integral = 0;
        }

        void Pid::setKp(float kP)
        {
                this->kP = kP;
        }

        float Pid::getKp()
        {
                return this->kP;
        }

        void Pid::setKi(float kI)
        {
                this->kI = kI;
        }

        float Pid::getKi()
        {
                return this->kI;
        }

        void Pid::setKd(float kD)
        {
                this->kD = kD;
        }

        float Pid::getKd()
        {
                return this->kD;
        }

        void Pid::setKpStab(float kP)
        {
            this->kPStab = kP;
        }

        float Pid::getKpStab()
        {
            return this->kPStab;
        }

        void Pid::setKiStab(float kI)
        {
            this->kIStab = kI;
        }

        float Pid::getKiStab()
        {
            return this->kIStab;
        }

        float Pid::calculate(float target, float current, float G_Dt) {

                float error = target - current;

                integral += (error*G_Dt);
                if (integral > PID_I_GUARD) integral = PID_I_GUARD;
                if (integral < -PID_I_GUARD) integral = -PID_I_GUARD;

                float errorDiff = error - lastError;
                lastError = error;

                return (error * kP) + (integral * kI) + (errorDiff * kD);
        }

        float Pid::calculate(float target, float angle, float gyro, float G_Dt) {

            float errorAngle = target - angle;

            integral += (errorAngle*G_Dt);
            if (integral > PID_I_GUARD) integral = PID_I_GUARD;
            if (integral < -PID_I_GUARD) integral = -PID_I_GUARD;


            float errorSum = gyro - lastError;
            lastError = gyro;


            return ((errorAngle * kPStab) - (gyro*kP)) + (integral * kIStab) - (errorSum * kD);
        }
    }
}
