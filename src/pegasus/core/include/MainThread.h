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
        };

        inline
        MainThread::MainThread() :
            Thread("IDLE") {}

        inline
        MainThread::~MainThread() {}


        extern MainThread mainThread;

    } /* namespace core */
} /* namespace pegasus */

#endif /* MAINTHREAD_H_ */
