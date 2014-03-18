/*
 * FlightController.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "FlightController.h"
#include "fc/include/Engine.h"
#include "fc/include/Mixing.h"
#include "fc/include/Supervisor.h"

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

        if (engine.rc->throttle.getInput() < (VAL_PWM_MIN+200)
            && engine.rc->roll.getInput() < (VAL_PWM_MIN+200)
            && engine.rc->pitch.getInput() < (VAL_PWM_MIN+200)
            && engine.rc->yaw.getInput() > (VAL_PWM_MAX-200)) {

                if (!sv.isArmed() && stickCounter >= 60 /* 3 sec */) {
                    sv.setArmed(true);
                    stickCounter = 0;
                }
                else if (sv.isArmed() && stickCounter >= 15) {
                    sv.setArmed(false);
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

        if (sv.isArmed()) {
            _pFunc = &FlightController::flightMode;
        } else {
            engine.rm(ENGINE_ALT_HOLD);
            _pFunc = &FlightController::waitingMode;
        }


        // Execute pointer function
        (*_pFunc)();

        // Sleep 50 ms

        counter++;
        sleep(50);
    }
}


void FlightController::waitingMode()
{

}

void FlightController::flightMode()
{
    static bool lastAux1Status = engine.rc->aux1.isOn;

    if (lastAux1Status != engine.rc->aux1.isOn) {
        if (engine.rc->aux1.isOn) {
            sv.enableAltHold();
        } else {
            sv.disableAltHold();
        }

        lastAux1Status = engine.rc->aux1.isOn;
    }
}

FlightController fc;

