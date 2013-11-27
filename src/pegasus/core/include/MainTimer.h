/*
 * MainTimer.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef MAINTIMER_H_
#define MAINTIMER_H_

#include "pegasus/core/include/Pegasus.h"

namespace pegasus {
    namespace core {

        class MainTimer
        {
            public:
                MainTimer();
                ~MainTimer();

                void init();
                void start();
                void stop();

                void interruptServiceHandler();
            private:
                pegasus::hal::CoreTimer _mTimer;
                volatile uint32_t _mTicks;
        };

        extern MainTimer mainTimer;
    }
}



#endif /* MAINTIMER_H_ */
