/*
 * Pegasus.cpp
 *
 *  Created on: 24 nov. 2013
 *      Author: alienx
 */

#include "pegasus/core/include/Pegasus.h"
#include "pegasus/core/include/MainTimer.h"
#include "pegasus/core/include/ThreadManager.h"
#include "pegasus/core/include/MainThread.h"
#include "pegasus/core/include/IdleThread.h"
#include "pegasus/core/include/Mixing.h"
#include "pegasus/core/include/Trace.h"

namespace pegasus
{
    namespace core
    {

        class PegasusCore {
            public:

                PegasusCore();
                ~PegasusCore();
        };

        PegasusCore::PegasusCore()
        {
            pegasus::hal::ArchCore::init();

        }

        PegasusCore::~PegasusCore()
        {
            pegasus::hal::ArchCore::reset();
        }



    } /* namespace core */




} /* namespace Pegasus */

// First initialization
static pegasus::core::PegasusCore pegasusCore;

namespace pegasus {
    namespace core {
        pegasus::core::MainTimer mainTimer;
        pegasus::core::Mixing mix;
        pegasus::core::Trace trace;
        pegasus::core::ThreadManager threadManager;
        pegasus::core::MainThread mainThread;
        pegasus::core::IdleThread idleThread;
    }

    namespace hal {
        pegasus::hal::PortMapping portMapping;
    }
}
