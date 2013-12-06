/*
 * Pegasus.cpp
 *
 *  Created on: 24 nov. 2013
 *      Author: alienx
 */

#include "core/include/Pegasus.h"
#include "core/include/MainTimer.h"
#include "core/include/ThreadManager.h"
#include "core/include/MainThread.h"
#include "core/include/IdleThread.h"
#include "core/include/Trace.h"
#include "core/include/Shell.h"

#include "fc/include/Engine.h"
#include "fc/include/Mixing.h"

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
        pegasus::core::Trace trace;
        pegasus::core::ThreadManager threadManager;
        pegasus::core::MainThread mainThread;
        pegasus::core::IdleThread idleThread;
        pegasus::core::Shell shell;
    }

    namespace hal {
        pegasus::hal::PortMapping portMapping;
    }

    namespace fc {
        pegasus::fc::Mixing mix;
    }
}
