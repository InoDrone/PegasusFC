/*
 * MainThread.h
 *
 *  Created on: 27 nov. 2013
 *      Author: alienx
 */

#ifndef MAINTHREAD_H_
#define MAINTHREAD_H_

#include "core/include/Thread.h"
#include "fc/include/Engine.h"

namespace pegasus
{
    namespace core
    {

        using namespace pegasus::fc;

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

            while(1) {

                /* Exec engine low priority task */
                engine.lowPriorityTask();

                /*yellow.high();
                sleep(100);
                yellow.low();
                white.high();
                sleep(100);
                white.low();
                red.high();
                sleep(100);
                red.low();*/
            };
        }


        extern MainThread mainThread;

    } /* namespace core */
} /* namespace pegasus */

#endif /* MAINTHREAD_H_ */
