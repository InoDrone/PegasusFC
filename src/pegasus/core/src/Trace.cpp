/*
 * Trace.cpp
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#include "pegasus/core/include/Trace.h"

namespace pegasus
{
    namespace core
    {

        Trace::Trace() { }

        void Trace::debug(const char msg[]) {
            //while (*msg) ITM_SendChar(*msg++);
        }

        void Trace::log(const char msg[]) {
            //while (*msg) ITM_SendChar(*msg++);
        }

        void Trace::warn(const char msg[]) {
            //while (*msg) ITM_SendChar(*msg++);
        }

        void Trace::error(const char msg[]) {
            //while (*msg) ITM_SendChar(*msg++);
        }

    } /* namespace core */
} /* namespace Pegasus */
