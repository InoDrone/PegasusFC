/*
 * Trace.cpp
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#include "Trace.h"

namespace Pegasus
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

        Trace trace;

    } /* namespace core */
} /* namespace Pegasus */
