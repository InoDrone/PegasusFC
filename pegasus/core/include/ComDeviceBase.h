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

#ifndef COMDEVICEBASE_H_
#define COMDEVICEBASE_H_

#include "core/include/Defs.h"
#include "core/include/ComManager.h"
#include "hal/include/ByteListener.h"
#include "core/include/Print.h"

namespace pegasus {
    namespace core {

        class ComDeviceBase : public pegasus::hal::ByteListener, public pegasus::core::io::Print {
            public:
                ComDeviceBase() :
                    _mId(-1),
                    _mConnected(false)
                     {}

                virtual void init() = 0;
                virtual bool open() = 0;
                virtual bool close() = 0;

                using Print::write;

                bool isConnected() {
                    return _mConnected;
                }

                void setId(int8_t id) {
                    _mId = id;
                }

                int8_t getId() {
                    return _mId;
                }

                void addReceiver(ComManager* com, int8_t deviceId) {
                    _mComManager = com;
                    setId(deviceId);
                }

                void receive(uint8_t byte) {
                    if (_mComManager && _mId != -1) {
                        _mComManager->receive(byte, _mId);
                    }
                }

            private:
                int8_t _mId;
                ComManager* _mComManager;

            protected:
                bool _mConnected;
        };
    }
}


#endif /* COMDEVICEBASE_H_ */
