/*
 * Trace.cpp
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#include "core/include/Trace.h"

namespace pegasus {
    namespace core {

        using namespace std;

        Trace::Trace() { }

        void Trace::debug(const char *fmt, ...) {
            va_list ap;

            va_start(ap, fmt);
            (void)vprintf(fmt, ap);
            va_end(ap);

            printf("\r\n");
        }

        void Trace::log(const char *fmt, ...) {
            va_list ap;

            va_start(ap, fmt);
            (void)vprintf(fmt, ap);
            va_end(ap);

            printf("\r\n");
        }

        void Trace::warn(const char *fmt, ...) {
            va_list ap;
            printf("[WARN] ");

            va_start(ap, fmt);
            (void)vprintf(fmt, ap);
            va_end(ap);

            printf("\r\n");
        }

        void Trace::error(const char *fmt, ...) {
            va_list ap;
            printf("[ERROR] ");

            va_start(ap, fmt);
            (void)vprintf(fmt, ap);
            va_end(ap);

            printf("\r\n");
        }

    } /* namespace core */
} /* namespace Pegasus */
