/*
 * ThreadManager.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_

#include "pegasus/core/include/Defs.h"
#include "pegasus/core/include/Pipe.h"

#define MAX_THREAD 3

namespace pegasus {
    namespace core {

        class Thread;

        class ThreadManager {
            public:
                ThreadManager();

                void start();
                void stop();

                void yield();

                void addThread(Thread* thread);

                void contextSwitchInterrupt();
                Thread* getCurrent();

            private:
                Pipe<Thread, MAX_THREAD> _mThreads;
                Thread* volatile _mCurrentThread;
                bool volatile _mIsStarted;


        };

        INLINE Thread* ThreadManager::getCurrent() {
            return _mCurrentThread;
        }

        extern ThreadManager threadManager;
    }
}


#endif /* THREADMANAGER_H_ */
