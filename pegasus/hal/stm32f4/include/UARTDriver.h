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

#ifndef UARTDRIVERSTM32F4_H_
#define UARTDRIVERSTM32F4_H_

#include "hal/stm32f4/include/Defs.h"
#include "hal/include/UARTDriverBase.h"
#include "hal/stm32f4/include/Gpio.h"
#include "hal/stm32f4/include/Core.h"

#define RCC_APB2_USART1            ((uint32_t)0x00000010)
#define RCC_APB1_USART2            ((uint32_t)0x00020000)
#define RCC_APB1_USART3            ((uint32_t)0x00040000)
#define RCC_APB1_USART4            ((uint32_t)0x00080000)
#define RCC_APB1_USART5            ((uint32_t)0x00100000)
#define RCC_APB2_USART6            ((uint32_t)0x00000020)
#define RCC_APB1_USART7            ((uint32_t)0x40000000)
#define RCC_APB1_USART8            ((uint32_t)0x80000000)

namespace pegasus {
    namespace hal {

        namespace uart {

            enum PORT {
                UART_1,
                UART_2,
                UART_3,
                UART_4,
                UART_PORT_COUNT
            };

            typedef struct {
                    pegasus::hal::gpio::pin RX;
                    pegasus::hal::gpio::pin TX;
                    pegasus::hal::gpio::AlternateFunction AF;
                    uint32_t bauds;
            } UartConfig;

            extern const UartConfig uartConfig[UART_PORT_COUNT];
        }

        typedef pegasus::hal::UARTDriverBase<USART_TypeDef, pegasus::hal::uart::UartConfig> UARTDriverBase_t;

        namespace stm32f4 {
            using namespace pegasus::hal::uart;



            class UARTDriver : public UARTDriverBase_t
            {
                public:
                    UARTDriver(USART_TypeDef* reg, UartConfig config);

                    void init();
                    bool open();
                    bool close();


                    void write(uint8_t c);
                    void receive(uint8_t byte);

                private:
                    void setBaudRate();
            };

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */

#endif /* UARTDRIVER_H_ */
