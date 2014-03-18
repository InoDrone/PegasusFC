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

#ifndef USBSERIAL_H_
#define USBSERIAL_H_

#include "core/include/ComDeviceBase.h"
#include "hal/stm32f4/include/Gpio.h"

#include "usbd_cdc_core.h"

extern "C" {


    //extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
    //extern void initUSBSerial(USB_OTG_CORE_HANDLE* pDev);
}

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {

            class USBSerial : public pegasus::core::ComDeviceBase
            {
                public:
                    USBSerial();

                    void init();
                    bool open();
                    bool close();

                    void write(uint8_t c);
                    uint8_t read();
                    bool available();

                    //static USB_OTG_CORE_HANDLE  USB_OTG_dev;

                private:
                    Gpio _mDiscPin;
            };

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */

#endif /* USBSERIAL_H_ */
