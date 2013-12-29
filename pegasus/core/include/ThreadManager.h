/*
 * ThreadManager.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_

#include "core/include/Defs.h"
#include "core/include/Pipe.h"

#define MAX_THREAD 10

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
                bool isStarted();

                void contextSwitchInterrupt();
                Thread* getCurrent();

            private:
                Pipe<Thread, MAX_THREAD> _mThreads;
                bool volatile _mIsStarted;
                Thread* volatile _mCurrentThread;


        };

        INLINE Thread* ThreadManager::getCurrent() {
            return _mCurrentThread;
        }

        INLINE bool ThreadManager::isStarted()
        {
            return _mIsStarted;
        }

        extern ThreadManager threadManager;
    }
}


#endif /* THREADMANAGER_H_ */
