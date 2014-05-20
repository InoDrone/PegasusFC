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

#include "core/include/Mutex.h"
#include "core/include/ThreadManager.h"

namespace pegasus
{
    namespace core
    {
        uint8_t Mutex::mutexSize = 0;
        bool Mutex::mutexLocked[64];

        uint8_t Mutex::create()
        {
            mutexLocked[mutexSize] = false;
            return mutexSize;
        }

        void Mutex::enter(uint8_t mutexId)
        {
            //Thread *thread = threadManager.getCurrent();

            if (!threadManager.isStarted()) return;

            if (mutexLocked[mutexId]) {
                /* Waiting to mutex released */
                while (mutexLocked[mutexId]);

                /*return;*/
            }

            mutexLocked[mutexId] = true;
        }

        void Mutex::leave(uint8_t mutexId)
        {
            mutexLocked[mutexId] = false;
        }

    } /* namespace core */
} /* namespace pegasus */
