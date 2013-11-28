/*
 * main.c
 *
 *  Created on: 25 oct. 2013
 *      Author: alienx
 */

#ifndef PEGASUS_STM32
#define PEGASUS_STM32
#endif

#include <pegasus/core/include/Pegasus.h>
#include <pegasus/core/include/ThreadManager.h>
#include "FlightController.h"

#include "Config.h"

#define LED_YELLOW 	Port2Pin('C', 3)
#define LED_BLUE 	Port2Pin('C', 2)
#define LED_RED 	Port2Pin('F', 3)

#define LED_1	Port2Pin('G', 13)
#define LED_2   Port2Pin('G', 14)


//pegasus::hal::Gpio led(pegasus::gpio::Port::G, pegasus::gpio::Pin::PIN13);
/*pegasus::hal::Timer timer4(TIM4);
pegasus::hal::TimerChannel T4C0(&timer4, 0);
pegasus::hal::Gpio D12(pegasus::gpio::Port::D, pegasus::gpio::Pin::PIN12);
pegasus::hal::RCOutput servo1(&D12, &T4C0, 50);*/

// FC Thread
FlightController fc;

int main(void)
{
        // Init platform specifique (motor, com, etc ..)
        initPlatform();

        // Start Flight Control Thread
        fc.start(); // start Fc Thread

        // Starte Main scheduler
        pegasus::core::threadManager.start();

	return 0;
}
