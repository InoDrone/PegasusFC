/*
 * FlightController.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "FlightController.h"

using namespace pegasus::core;

FlightController::FlightController():
    Thread("FC", [](FlightController* p) { p->run();}) {}

FlightController::~FlightController() {}

/**
 * Thread entry
 */
void FlightController::run()
{
    pegasus::hal::Gpio led(pegasus::gpio::Port::G, pegasus::gpio::Pin::PIN13);
    led.mode(pegasus::gpio::Mode::OUTPUT);

    while(1) {
        led.high();
        sleep(100);
        led.low();
        sleep(100);
    }
}

