/*
 * Spi.cpp
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#include "hal/stm32f4/include/SpiDriver.h"

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {

            using namespace pegasus::hal::spi;
            using namespace pegasus::hal::gpio;

            SpiDriver::SpiDriver(SPI_TypeDef* reg, SpiPort spiConfig  ) :
                SpiDriverBase_t(reg, spiConfig) {

                uint16_t tmpReg;

                Gpio miso(_mSpiConfig.MISO.port, _mSpiConfig.MISO.bit);
                Gpio mosi(_mSpiConfig.MOSI.port, _mSpiConfig.MOSI.bit);
                Gpio  sck(_mSpiConfig.SCK.port, _mSpiConfig.SCK.bit);
                miso.mode(Mode::AF_PD);
                miso.setAlternateFunction(_mSpiConfig.AF);
                mosi.mode(Mode::AF_PD);
                mosi.setAlternateFunction(_mSpiConfig.AF);
                sck.mode(Mode::AF_PD);
                sck.setAlternateFunction(_mSpiConfig.AF);

                if (reg == SPI1) {
                    RCC->APB2ENR &= ~(RCC_SPI1); // Reset
                    RCC->APB2ENR |= RCC_SPI1;
                } else if (reg == SPI2) {
                	RCC->APB1ENR &= ~(RCC_SPI2);
                    RCC->APB1ENR |= RCC_SPI2;
                } else if (reg == SPI5) {
                    RCC->APB2ENR &= ~(RCC_SPI5); // Reset
                    RCC->APB2ENR |= RCC_SPI5;
                }

                tmpReg = reg->CR1;
                tmpReg &= ((uint16_t)0x3000); // Reset register
                tmpReg |= (uint16_t)((uint32_t)SPI_MODE_MASTER | SPI_DATASIZE_8B |
                                               SPI_CPOL_LOW | SPI_CPHA_1EDGE |
                                               SPI_NSS_SOFT | SPI_BAUDPSC_8 | SPI_LSBFIRST_MSB);
                reg->CR1 = tmpReg;
                reg->I2SCFGR &= (uint16_t)~((uint16_t)SPI_I2SCFGR_I2SMOD); // Set mod SPI 0
                reg->CRCPR = 7; //Reset value
                reg->CR1 |= SPI_CR1_SPE; // Enable SPI
            }

            void SpiDriver::setSpeed(uint8_t speed) {
                uint16_t tmpReg;
                tmpReg = _mReg->CR1;
                tmpReg &= ~(0x38); // Reset
                tmpReg |= speed;

                _mReg->CR1 = tmpReg;
            }

            void SpiDriver::write(uint8_t* buffer, uint8_t addr, uint16_t size)
            {
                if (size > 1) {
                    //addr |= (uint8_t)SPI_MULTIPLEBYTE_CMD;
                }

                sendByte(addr);
                while (size >= 0x01) {
                    sendByte(*buffer);
                    buffer++;
                    size--;
                }
            }

            void SpiDriver::writeByte(uint8_t addr, uint8_t data)
            {
                sendByte(addr);
                sendByte(data);
            }

            void SpiDriver::writeByte(uint8_t addr)
            {
                sendByte(addr);
            }

            void SpiDriver::read(uint8_t* buffer, uint8_t addr, uint16_t size)
            {
                /*if (size > 1) {
                    addr |= (uint8_t)(SPI_READWRITE_CMD | SPI_MULTIPLEBYTE_CMD);
                } else {
                    addr |= (uint8_t)SPI_READWRITE_CMD;
                }*/


                sendByte(addr);
                while (size > 0x00) {
                    *buffer = sendByte(0x0);
                    buffer++;
                    size--;
                    //buffer++;
                }
            }

            void SpiDriver::read(volatile uint8_t* buffer, uint8_t addr, uint16_t size)
            {
                sendByte(addr);
                while (size > 0x00) {
                    *buffer = sendByte(0x0);
                    buffer++;
                    size--;
                    //buffer++;
                }
            }

            uint8_t SpiDriver::sendByte(uint8_t byte)
            {
                // TODO add timeout

                while ( (_mReg->SR & 0x2) ==  0); // Wait tx buffer empty
                _mReg->DR = (uint16_t)byte;       // Send byte
                while ( (_mReg->SR & 0x1) ==  0); // Wait for receive byte

                return (uint8_t)_mReg->DR;        // Return received byte
            }

        } /* namespace stm32f4 */

    } /* namespace hal */
} /* namespace pegasus */
