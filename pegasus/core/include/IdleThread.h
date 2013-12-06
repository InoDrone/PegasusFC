/*
 * IdleThread.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef IDLETHREAD_H_
#define IDLETHREAD_H_

#include "core/include/Thread.h"

namespace pegasus {
    namespace core {

        class IdleThread : public Thread
        {
            public:
                IdleThread();
                ~IdleThread();

            protected:
                void run();
        };

        extern IdleThread idleThread;
    }
}



#endif /* IDLETHREAD_H_ */
