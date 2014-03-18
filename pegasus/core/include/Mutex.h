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

#ifndef MUTEX_H_
#define MUTEX_H_

#include <inttypes.h>

namespace pegasus
{
    namespace core
    {

        typedef uint8_t MutexID;

        class Mutex
        {
            public:
                static MutexID create();
                static void enter(MutexID mutexId);
                static void leave(MutexID mutexId);

            private:
                static bool mutexLocked[64];
                static MutexID mutexSize;
        };

    } /* namespace core */
} /* namespace pegasus */

#endif /* MUTEX_H_ */
