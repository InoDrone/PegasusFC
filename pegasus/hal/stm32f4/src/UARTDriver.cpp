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

#include "hal/stm32f4/include/UARTDriver.h"

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {
            using namespace pegasus::hal::uart;
            using namespace pegasus::hal::gpio;

            UARTDriver::UARTDriver(USART_TypeDef* reg, UartConfig config) :
                UARTDriverBase_t(reg, config){}

            void UARTDriver::init()
            {

                if (_mReg == USART3) {
                    RCC->APB1ENR |= RCC_APB1_USART3;
                }

                Gpio txPin(_mConfig.TX.port, _mConfig.TX.bit);
                Gpio rxPin(_mConfig.RX.port, _mConfig.RX.bit);
                txPin.mode(Mode::AF_PU);
                txPin.setAlternateFunction(_mConfig.AF);
                rxPin.mode(Mode::AF_PU);
                rxPin.setAlternateFunction(_mConfig.AF);

                uint32_t cr1 = _mReg->CR1;
                uint32_t cr2 = _mReg->CR2;
                uint32_t cr3 = _mReg->CR3;
                cr1 &= (uint16_t)~((uint16_t) 0xFFFF); // Reset register
                cr2 &= (uint16_t)~((uint16_t) 0xFFFF); // Reset Register
                cr3 &= (uint16_t)~((uint16_t) 0xFFFF); // Reset Register

                // 1 start, 8 bits, n stop
                cr1 |= 0x0C; // Enable TX/RX line
                //cr1 |= 0x20; // Enable RX interrupt

                _mReg->CR1 = cr1;
                _mReg->CR2 = cr2;
                _mReg->CR3 = cr3;

                setBaudRate();

            }

            bool UARTDriver::open()
            {
                _mReg->CR1 |= USART_CR1_UE;
                _mStarted = true;

                return true;
            }

            bool UARTDriver::close()
            {
                _mReg->CR1 &= ~(USART_CR1_UE);
                _mStarted = false;

                return true;
            }

            void UARTDriver::setBaudRate()
            {
                uint32_t integer = 0, fractionnal = 0, tmp = 0;
                Clocks clocks;
                Core::getSystemClock(&clocks);

                uint32_t clockSpeed;
                if (_mReg == USART1 || _mReg == USART6) {
                    clockSpeed = clocks.PCLK2;
                } else {
                    clockSpeed = clocks.PCLK1;
                }

                // if OVER8=0 only
                integer = (25 * clockSpeed) / ( 4 * _mConfig.bauds);
                tmp = (integer / 100) << 4;
                fractionnal = integer - (100 * (tmp >> 4));
                tmp |= (((fractionnal * 16) + 50) / 100) & ((uint8_t)0x0F);

                _mReg->BRR = (uint16_t)tmp;
            }

            uint32_t UARTDriver::write(const uint8_t *buffer, uint32_t len)
            {
                uint32_t i = 0;
                while ( (_mReg->SR & USART_SR_TXE) && (i < len)) {
                    _mReg->DR = buffer[i++];
                }

                return i;
            }

            uint32_t UARTDriver::write(const char *msg)
            {
                while(*msg) {
                    while ( !(_mReg->SR & USART_SR_TC) );
                    _mReg->DR = *msg++;
                }

                return 1;
            }

            void UARTDriver::write(char c) {
                while ( !(_mReg->SR & USART_SR_TC) );
                _mReg->DR = c;
            }

            /*uint32_t UARTDriver::write(const uint8_t buffer[])
            {
                uint32_t i;
                while ( (_mReg->SR & USART_SR_TXE) && (*buffer)) {
                    _mReg->DR = *buffer++;
                }

                return i;
            }*/

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */
