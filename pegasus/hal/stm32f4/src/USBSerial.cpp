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

#include <string.h>
#include "hal/stm32f4/include/USBSerial.h"
#include "hal/stm32f4/include/Processor.h"
#include "core/include/MainTimer.h"
#include "usb_dcd_int.h"

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {

            //USB_OTG_CORE_HANDLE  USBSerial::USB_OTG_dev;

            USBSerial::USBSerial() :
                    _mDiscPin(pegasus::gpio::D, pegasus::gpio::PIN0)
            {
                _mDiscPin.mode(pegasus::gpio::Mode::OUTPUT_OD);
            }

            void USBSerial::init()
            {

            }

            bool USBSerial::open()
            {
                return false;

                _mDiscPin.low();
                pegasus::core::mainTimer.delay(200);
                _mDiscPin.high();
                //initUSBSerial(&USB_OTG_dev);

                _mConnected = true;

                return true;
            }

            bool USBSerial::close()
            {
                _mConnected = false;
                return true;
            }

            /*uint32_t USBSerial::write(const uint8_t *buffer, uint32_t len)
            {
                VCP_DataTx((uint8_t*)buffer, len);

                return len;
            }

            uint32_t USBSerial::write(const char *msg)
            {
                write((uint8_t*)msg, strlen(msg));
                return 0;
            }*/

            void USBSerial::write(uint8_t c)
            {
                //VCP_put_char(c);
            }

            namespace InterruptHandler
            {
                void OTGFSInt()
                {
                    pegasus::hal::stm32f4::Processor::disableInterrupts();
                    //USBD_OTG_ISR_Handler (&USBSerial::USB_OTG_dev);
                    pegasus::hal::stm32f4::Processor::enableInterrupts();
                }
            }

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */
