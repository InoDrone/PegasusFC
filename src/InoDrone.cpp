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


pegasus::hal::Gpio led(pegasus::gpio::Port::G, pegasus::gpio::Pin::PIN13);
pegasus::hal::Timer timer4(TIM4);
pegasus::hal::TimerChannel T4C0(&timer4, 0);
pegasus::hal::Gpio D12(pegasus::gpio::Port::D, pegasus::gpio::Pin::PIN12);
pegasus::hal::RCOutput servo1(&D12, &T4C0, 50);

// FC Thread
//FlightController fc;
uint32_t PSP     = pegasus::hal::Processor::getPSP();
uint32_t MSP     = pegasus::hal::Processor::getMSP();
uint32_t CONTROL = pegasus::hal::Processor::getCONTROL();

void setup()
{

    led.mode(pegasus::gpio::Mode::OUTPUT);
    led.high();


    pegasus::core::threadManager.start();

  //os::rcout.add(Port2Pin('D', 12), 50);
  //Pegasus::core::mix.add(Port2Pin('D', 12),0,50);


	//sys.timer->attach(loop1Khz, 1000); // 1Khz 0.001ms
	//sys.timer->attach(loop100hz, 10000); // 100hz 0.010ms
	//sys.timer->attach(loop10hz, 100000); // 10hz 0.100ms
}

void loop() {

        led.high();



        PSP     = pegasus::hal::Processor::getPSP();
        MSP     = pegasus::hal::Processor::getMSP();
        CONTROL = pegasus::hal::Processor::getCONTROL();
        //pegasus::hal::coreTimer.delayMillis(100);
        //led.low();
       // pegasus::hal::coreTimer.delayMillis(100);


        /*for (int i=900;i<2500;i+=10) {
            servo1.write(i);
            pegasus::coreTimer.delayMillis(25);
        }*/



}


int main(void)
{

        initPlatform();


        setup();

	while(1) {
		loop();
	}

	return 0;
}
