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

#ifndef CRITICALSECTION_H_
#define CRITICALSECTION_H_

#include "core/include/Defs.h"
#include "core/include/ThreadManager.h"

namespace pegasus
{
    namespace core
    {

        class CriticalSection
        {
            public:
                CriticalSection();
                ~CriticalSection();
        };

        INLINE CriticalSection::CriticalSection() {
            threadManager.lock();
        }

        INLINE CriticalSection::~CriticalSection() {
            threadManager.unlock();
        }

    } /* namespace core */
} /* namespace pegasus */

#endif /* CRITICALSECTION_H_ */
