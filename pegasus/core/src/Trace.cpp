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
            com.print("[D]", ComManager::DEBUG);
            com.println(msg, ComManager::DEBUG);
        }

        void Trace::log(const char msg[]) {
            com.print("[L]", ComManager::DEBUG);
            com.println(msg, ComManager::DEBUG);
        }

        void Trace::warn(const char msg[]) {
            com.print("[W]", ComManager::DEBUG);
            com.println(msg, ComManager::DEBUG);
        }

        void Trace::error(const char msg[]) {
            com.print("[E]", ComManager::DEBUG);
            com.println(msg, ComManager::DEBUG);
        }

        void Trace::print(const char msg[]) {
            com.print(msg, ComManager::DEBUG);
        }

        void Trace::print(uint32_t number) {
            unsigned char buf[8 * sizeof(uint32_t)];
            unsigned long i = 0;

            if (number == 0) {
                com.write('0', ComManager::DEBUG);
            }

            while (number > 0) {
                buf[i++] = number % 10;
                number /= 10;
            }

            for (; i > 0; i--) {
                com.write((char)(buf[i - 1] < 10 ?
                             '0' + buf[i - 1] :
                             'A' + buf[i - 1] - 10), ComManager::DEBUG);
            }
        }

        void Trace::println()
        {
            com.print("\r\n", ComManager::DEBUG);
        }

    } /* namespace core */
} /* namespace Pegasus */
