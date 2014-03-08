/*
 * FlightController.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "FlightController.h"
#include "fc/include/Engine.h"
#include "fc/include/Mixing.h"

using namespace pegasus::core;
using namespace pegasus::gpio;
using namespace pegasus::fc;

pegasus::Func FlightController::_pFunc;


FlightController::FlightController():
    Thread("FC", [](FlightController* p) { p->run();}) {}

FlightController::~FlightController() {}

/**
 * Thread entry
 */
void FlightController::run()
{
    uint32_t counter = 0;
    uint32_t stickCounter = 0;
    _pFunc = &FlightController::waitingMode;


    while(1) {

        if (engine.rc->throttle.getInput() < (VAL_PWM_MIN+150)
            && engine.rc->roll.getInput() < (VAL_PWM_MIN+150)
            && engine.rc->pitch.getInput() < (VAL_PWM_MIN+150)
            && engine.rc->yaw.getInput() > (VAL_PWM_MAX-150)) {

                if (!engine.is(ENGINE_ARMED) && stickCounter >= 60 /* 3 sec */) {
                    engine.set(ENGINE_ARMED);
                    stickCounter = 0;
                }
                else if (engine.is(ENGINE_ARMED) && stickCounter >= 15) {
                    engine.rm(ENGINE_ARMED);
                    stickCounter = 0;
                }

                stickCounter++;
        }
        /* if (rc.throttle == MIN_THROTTLE) {
            if (rc.pitch == MIN_PITCH_CMD && rc.yaw == MIN_YAW_CMD && counter >= 60) { // for 3 second
                counter = 0;
                if (!engine.is(ENGINE_ARMED)) {
                    engine.status |= ENGINE_ARMED;
                    _pFunc = &FlightController::flightMode;
                }
            } else if (rc.pitch

           }*/

        if (engine.is(ENGINE_ARMED)) {
            _pFunc = &FlightController::flightMode;
        } else if (engine.is( ENGINE_ESCCALIBRATION )) {
            //_pFunc = &FlightController::calibrateESC;
            engine.rm(ENGINE_ESCCALIBRATION);
            _pFunc = &FlightController::waitingMode;
        } else {
            _pFunc = &FlightController::waitingMode;
        }


        // Execute pointer function
        (*_pFunc)();

        // Sleep 50 ms

        counter++;
        sleep(50);
    }
}

void FlightController::calibrateESC()
{
    // Check commande for entering in correct calibration mode

    mix.write(0, true);          //   Down PWM signal to very lower value (500)
    mainTimer.delay(1000);       //   wait 1s
    mix.write(MAX_ESC);          //   UP PWM signal to max engine value (MAX_ESC) (entering ESC calibration)
    mainTimer.delay(2000);       //   wait 2s
    mix.write(IDLE_ESC);         //   Down PWM signal to min engine value (IDLE_ESC)
    mainTimer.delay(2000);       //   wait 2s
    mix.write(0, true);          //   Down PWM signal to very lower value
    mainTimer.delay(1000);       //   wait 2s
    mix.write(MIN_ESC);          //   UP PWM signal to min engine value (MIN_ESC)

    engine.rm(ENGINE_ESCCALIBRATION);
    _pFunc = &FlightController::waitingMode;
}

void FlightController::waitingMode()
{

}

void FlightController::flightMode()
{
    if (engine.is( (ENGINE_INFLIGHT | ENGINE_RCERROR) )) {
        //auto takeof
    }
    // start motor
    // if auto landing -> go to 1 meter and stabilize
}

FlightController fc;

