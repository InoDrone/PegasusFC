/*
 * Pegasus.cpp
 *
 *  Created on: 24 nov. 2013
 *      Author: alienx
 */

#include "pegasus/core/include/Pegasus.h"

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
            ArchCore::init();

        }

        PegasusCore::~PegasusCore()
        {
            ArchCore::reset();
        }

        pegasus::core::Mixing mix;
        pegasus::core::Trace trace;
    } /* namespace core */

} /* namespace Pegasus */

static pegasus::core::PegasusCore pegasusCore;
