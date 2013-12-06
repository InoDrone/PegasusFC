/*
 * Spi.h
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#ifndef SPIDRIVERSTM32_H_
#define SPIDRIVERSTM32_H_

#include "hal/stm32f4/include/Defs.h"
#include "hal/include/SpiDriverBase.h"
#include "hal/stm32f4/include/Gpio.h"

#define RCC_SPI1        ((uint32_t)0x00001000)
#define RCC_SPI2        ((uint32_t)0x00004000)
#define RCC_SPI3        ((uint32_t)0x00008000)
#define RCC_SPI4        ((uint32_t)0x00002000)
#define RCC_SPI5        ((uint32_t)0x00100000)
#define RCC_SPI6        ((uint32_t)0x00200000)

#define SPI_MODE_MASTER ((uint16_t)0x0104)

#define SPI_DATASIZE_16B                ((uint16_t)0x0800)
#define SPI_DATASIZE_8B                 ((uint16_t)0x0000)

#define SPI_CPOL_LOW                   ((uint16_t)0x0000)
#define SPI_CPOL_HIGH                  ((uint16_t)0x0002)

#define SPI_CPHA_1EDGE                  ((uint16_t)0x0000)
#define SPI_CPHA_2EDGE                  ((uint16_t)0x0001)

#define SPI_NSS_SOFT                    ((uint16_t)0x0200)
#define SPI_NSS_HARD                    ((uint16_t)0x0000)

#define SPI_BAUDPSC_2         ((uint16_t)0x0000)
#define SPI_BAUDPSC_4         ((uint16_t)0x0008)
#define SPI_BAUDPSC_8         ((uint16_t)0x0010)
#define SPI_BAUDPSC_16        ((uint16_t)0x0018)
#define SPI_BAUDPSC32         ((uint16_t)0x0020)
#define SPI_BAUDPSC_64        ((uint16_t)0x0028)
#define SPI_BAUDPSC_128       ((uint16_t)0x0030)
#define SPI_BAUDPSC_256       ((uint16_t)0x0038)

#define SPI_LSBFIRST_MSB                ((uint16_t)0x0000)
#define SPI_LSBFIRST_LSB                ((uint16_t)0x0080)

namespace pegasus{
    namespace hal {

        namespace spi {

            enum PORT {
                SPI_1,
                SPI_2,
                SPI_3,
                SPI_4,
                SPI_5,
                SPI_6,
                SPI_PORT_COUNT
            };

            typedef struct {
                    pegasus::hal::gpio::pin MOSI;
                    pegasus::hal::gpio::pin MISO ;
                    pegasus::hal::gpio::pin SCK;
                    pegasus::hal::gpio::AlternateFunction AF;
            } SpiPort;

            extern const SpiPort spiPort[SPI_PORT_COUNT];
        }

        namespace stm32f4 {
            using namespace pegasus::hal::spi;

            typedef pegasus::hal::SpiDriverBase<SPI_TypeDef, SpiPort> SpiDriverBase_t;

            class SpiDriver : public SpiDriverBase_t
            {
                public:
                    SpiDriver(SPI_TypeDef* reg, SpiPort spiPort);

                    void write(uint8_t* buffer, uint8_t addr, uint16_t size);
                    void read(uint8_t* buffer, uint8_t addr, uint16_t size);
                    uint8_t sendByte(uint8_t byte);

            };

        } /* namespace stm32f4 */

    } /* namespace hal */
} /* namespace pegasus */

#endif /* SPIDRIVERSTM32_H_ */
