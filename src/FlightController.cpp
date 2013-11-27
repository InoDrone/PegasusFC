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
    while(1) {}
}

