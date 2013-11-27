/*
 * MainTimer.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "pegasus/core/include/MainTimer.h"

namespace pegasus {
    namespace core {

        MainTimer::MainTimer() {}
        MainTimer::~MainTimer() {}

        void MainTimer::init()
        {
            _mTimer.init();
        }

        void MainTimer::start()
        {
            _mTicks = 0;
            _mTimer.start();
        }

        void MainTimer::stop()
        {
            _mTimer.stop();
        }

        /**
         * MainTimer Interrupt (every 1ms)
         */
        void MainTimer::interruptServiceHandler()
        {
            _mTicks++;
        }
    }
}
