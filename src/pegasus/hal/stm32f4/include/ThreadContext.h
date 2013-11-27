/*
 * ThreadContext.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef THREADCONTEXT_H_
#define THREADCONTEXT_H_

#include "pegasus/hal/stm32f4/include/Defs.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            class ThreadContext
            {
                public:
                    ThreadContext();
                    ~ThreadContext();

                    void init(pegasus::hal::Stack_t* pStackStart, pegasus::core::ThreadEntry_t entryPoint,
                              void* func, void* thread);

                    uint32_t** getPStack();

                    static void callSwitch();
                    static uint32_t** _sStack;

                private:
                    uint32_t* _mStack;
            };

            INLINE uint32_t** ThreadContext::getPStack()
            {
                return &_mStack;
            }
        }

        using ThreadContext = pegasus::hal::stm32f4::ThreadContext;
    }
}



#endif /* THREADCONTEXT_H_ */
