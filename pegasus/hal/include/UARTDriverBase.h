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

#ifndef UARTDRIVERBASE_H_
#define UARTDRIVERBASE_H_

#include "core/include/ComDeviceBase.h"


namespace pegasus {
    namespace hal {

        template<class UartReg_t, class UartConfig_t>
        class UARTDriverBase : public pegasus::core::ComDeviceBase {
            public:
                UARTDriverBase(UartReg_t* reg, UartConfig_t config) :
                    pegasus::core::ComDeviceBase(),
                    _mReg(reg),
                    _mConfig(config),
                    _mStarted(false) {}

                virtual void setBaudRate(uint32_t bauds) = 0;

            protected:
                UartReg_t* _mReg;
                UartConfig_t _mConfig;
                bool _mStarted;
        };

    }
}



#endif /* UARTDRIVER_H_ */
