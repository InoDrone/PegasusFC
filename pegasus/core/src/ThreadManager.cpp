/*
 * ThreadManager.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "core/include/MainTimer.h"
#include "core/include/ThreadManager.h"
#include "core/include/MainThread.h"
#include "core/include/IdleThread.h"

namespace pegasus {
    namespace core {

        ThreadManager::ThreadManager() :
            _mIsStarted(false),
            _mIsLocked(false),
            _mCurrentThread(&mainThread) {

        }

        void ThreadManager::start()
        {
            // start IdleThread
            pegasus::core::idleThread.start();

            _mIsStarted = true;

            //pegasus::hal::ArchCore::initThread();
            //pegasus::hal::Processor::setCONTROL(0x02);

            __set_CONTROL(0x02);
            yield();
        }

        void ThreadManager::stop()
        {
            //pegasus::core::mainTimer.stop();
            _mIsStarted = false;
        }

        void ThreadManager::yield()
        {
            if (_mIsLocked) return;

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
