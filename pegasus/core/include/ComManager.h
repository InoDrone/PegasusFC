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

#ifndef COMMANAGER_H_
#define COMMANAGER_H_

#include "core/include/Defs.h"
//#include "core/include/ComDeviceBase.h"

#define COMMANAGER_MAX_DEVICE 2

namespace pegasus {
    namespace core {

        class ComDeviceBase; // Foward declartion for pointer
        class ComManager
        {
            public:
                enum {
                    DEBUG,
                    UAVLINK
                };

                enum STEP {
                    STX1,
                    STX2,
                    CTX,
                    LTX,
                    DTX,
                    CRC8
                };

                struct Device {
                        ComDeviceBase* device;
                        uint8_t  buffer[255];
                        uint8_t* ptr;
                        uint8_t step;
                        uint8_t type;
                        uint8_t crc;
                        uint16_t len;
                };

                ComManager();

                void addDevice(ComDeviceBase* device);

                void write(char c, uint8_t type);
                void print(const char msg[], uint8_t type);
                void println(const char msg[], uint8_t type);

            private:
                friend ComDeviceBase;
                void receive(uint8_t byte, uint8_t id);

                Device  _mDevices[COMMANAGER_MAX_DEVICE];

                uint8_t countDevice;
        };

        extern ComManager com;

    } /* namespace core */
} /* namespace pegasus */

#endif /* COMMANAGER_H_ */
