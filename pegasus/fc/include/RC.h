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

#ifndef RC_H_
#define RC_H_

#include "fc/include/RCChannel.h"

namespace pegasus {
    namespace fc {

        class RC
        {
            public:
                RC();

                void check();

                bool hasError() {
                    return _mSignalError;
                }

                RCChannel roll;
                RCChannel pitch;
                RCChannel yaw;
                RCChannel throttle;
                RCChannel aux1;
                RCChannel aux2;

            private:
                uint8_t _mValidChannel;
                bool _mSignalError;
        };

        extern RC rc;

    } /* namespace fc */
} /* namespace pegasus */

#endif /* RC_H_ */
