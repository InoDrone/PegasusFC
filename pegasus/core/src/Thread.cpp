/*
 * Thread.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "core/include/Thread.h"
#include "core/include/ThreadManager.h"
#include "core/include/MainTimer.h"

namespace pegasus
{
    namespace core
    {

        Thread::~Thread()
        {

        }

        void Thread::init()
        {
            uint32_t* p = _mStack.stack;
            if (p != nullptr) {
                uint32_t size = STACK_SIZE / sizeof(uint32_t);
                for(; size > 0;--size)
                    *p++ = 0x99AAAA99;
            }
        }

        void Thread::start()
        {

            _mContext.init(&_mStack, (pegasus::core::ThreadEntry_t) runEntryPoint, (void*) _mEntry, (void*) this);

            threadManager.addThread(this);
            _mState = thread::State::RUNNING;
        }


        /**
         * Static method called by Context Switch
         */
        void Thread::runEntryPoint(Thread_t entry, Thread* pThread)
        {
            // execute function
            (*entry)();


          // pass control to the next thread
          pegasus::hal::ArchCore::yield();
        }

        uint32_t* Thread::getTopStack()
        {
            return _mStack.top;
        }

        void Thread::sleep(uint32_t ms) {
            uint32_t start = pegasus::core::mainTimer.ticks();

            while( (pegasus::core::mainTimer.ticks() - start) < ms);
        }

    } /* namespace core */
} /* namespace Pegasus */
