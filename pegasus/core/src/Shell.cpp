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

#include "core/include/Shell.h"

namespace pegasus
{
    namespace core
    {

        Shell::Shell() :
                _mUart(0),
                _mConnected(false)
        {
        }

        void Shell::init(pegasus::hal::UARTDriverBase_t* uart)
        {
            _mUart = uart;
            _mUart->init();
            _mConnected = _mUart->open();
        }

        void Shell::print(const char msg[])
        {
            if (!_mConnected) return;
            _mUart->write(msg);
        }

        void Shell::println(const char msg[])
        {
            if (!_mConnected) return;
            _mUart->write(msg);
            _mUart->write("\r\n");
        }

        void Shell::write(char c)
        {
            if (!_mConnected) return;
            _mUart->write(c);
        }

    } /* namespace core */
} /* namespace pegasus */
