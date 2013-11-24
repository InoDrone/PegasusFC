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
#include "Config.h"

#define LED_YELLOW 	Port2Pin('C', 3)
#define LED_BLUE 	Port2Pin('C', 2)
#define LED_RED 	Port2Pin('F', 3)

#define LED_1	Port2Pin('G', 13)
#define LED_2   Port2Pin('G', 14)


pegasus::Gpio led(pegasus::gpio::Port::G, pegasus::gpio::Pin::PIN13);

void setup()
{

  //initPlatform();

    led.mode(pegasus::gpio::Mode::OUTPUT);
    led.high();


  //os::rcout.add(Port2Pin('D', 12), 50);
  //Pegasus::core::mix.add(Port2Pin('D', 12),0,50);


	//sys.timer->attach(loop1Khz, 1000); // 1Khz 0.001ms
	//sys.timer->attach(loop100hz, 10000); // 100hz 0.010ms
	//sys.timer->attach(loop10hz, 100000); // 10hz 0.100ms
}

void loop() {

        led.high();
        pegasus::coreTimer.delayMillis(100);
        led.low();
        pegasus::coreTimer.delayMillis(100);


        /*for (int i=900;i<2500;i+=50) {
            os::rcout.write(Port2Pin('D', 12), i);
            os::timer.delayMillis(100);
        }*/

}


int main(void)
{
	//os::core.init();

	setup();

	while(1) {
		loop();
	}

	return 0;
}
