/*
 * SpiDevice.h
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#ifndef SPIDEVICE_H_
#define SPIDEVICE_H_

#include "hal/include/SpiDeviceBase.h"
#include "hal/stm32f4/include/SpiDriver.h"
#include "hal/stm32f4/include/Gpio.h"

namespace pegasus {
    namespace hal {

        typedef pegasus::hal::SpiDeviceBase<stm32f4::SpiDriver, stm32f4::Gpio> SpiDeviceBase_t;

        namespace stm32f4 {

            class SpiDevice : public pegasus::hal::SpiDeviceBase_t
            {
                public:
                    SpiDevice(SpiDriver* spi, Gpio* cs);


                    void write(uint8_t* buffer, uint8_t addr, uint16_t size);
                    void writeByte(uint8_t addr, uint8_t data);
                    void read(uint8_t* buffer, uint8_t addr, uint16_t size);
                    void read(volatile uint8_t* buffer, uint8_t addr, uint16_t size);

                    void setSpeed(uint8_t speed);
            };

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */

#endif /* SPIDEVICE_H_ */
