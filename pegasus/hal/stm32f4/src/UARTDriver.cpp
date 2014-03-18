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
#include "hal/stm32f4/include/InterruptRegister.h"
#include "stm32f4xx_dma.h"

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {
            using namespace pegasus::hal::uart;
            using namespace pegasus::hal::gpio;

            UARTDriver* UARTDriver::dmaPtr[2][7];

            UARTDriver::UARTDriver(USART_TypeDef* reg, UartConfig config) :
                UARTDriverBase_t(reg, config),
                btConnected(false) {}

            void UARTDriver::init()
            {

                if (_mReg == USART1) {
                    RCC->APB2ENR |= RCC_APB2_USART1;
                } else if (_mReg == USART2) {
                    RCC->APB1ENR |= RCC_APB1_USART2;
                } else if (_mReg == USART3) {
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

                memset(&tx, 0, sizeof(DMABuffer));
                memset(&rx, 0, sizeof(DMABuffer));

                enableDMA();

                print("\r\n+INQ=1\r\n");
            }

            void UARTDriver::enableDMA()
            {
                DMA_InitTypeDef DMA_InitStructure;
                NVIC_InitTypeDef NVIC_InitStructure;
                if (_mReg == USART2) {
                    rx.channel = DMA_Channel_4;
                    rx.stream = DMA1_Stream5;
                    rx.streamNum = 5;
                    // RX -> DMA 1 CHANNEL 4 Stream 5
                    // TX -> DMA 1 CHANNEL 4 Stream 6
                    tx.channel = DMA_Channel_4;
                    tx.stream = DMA1_Stream6;
                    tx.streamNum = 6;

                    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
                } else {
                    return;
                }

                // INIT DMA STRUCTURE
                DMA_StructInit(&DMA_InitStructure);
                DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&_mReg->DR;
                DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
                DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
                DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
                DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
                DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
                DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

                // ENABLE RX DMA
                DMA_DeInit(rx.stream);
                DMA_InitStructure.DMA_Channel = rx.channel;
                DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx.buffer;
                DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
                DMA_InitStructure.DMA_BufferSize = sizeof(rx.buffer);
                DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
                DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
                DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
                DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
                DMA_Init(rx.stream, &DMA_InitStructure);

                //DMA_ClearFlag(rxStream, s->rxDmaFlags);
                DMA_Cmd(rx.stream, ENABLE);

                _mReg->CR3 |= 0x40; // Enable USART DMA RX
                rx.pos = DMA_GetCurrDataCounter(rx.stream);

                // ENABLE TX DMA
                DMA_DeInit(tx.stream);
                DMA_InitStructure.DMA_Channel = tx.channel;
                DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
                DMA_InitStructure.DMA_BufferSize = sizeof(tx.buffer);
                DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
                DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
                DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
                DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
                DMA_Init(tx.stream, &DMA_InitStructure);

                DMA_SetCurrDataCounter(tx.stream, 0);
                DMA_ITConfig(tx.stream, DMA_IT_TC, ENABLE);

                _mReg->CR3 |= 0x80; //  Enable TX DMA

                NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
                NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
                NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStructure);
            }

            bool UARTDriver::open()
            {
                _mReg->CR1 |= USART_CR1_UE;
                //InterruptRegister::attachUARTInterrupt(this, _mReg);

                _mStarted = true;
                _mConnected = true;
                #ifndef BT_MODE
                 btConnected = true;
                #endif

                return true;
            }

            bool UARTDriver::close()
            {
                _mReg->CR1 &= ~(USART_CR1_UE);
                _mStarted = false;
                _mConnected = false;

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

            void UARTDriver::write(uint8_t c) {
                if (btConnected) dmaWrite(c);
            }

            void UARTDriver::dmaWrite(uint8_t c)
            {
                tx.buffer[tx.pos] = c;
                tx.pos = (tx.pos + 1) % sizeof(tx.buffer);

                startTxDMA();
            }

            void UARTDriver::print(const char *str)
            {
                    while (*str) {
                        dmaWrite(*str++);
                    }
            }

            void UARTDriver::btParseByte(uint8_t c)
            {
                static uint8_t buffer[256];
                static uint8_t pos = 0;
                static uint8_t step = 0;

                // Check modem command
                if (pos == 0 && c == '\r') {
                    step = 0;
                } else if (c == '\n' && step == 0) {
                    step = 1;
                    pos = 0;
                } else if (step == 1 && c == '\r') {
                    pos = 0;
                    step = 0;

                    if (buffer[0] == 'O' && buffer[1] == 'K') {
                    } else if (buffer[9] == '4') {
                        btConnected = true;
                    } else if (buffer[9] != '4') {
                        btConnected = false;
                        print("\r\n+INQ=1\r\n");
                    }
                    return;
                } else if (step == 1) {
                    buffer[pos++] = c;
                }
            }

            void UARTDriver::startTxDMA()
            {
                int size;
                uint32_t tail = tx.tail;

                if (!tx.enabled && tx.pos != tail) {
                    if (tx.pos > tail) {
                        size = tx.pos - tail;
                        tx.tail = tx.pos;
                    } else {
                        size = sizeof(tx.buffer) - tail;
                        tx.tail = 0;
                    }

                    tx.enabled = true;
                    dmaPtr[0][tx.streamNum] = this;

                    tx.stream->M0AR = (uint32_t)&tx.buffer[tail];
                    tx.stream->NDTR = size;

                    DMA_Cmd(tx.stream, ENABLE);
                }
            }

            void UARTDriver::dmaInterrupt()
            {
                tx.enabled = false;
                startTxDMA();
            }

            uint8_t UARTDriver::read()
            {
                uint8_t c;
                uint16_t bufferSize = sizeof(rx.buffer);
                c = rx.buffer[bufferSize - rx.pos];
                if (--rx.pos == 0) {
                    rx.pos = bufferSize;
                }

                /* Check BT module message */
                btParseByte(c);

                return c;
            }

            bool UARTDriver::available()
            {
                return rx.stream->NDTR != rx.pos;
            }

            namespace InterruptHandler
            {
                void DMA1_Stream6_IRQ()
                {
                    DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6 | DMA_FLAG_HTIF6 | DMA_FLAG_TEIF6 | DMA_FLAG_DMEIF6 | DMA_FLAG_FEIF6 );
                    (pegasus::hal::stm32f4::UARTDriver::dmaPtr[0][6])->dmaInterrupt();
                }
            }

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */
