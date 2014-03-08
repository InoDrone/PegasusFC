/*
 * main.c
 *
 *  Created on: 25 oct. 2013
 *      Author: alienx
 */

#ifndef PEGASUS_STM32
#define PEGASUS_STM32
#endif

#include "Config.h"

#include <core/include/Pegasus.h>
#include <core/include/ThreadManager.h>
#include <fc/include/Engine.h>
#include "FlightController.h"
#include "Telemetry.h"


int main(void)
{
    initPlatform();

    fc.start(); // start Fc Thread
    telemetry.start(); // Start Telemetry Thread

    // Starte Main scheduler
    pegasus::core::threadManager.start();

    return 0;
}
