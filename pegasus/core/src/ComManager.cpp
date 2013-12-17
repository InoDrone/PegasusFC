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

#include "core/include/ComManager.h"
#include "core/include/ComDeviceBase.h"

namespace pegasus
{
    namespace core
    {

        ComManager::ComManager() :
                countDevice(0)
        {}

        void ComManager::addDevice(ComDeviceBase* device) {
            if (countDevice >= COMMANAGER_MAX_DEVICE) {
                return;
            }

            device->init();
            device->addReceiver(this, countDevice);
            _mDevices[countDevice].device = device;
            _mDevices[countDevice].type = ComManager::DEBUG;
            _mDevices[countDevice].ptr = &_mDevices[countDevice].buffer[0];
            _mDevices[countDevice].step = ComManager::STX1;
            _mDevices[countDevice].crc = 0;
            _mDevices[countDevice].len = 0;
            device->open();

            countDevice++;
        }

        void ComManager::write(char c, uint8_t type) {
            uint8_t i;
            for (i=0;i<countDevice;i++) {
                if (_mDevices[i].device->isConnected() && _mDevices[i].type == type) {
                    _mDevices[i].device->write(c);
                }
            }
        }

        void ComManager::print(const char msg[], uint8_t type)
        {
            uint8_t i;
            for (i=0;i<countDevice;i++) {
                if (_mDevices[i].device->isConnected() && _mDevices[i].type == type) {
                    _mDevices[i].device->write(msg);
                }
            }
        }

        void ComManager::println(const char msg[], uint8_t type)
        {
            uint8_t i;
            for (i=0;i<countDevice;i++) {
                if (_mDevices[i].device->isConnected() && _mDevices[i].type == type) {
                    _mDevices[i].device->write(msg);
                    _mDevices[i].device->write("\r\n");
                }
            }
        }

        void ComManager::receive(uint8_t byte, uint8_t id) {

            Device* d = &_mDevices[id];
            switch(d->step) {
                case STX1:
                    if (byte == 0xFF) {
                        d->step = STX2;
                    }
                    break;
                case STX2:
                    if (byte == 0xFF) {
                        d->ptr = &d->buffer[0];
                        d->step = CTX;
                    } else {
                        d->step = STX1;
                    }
                    break;
                case CTX:
                    if (byte == 0xCC) { // KikooLool It's Me
                        d->step = STX1;
                        d->device->write("ACK\r\n");
                    } else if (byte == 0xDD) { // Switch To debug Mode
                        d->step = STX1;
                        d->device->write("DBG\r\n");
                        d->type = ComManager::DEBUG;
                    } else if (byte == 0xEE) { // Switch To UAVLink Mode
                        d->step = STX1;
                        d->device->write("UVL\r\n");
                        d->type = ComManager::UAVLINK;
                    } else {
                        *d->ptr++ = byte;
                        d->crc ^= byte;
                        d->step = LTX;
                    }
                    break;
                case LTX:
                    *d->ptr++ = byte;
                    d->crc ^= byte;
                    d->len = byte;
                    if (d->len <= 0) {
                        d->step = STX1;
                    } else {
                        d->step = DTX;
                    }
                    break;
                case DTX:
                    *d->ptr++ = byte;
                    d->crc ^= byte;
                    d->len--;
                    if (d->len == 0) {
                        d->step = CRC8;
                    }
                    break;
                case CRC8:
                    if (d->crc == byte) {
                        println("Data receive OK", 0);
                    }
                    d->step = STX1;
                    break;
            }
        }

    } /* namespace core */
} /* namespace pegasus */
