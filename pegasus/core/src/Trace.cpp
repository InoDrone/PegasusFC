/*
 * Trace.cpp
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#include "core/include/Trace.h"

namespace pegasus
{
    namespace core
    {

        Trace::Trace() { }

        void Trace::debug(const char msg[]) {
            shell.print("[D]");
            shell.println(msg);
        }

        void Trace::log(const char msg[]) {
            shell.print("[L]");
            shell.println(msg);
        }

        void Trace::warn(const char msg[]) {
            shell.print("[W]");
            shell.println(msg);
        }

        void Trace::error(const char msg[]) {
            shell.print("[E]");
            shell.println(msg);
        }

        void Trace::print(const char msg[]) {
            shell.print(msg);
        }

        void Trace::print(uint32_t number) {
            unsigned char buf[8 * sizeof(uint32_t)];
            unsigned long i = 0;

            if (number == 0) {
                shell.write('0');
            }

            while (number > 0) {
                buf[i++] = number % 10;
                number /= 10;
            }

            for (; i > 0; i--) {
                shell.write((char)(buf[i - 1] < 10 ?
                             '0' + buf[i - 1] :
                             'A' + buf[i - 1] - 10));
            }
        }

        void Trace::println()
        {
            shell.print("\r\n");
        }

    } /* namespace core */
} /* namespace Pegasus */
