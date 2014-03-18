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

            void SpiDevice::setSpeed(uint8_t speed) {
                _mSpi->setSpeed(speed);
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

            void SpiDevice::read(volatile uint8_t* buffer, uint8_t addr, uint16_t size)
            {
                _mCsPin->low();
                _mSpi->read(buffer, addr, size);
                _mCsPin->high();
            }

            void SpiDevice::writeByte(uint8_t addr, uint8_t data)
            {
                _mCsPin->low();
                _mSpi->writeByte(addr, data);
                _mCsPin->high();
            }

            void SpiDevice::writeByte(uint8_t addr)
            {
                _mCsPin->low();
                _mSpi->writeByte(addr);
                _mCsPin->high();
            }

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */
