/*
 * SpiDeviceBase.h
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */
#ifndef SPIDEVICESTM32_H_
#define SPIDEVICESTM32_H_

#include <stdint.h>

namespace pegasus {
    namespace hal {

        template<class SpiDriver_t, class Gpio_t>
        class SpiDeviceBase {
            public:
                SpiDeviceBase(SpiDriver_t* reg, Gpio_t* cs) :
                    _mSpi(reg),
                    _mCsPin(cs)
                    {}

                virtual void write(uint8_t* buffer, uint8_t addr, uint16_t size) = 0;
                virtual void read(uint8_t* buffer, uint8_t addr, uint16_t size) = 0;
                virtual void read(volatile uint8_t* buffer, uint8_t addr, uint16_t size) = 0;
                virtual void writeByte(uint8_t addr, uint8_t data) = 0;
                virtual void writeByte(uint8_t addr) = 0;

                virtual void setSpeed(uint8_t speed) = 0;

            protected:
                SpiDriver_t*     _mSpi;
                Gpio_t*       _mCsPin;
        };

    }
}



#endif /* SPIDEVICESTM32_H_ */
