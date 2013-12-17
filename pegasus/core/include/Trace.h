/*
 * Trace.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef TRACE_H_
#define TRACE_H_

#include "core/include/ComManager.h"

namespace pegasus
{
    namespace core
    {

        class Trace
        {
            public:
                Trace();

                void log(const char message[]);
                void debug(const char message[]);
                void warn(const char message[]);
                void error(const char message[]);

                void print(uint32_t number);
                void print(const char message[]);
                void println();
        };

        extern Trace trace;

    } /* namespace core */
} /* namespace Pegasus */

#endif /* TRACE_H_ */
