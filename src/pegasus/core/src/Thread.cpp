/*
 * Thread.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "pegasus/core/include/Thread.h"
#include "pegasus/core/include/ThreadManager.h"

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
        void Thread::runEntryPoint(Thread_t* entry, Thread* pThread)
        {
            // execute function
            (*entry)();
        }

        uint32_t* Thread::getTopStack()
        {
            return _mStack.top;
        }

        uint32_t* Thread::getTopStackAligned()
        {
            // go to the end of the unaligned area
            char* p = reinterpret_cast<char*>(_mStack.top);

            // compute how far we are from the alignment area
            // assume the input value is a power of 2
            uint32_t diff = ((uint32_t) p) & 7;

            // diminish pointer to aligned border
            return (uint32_t*) (p - diff);
        }

    } /* namespace core */
} /* namespace Pegasus */
