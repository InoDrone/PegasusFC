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
#include "fc/include/Engine.h"
#include <cstdio>

namespace pegasus
{
    namespace core
    {

        ComManager::ComManager() :
                countDevice(0),
                lastPong(0)
        {
            txMutex = Mutex::create();
        }

        void ComManager::addDevice(ComDeviceBase* device) {
            if (countDevice >= COMMANAGER_MAX_DEVICE) {
                return;
            }

            device->init();
            device->addReceiver(this, countDevice);
            _mDevices[countDevice].device = device;
            _mDevices[countDevice].type = ComManager::DEBUG;
            uavlink_init(&_mDevices[countDevice].status);
            device->open();

            countDevice++;
        }

        void ComManager::write(char c, uint8_t type) {
            uint8_t i;

            //Mutex::enter(txMutex);
            for (i=0;i<countDevice;i++) {
                if (_mDevices[i].device->isConnected() && _mDevices[i].type == type) {
                    _mDevices[i].device->write(c);
                }
            }
            //Mutex::leave(txMutex);
        }

        void ComManager::print(const char msg[], uint8_t type)
        {
            uint8_t i;

            Mutex::enter(txMutex);
            for (i=0;i<countDevice;i++) {
                if (_mDevices[i].device->isConnected() && _mDevices[i].type == type) {
                    _mDevices[i].device->write(msg);
                }
            }
            Mutex::leave(txMutex);
        }

        void ComManager::println(const char msg[], uint8_t type)
        {
            print (msg, type);
            print ("\r\n", type);
        }

        void ComManager::send(const uavlink_message_t msg)
        {
            uint8_t i;
            uint8_t buffer[255];
            uint16_t size = 0;
            size = uavlink_get_buffer(buffer, msg);

            Mutex::enter(txMutex);
            for (i=0;i<countDevice;i++) {
                if (_mDevices[i].device->isConnected() && _mDevices[i].type == ComManager::UAVLINK) {
                    _mDevices[i].device->write((const char*)buffer, size);
                }
            }
            Mutex::leave(txMutex);
        }

        void ComManager::send(uavlink_message_t msg, uint8_t id)
        {
            Mutex::enter(txMutex);
            if (!_mDevices[id].device->isConnected() || !_mDevices[id].type == ComManager::UAVLINK) {
                return;
            }

            uint8_t buffer[255];
            uint16_t size = 0;
            size = uavlink_get_buffer(buffer, msg);

            _mDevices[id].device->write((const char*)buffer, size);
            Mutex::leave(txMutex);
        }

        void ComManager::ping()
        {
            send(uavlink_generate_state(UAVLINK_PING));
        }

        bool ComManager::isActive()
        {
            if (lastPong == 0) return false;

            return (mainTimer.millis() - lastPong) < 1000;
        }

        void ComManager::task()
        {
            uint8_t c;
            for (uint8_t i=0;i<countDevice;i++) {
                if (_mDevices[i].device->isConnected() && _mDevices[i].device->available()) {
                    c = _mDevices[i].device->read();
                    receive(c, i);
                }
            }
        }

        void ComManager::receive(uint8_t byte, uint8_t id) {

            uavlink_message_t msg;
            uint8_t ack;
            if (uavlink_parse(byte, &_mDevices[id].status, &msg) == UAVLINK_VALID_MSG) {
                lastPong = mainTimer.millis();
                if (msg.cmd == UAVLINK_STATE) {
                    switch (msg.datas[0]) {
                        case UAVLINK_PING: // Kikoolol it's Me !
                            //if (_mDevices[id].type == ComManager::DEBUG) {
                                _mDevices[id].device->write("ACK\r\n");
                                lastPong = mainTimer.millis();
                            //} else {
                            //    send(uavlink_generate_ack(msg.cmd, ack), id);
                            //}
                            return;
                        case UAVLINK_DEBUG: // to Debug/Shell
                            _mDevices[id].type = ComManager::DEBUG;
                            _mDevices[id].device->write("DBG\r\n");
                            return;
                        case UAVLINK_START: // to UAVLink
                            _mDevices[id].type = ComManager::UAVLINK;
                            send(uavlink_generate_ack(msg.cmd, 1), id);
                            return;
                    }
                }

                ack = pegasus::fc::engine.uavlinkReceive(msg);
                send(uavlink_generate_ack(msg.cmd, ack), id);

            }
        }

        /* Syscall for printf etc ... */
        extern "C" {
            int _write(int file, char *ptr, int len) {
                        int i;
                        for(i = 0;i < len;i++) {
                                com.write(ptr[i], 0);
                        }
                        return len;
            };
        }

    } /* namespace core */
} /* namespace pegasus */
