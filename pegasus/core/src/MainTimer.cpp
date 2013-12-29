/*
 * MainTimer.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "core/include/MainTimer.h"
#include "core/include/ThreadManager.h"

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

        void MainTimer::delay(uint32_t ms)
        {
            uint32_t start = _mTicks;

            while( (_mTicks - start) < ms);
        }

        /**
         * MainTimer Interrupt (every 1ms)
         */
        void MainTimer::interruptServiceHandler()
        {
            _mTicks++;

            if (pegasus::core::threadManager.isStarted()) {
                pegasus::hal::ArchCore::yield();
            }
        }
    }
}
