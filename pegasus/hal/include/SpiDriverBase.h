/*
 * SpiBase.h
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#ifndef SPIDRIVERBASE_H_
#define SPIDRIVERBASE_H_

#include <stdint.h>
#define SPI_READWRITE_CMD              ((uint8_t)0x80)
#define SPI_MULTIPLEBYTE_CMD           ((uint8_t)0x40)


namespace pegasus {
    namespace hal {

        template<class SpiReg_t, typename SpiConfig_t>
        class SpiDriverBase {
            public:
                SpiDriverBase(SpiReg_t* reg, SpiConfig_t spiConfig) :
                    _mReg(reg),
                    _mSpiConfig(spiConfig)
                    {}

                virtual void write(uint8_t* buffer, uint8_t addr, uint16_t size) = 0;
                virtual void read(uint8_t* buffer, uint8_t addr, uint16_t size) = 0;
                virtual uint8_t sendByte(uint8_t byte) = 0;

            protected:
                SpiReg_t*      _mReg;
                SpiConfig_t   _mSpiConfig;
        };

    }
}



#endif /* SPIDRIVERBASE_H_ */
