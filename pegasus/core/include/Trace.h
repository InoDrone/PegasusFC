/*
 * Trace.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef TRACE_H_
#define TRACE_H_


#include "core/include/Defs.h"
#include "core/include/ComManager.h"
#include "core/include/Print.h"


namespace pegasus
{
    namespace core
    {

        class Trace
        {
            public:
                Trace();

                void log(const char *fmt, ...);
                void debug(const char *fmt, ...);
                void warn(const char *fmt, ...);
                void error(const char *fmt, ...);
        };

        extern Trace trace;

    } /* namespace core */
} /* namespace Pegasus */

#endif /* TRACE_H_ */
