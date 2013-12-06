/*
 * SpiDevice.cpp
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#include "hal/stm32f4/include/SpiDevice.h"

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {

            SpiDevice::SpiDevice(SpiDriver* spi, Gpio* cs) :
                    SpiDeviceBase_t(spi, cs)
            {
                _mCsPin->mode(pegasus::gpio::Mode::OUTPUT);
                _mCsPin->high();
            }

            void SpiDevice::write(uint8_t* buffer, uint8_t addr, uint16_t size)
            {
                _mCsPin->low();
                _mSpi->write(buffer, addr, size);
                _mCsPin->high();
            }

            void SpiDevice::read(uint8_t* buffer, uint8_t addr, uint16_t size)
            {
                _mCsPin->low();
                _mSpi->read(buffer, addr, size);
                _mCsPin->high();
            }

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */
