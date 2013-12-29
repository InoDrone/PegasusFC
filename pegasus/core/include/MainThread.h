/*
 * MainThread.h
 *
 *  Created on: 27 nov. 2013
 *      Author: alienx
 */

#ifndef MAINTHREAD_H_
#define MAINTHREAD_H_

#include "core/include/Thread.h"

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

            pegasus::hal::Gpio yellow(pegasus::gpio::Port::E, pegasus::gpio::Pin::PIN0);
            yellow.mode(pegasus::gpio::Mode::OUTPUT);
            pegasus::hal::Gpio white(pegasus::gpio::Port::E, pegasus::gpio::Pin::PIN2);
            white.mode(pegasus::gpio::Mode::OUTPUT);
            pegasus::hal::Gpio red(pegasus::gpio::Port::E, pegasus::gpio::Pin::PIN1);
            red.mode(pegasus::gpio::Mode::OUTPUT);

            while(1) {
                yellow.high();
                sleep(100);
                yellow.low();
                white.high();
                sleep(100);
                white.low();
                red.high();
                sleep(100);
                red.low();
            };
        }


        extern MainThread mainThread;

    } /* namespace core */
} /* namespace pegasus */

#endif /* MAINTHREAD_H_ */
