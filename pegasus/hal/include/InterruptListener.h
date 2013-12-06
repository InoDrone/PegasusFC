/*
 * MemberFunction.h
 *
 *  Created on: 30 nov. 2013
 *      Author: alienx
 */

#ifndef MEMBERFUNCTION_H_
#define MEMBERFUNCTION_H_

#include "core/include/Defs.h"

namespace pegasus {
    namespace hal {

        class InterruptListener {
            public:

                virtual void interrupt() = 0;
                static void trampoline(InterruptListener* listener) {
                    listener->interrupt();
                }
        };
    }
}

#endif /* MEMBERFUNCTION_H_ */
