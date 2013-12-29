/*
 * FlightController.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "FlightController.h"

using namespace pegasus::core;
using namespace pegasus::gpio;

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
    _pFunc = &FlightController::waitingMode;


    while(1) {

        /* if (rc.throttle == MIN_THROTTLE) {
            if (rc.pitch == MIN_PITCH_CMD && rc.yaw == MIN_YAW_CMD && counter >= 60) { // for 3 second
                counter = 0;
                _pFunc = &FlightController::flightMode;
            } else if (rc.pitch

           }*/


        // Execute pointer function
        (*_pFunc)();

        // Sleep 50 ms

        counter++;
        sleep(50);
    }
}

void FlightController::calibrationMode()
{
    // Check commande for entering in correct calibration mode

    // - ESC
    //   Down PWM signal to very lower value (500)
    //   wait 1s
    //   UP PWM signal to max engine value (2500) (entering ESC calibration)
    //   wait 1s
    //   Down PWM signal to min engine value (1000)
    //   wait 1s
    //   Down PWM signal to very lower value
    //   wait 1s
    //   UP PWM signal to min engine value (1000)
    //   Finish

    // - Radio

    // - Sensors
}

void FlightController::waitingMode()
{

}

void FlightController::flightMode()
{
    // start motor
    // if auto landing -> go to 1 meter and stabilize
}

