/*
 * FlightController.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "FlightController.h"

using namespace pegasus::core;
using namespace pegasus::gpio;

FlightController::FlightController():
    Thread("FC", [](FlightController* p) { p->run();}) {}

FlightController::~FlightController() {}

/**
 * Thread entry
 */
void FlightController::run()
{
    pegasus::hal::Gpio led(Port::G, Pin::PIN13);
    led.mode(Mode::OUTPUT);

    while(1) {
        led.high();
        sleep(100);
        led.low();
        sleep(100);
    }
}

