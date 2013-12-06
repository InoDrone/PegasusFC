/*
 * Thread.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "core/include/Pegasus.h"
#include "core/include/ThreadManager.h"

namespace pegasus {
    namespace core {

        namespace thread {

            enum class State : uint32_t {
               READY,
               RUNNING,
               WAIT
            };
        }

        class Thread
        {
            public:
                Thread(const char* name, Thread_t entry) :
                    _mName(name),
                    _mState(thread::State::READY)
                {
                    _mEntry = reinterpret_cast<Thread_t>(*entry);
                    _mStack.top = (uint32_t*)_mStack.stack  + (STACK_SIZE / sizeof(uint32_t));
                    init();
                }

                Thread(const char* name) :
                    _mName(name),
                    _mState(thread::State::READY)
                {
                    _mEntry = nullptr;
                    _mStack.top = (uint32_t*)_mStack.stack  + (STACK_SIZE / sizeof(uint32_t));
                    init();
                }

                template<class Lambda_t>
                Thread(const char* name, Lambda_t func);

                ~Thread();

                void init();
                void start();
                void sleep(uint32_t ms);

                uint32_t* getTopStack();
                pegasus::hal::ThreadContext& getContext();

                thread::State getState() const;
                static void runEntryPoint(Thread_t, Thread* pThread);

            private:

                friend ThreadManager;

                pegasus::hal::Stack_t         _mStack; //[STACK_SIZE / sizeof(pegasus::hal::Stack_t)];
                const char*             _mName;
                Thread_t                _mEntry;
                thread::State           _mState;

                pegasus::hal::ThreadContext _mContext;
        };

        template<class Entry_t>
        Thread::Thread(const char* name, Entry_t entry) :
            _mName(name),
            _mState(thread::State::READY) {
            _mEntry = reinterpret_cast<Thread_t>(*entry);
            _mStack.top = (uint32_t*)_mStack.stack  + (STACK_SIZE / sizeof(uint32_t));

            init();

        }

        INLINE pegasus::hal::ThreadContext& Thread::getContext()
        {
            return _mContext;
        }

        INLINE thread::State Thread::getState() const {
            return _mState;
        }

    } /* namespace core */
} /* namespace Pegasus */

#endif /* THREAD_H_ */
