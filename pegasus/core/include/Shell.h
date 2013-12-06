/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author: Marc Jacquier <marc@inodrone.com>
 *  Project: InoDronePegasus
 */

#ifndef SHELL_H_
#define SHELL_H_

#include "core/include/Pegasus.h"
#include "hal/include/UARTDriverBase.h"

namespace pegasus {
    namespace core {

        class Shell
        {
            public:
                Shell();
                void init(pegasus::hal::UARTDriverBase_t* uart);

                void print(const char msg[]);
                void println(const char msg[]);
                void write(char c);

                bool isConnected() {
                    return _mConnected;
                }

            private:
                pegasus::hal::UARTDriverBase_t* _mUart;
                bool _mConnected;
        };

        extern Shell shell;

    } /* namespace core */
} /* namespace pegasus */

#endif /* SHELL_H_ */
