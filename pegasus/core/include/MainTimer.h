/*
 * MainTimer.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef MAINTIMER_H_
#define MAINTIMER_H_

#include "core/include/Pegasus.h"

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
                uint32_t ticks();
                uint32_t millis();

            private:
                pegasus::hal::CoreTimer _mTimer;
                volatile uint32_t _mTicks;
        };

        INLINE uint32_t MainTimer::ticks()
        {
            return _mTicks;
        }

        INLINE uint32_t MainTimer::millis()
        {
            return _mTicks;
        }

        extern MainTimer mainTimer;
    }
}



#endif /* MAINTIMER_H_ */
