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

                void lock();
                void unlock();

            private:
                Pipe<Thread, MAX_THREAD> _mThreads;
                bool volatile _mIsStarted;
                bool volatile _mIsLocked;
                Thread* volatile _mCurrentThread;


        };

        INLINE Thread* ThreadManager::getCurrent() {
            return _mCurrentThread;
        }

        INLINE bool ThreadManager::isStarted()
        {
            return _mIsStarted;
        }

        INLINE void ThreadManager::lock()
        {
            _mIsLocked = true;
        }

        INLINE void ThreadManager::unlock()
        {
            _mIsLocked = false;
        }

        extern ThreadManager threadManager;
    }
}


#endif /* THREADMANAGER_H_ */
