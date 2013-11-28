/*
 * MainThread.h
 *
 *  Created on: 27 nov. 2013
 *      Author: alienx
 */

#ifndef MAINTHREAD_H_
#define MAINTHREAD_H_

#include "pegasus/core/include/Thread.h"

namespace pegasus
{
    namespace core
    {

        class MainThread : public Thread
        {
            public:
                MainThread();
                ~MainThread();

                void run();
        };

        inline
        MainThread::MainThread() :
            Thread("IDLE", [](MainThread* p) { p->run();}) {}

        inline
        MainThread::~MainThread() {}


        inline void MainThread::run() {

            pegasus::hal::Timer timer4(TIM4);
            pegasus::hal::TimerChannel T4C0(&timer4, 0);
            pegasus::hal::Gpio D12(pegasus::gpio::Port::D, pegasus::gpio::Pin::PIN12);
            pegasus::hal::RCOutput servo1(&D12, &T4C0, 50);

            pegasus::hal::Gpio led(pegasus::gpio::Port::G, pegasus::gpio::Pin::PIN14);
            led.mode(pegasus::gpio::Mode::OUTPUT);

            while(1) {
                for (int i=900;i<2500;i+=50) {
                    servo1.write(i);
                    led.high();
                    sleep(50);
                    led.low();
                    sleep(50);
                }
            };
        }


        extern MainThread mainThread;

    } /* namespace core */
} /* namespace pegasus */

#endif /* MAINTHREAD_H_ */
