/*
 * ThreadManager.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "pegasus/core/include/MainTimer.h"
#include "pegasus/core/include/ThreadManager.h"
#include "pegasus/core/include/IdleThread.h"

namespace pegasus {
    namespace core {

        ThreadManager::ThreadManager() :
            _mIsStarted(false),
            _mCurrentThread(&idleThread) {

        }

        void ThreadManager::start()
        {
            // start IdleThread
            pegasus::core::idleThread.start();
            //pegasus::core::idleThread.start();

            // start hardware timer
            pegasus::core::mainTimer.init();
            pegasus::core::mainTimer.start();

            _mIsStarted = true;

            //pegasus::hal::ArchCore::initThread();
            //pegasus::hal::Processor::setCONTROL(0x02);


            yield();
        }

        void ThreadManager::stop()
        {
            pegasus::core::mainTimer.stop();
            _mIsStarted = false;
        }

        void ThreadManager::yield()
        {
            pegasus::hal::ArchCore::yield();
        }

        void ThreadManager::addThread(Thread* thread)
        {
            _mThreads.push(thread);
            // TODO int pendSV
        }

        void ThreadManager::contextSwitchInterrupt() {

            Thread* thread = (Thread*) _mCurrentThread;

            _mThreads.remove(thread);
            if (thread->getState() == thread::State::RUNNING) {
                _mThreads.push(thread);
            }

            _mCurrentThread = _mThreads.pop();
        }
    }
}
