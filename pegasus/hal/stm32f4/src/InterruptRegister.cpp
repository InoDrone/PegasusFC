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

#include "hal/stm32f4/include/InterruptRegister.h"
#include "hal/stm32f4/include/Processor.h"
#include "hal/stm32f4/include/Processor.h"

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {
            pegasus::hal::InterruptListener* InterruptRegister::extHandlers[15]; // 15 interrupt
            pegasus::hal::ServicesListener* InterruptRegister::serviceListener[pegasus::fc::service::SERVICE_SIZE];
            pegasus::hal::InterruptListener* InterruptRegister::timListener[64]; // TODO calc size
            pegasus::hal::ByteListener* InterruptRegister::uartListener[5];


            bool InterruptRegister::attachExternalInt(pegasus::hal::InterruptListener* listener, Gpio* io)
            {
                uint32_t tmp;
                uint8_t bit = io->getBit();
                uint8_t port = io->getPort();
                uint32_t irq;

                io->mode(pegasus::gpio::Mode::INPUT);

                if (extHandlers[bit]) {
                    return false;
                }

                if ( (RCC->APB2ENR & RCC_APB2Periph_SYSCFG) == 0) {
                    RCC->APB2ENR |= RCC_APB2Periph_SYSCFG;
                }


                EXTI->IMR  &= ~(0x01 << bit);
                EXTI->EMR  &= ~(0x01 << bit);
                EXTI->RTSR &= ~(0x01 << bit);
                EXTI->FTSR &= ~(0x01 << bit);

                EXTI->IMR  |= (uint32_t) (0x1 << bit); // Enable interrupt
                EXTI->RTSR |= (uint32_t) (0x1 << bit); // Detect on Rising Edge

                // Attach pin to interrupt
                tmp = ((uint32_t) 0x0F) << ((bit & 0x03) * 0x04);
                SYSCFG->EXTICR[bit >> 0x02] &= ~tmp;
                SYSCFG->EXTICR[bit >> 0x02] |= port << ((bit & 0x03) * 0x04);

                if (bit > 4 && bit < 10) {
                    irq = EXTI9_5_IRQn;
                } else if (bit > 9) {
                    irq = EXTI15_10_IRQn;
                } else {
                    irq = EXTI0_IRQn + bit;
                }

                NVIC->IP[irq] = HIGH_PRIORITY_SENSOR; // TODO change to dynamic prio
                NVIC->ISER[irq >> 0x05] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);

                extHandlers[bit] = listener;

                return true;
            }

            bool InterruptRegister::attachTimerInt(pegasus::hal::InterruptListener* listener, TimerChannel* channel)
            {
                uint8_t timerId = channel->getUniqId();
                uint32_t irq;
                __IO TIM_TypeDef* reg = channel->getTimer()->getReg();

                if (timListener[timerId]) return false;
                reg->DIER |= (0x02 << channel->getChannel()); // Enable OC/IC interrupt to channel
                if (reg == TIM1) {
                    irq = TIM1_CC_IRQn;
                } else if (reg == TIM2) {
                    irq = TIM2_IRQn;
                } else if (reg == TIM3) {
                    irq = TIM3_IRQn;
                } else if (reg == TIM4) {
                    irq = TIM4_IRQn;
                } else if (reg == TIM5) {
                    irq = TIM5_IRQn;
                } else if (reg == TIM6) {
                    irq = TIM6_DAC_IRQn;
                } else if (reg == TIM7) {
                    irq = TIM7_IRQn;
                } else if (reg == TIM8) {
                    irq = TIM8_CC_IRQn;
                } else if (reg == TIM9) {
                    irq = TIM1_BRK_TIM9_IRQn;
                } else {
                    return false; // TODO finish
                }

                NVIC->IP[irq] = HIGH_PRIORITY_RC; // TODO change to dynamic prio
                NVIC->ISER[irq >> 0x05] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);

                timListener[timerId] = listener;
                return true;
            }

            bool InterruptRegister::attachTimerInt(pegasus::hal::InterruptListener* listener, TimerBase_t* timer)
            {
                uint8_t timerId = timer->getUniqId();
                uint32_t irq;
                __IO TIM_TypeDef* reg = timer->getReg();

                reg->DIER |= 0x01; // Enable UIF interrupt to channel
                if (reg == TIM1) {
                    irq = TIM1_CC_IRQn;
                } else if (reg == TIM2) {
                    irq = TIM2_IRQn;
                } else if (reg == TIM3) {
                    irq = TIM3_IRQn;
                } else if (reg == TIM4) {
                    irq = TIM4_IRQn;
                } else if (reg == TIM5) {
                    irq = TIM5_IRQn;
                } else if (reg == TIM6) {
                    irq = TIM6_DAC_IRQn;
                } else if (reg == TIM7) {
                    irq = TIM7_IRQn;
                } else if (reg == TIM8) {
                    irq = TIM8_UP_TIM13_IRQn;
                } else if (reg == TIM9) {
                    irq = TIM1_BRK_TIM9_IRQn;
                } else if (reg == TIM10) {
                    irq = TIM1_UP_TIM10_IRQn;
                } else {
                    return false; // TODO finish
                }

                if (timListener[timerId]) return false;

                NVIC->IP[irq] = MEDIUM_PRIORITY_FC; // TODO change to dynamic prio
                NVIC->ISER[irq >> 0x05] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);

                timListener[timerId] = listener;
                return true;
            }

            bool InterruptRegister::registerService(pegasus::hal::ServicesListener* listener, pegasus::fc::service::Service serviceId)
            {
                if (serviceListener[serviceId]) {
                    return false;
                }

                serviceListener[serviceId] = listener;
                return true;
            }

            void InterruptRegister::callServiceInterrupt( pegasus::fc::service::Service serviceId )
            {
                if (serviceListener[serviceId]) {
                    pegasus::hal::ServicesListener::trampoline(serviceListener[serviceId], serviceId);
                }
            }

            /**
             * Attache UART receiver interrupt
             */
            bool InterruptRegister::attachUARTInterrupt(pegasus::hal::ByteListener* listener, USART_TypeDef* reg)
            {
                uint8_t id;
                uint32_t irq;

                if (reg == USART1) {
                    irq = USART1_IRQn;
                    id = 0;
                } else if(reg == USART2) {
                    irq = USART2_IRQn;
                    id = 1;
                } else if(reg == USART3) {
                    irq = USART3_IRQn;
                    id = 2;
                } else if(reg == UART4) {
                    irq = UART4_IRQn;
                    id = 3;
                } else if(reg == UART5) {
                    irq = UART5_IRQn;
                    id = 4;
                } else {
                    return false;
                }

                if (uartListener[id]) return false;

                reg->CR1 |= USART_CR1_RXNEIE; // Enable receive Int
                NVIC->IP[irq] = LOW_PRIORITY; // TODO change to dynamic prio
                NVIC->ISER[irq >> 0x05] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);

                uartListener[id] = listener;

                return true;
            }

            /**
             * Global USART interrupt
             */
            void InterruptRegister::UARTInt(USART_TypeDef* reg, uint8_t id)
            {
                Processor::setBasePri(MEDIUM_PRIORITY);
                if (reg->SR & USART_SR_RXNE) {
                    uint8_t byte = reg->DR;
                    if (uartListener[id]) {
                        pegasus::hal::ByteListener::trampoline(uartListener[id], byte);
                    }
                    reg->SR &= ~(USART_SR_RXNE); // Clear int flag
                }/* else if (reg->SR & USART_SR_TXE) {
                    reg->SR &= ~(USART_SR_TXE); // Clear int flag
                }*/
                Processor::setBasePri(0);
            }

            /**
             * Global Timer interrupt
             */
            void InterruptRegister::TIMInt(TIM_TypeDef* reg, uint8_t id)
            {
                Processor::setBasePri(MEDIUM_PRIORITY_FC);
                uint32_t state = ((reg->DIER  & reg->SR) & 0x1F);// 0b11111;
                uint8_t j=0, bit=0;
                while (j<5) {
                    bit = _BIT(j);
                    if ( (state) & (bit) ) {
                        if (InterruptRegister::timListener[id+j]) {
                            pegasus::hal::InterruptListener::trampoline(InterruptRegister::timListener[id+j]);
                        }

                        reg->SR &= ~(bit);
                    }
                    j++;
                }
                Processor::setBasePri(0);
            }

            namespace InterruptHandler
            {
                using namespace pegasus::hal::stm32f4;

                void __attribute__ (( naked )) ServiceCall(void) {
                    asm volatile(
                            "push {lr}           \n"
                            "mrs  r1, PSP        \n"
                            "ldr  r0, [r1, #24]  \n"
                            "sub  r0, r0, #2     \n"
                            "ldrh r0, [r0]       \n"
                            "and r0, r0, #255    \n"
                            "bl %[svcHandler]    \n"
                            "pop {pc}            \n"
                            : /* no output */
                            : [svcHandler] "i"(&InterruptRegister::callServiceInterrupt)
                            : "r0"
                    );

                }

                void EXTIx() {
                    Processor::setBasePri(HIGH_PRIORITY_SENSOR);
                    uint32_t state = EXTI->PR & EXTI->IMR;
                    uint32_t bit = 0;
                    uint8_t j = 0;

                    for (j=0; j<15;j++) {
                        bit = _BIT(j);
                        if ((state & bit) && InterruptRegister::extHandlers[j]) {
                            pegasus::hal::InterruptListener::trampoline(InterruptRegister::extHandlers[j]);
                        }
                    }

                    EXTI->PR = 0xFFFFF; // Clear 16 bit
                    Processor::setBasePri(0);
                }

                void TIM1_9Int()
                {
                    InterruptRegister::TIMInt(TIM9, 40);//((9-1)*5)
                }

                void TIM1_10Int()
                {
                    InterruptRegister::TIMInt(TIM1, 0);//((1-1)*5) , UP Int TIM1
                    InterruptRegister::TIMInt(TIM10, 45);//((10-1)*5)
                }

                void TIM1_11Int()
                {
                    InterruptRegister::TIMInt(TIM11, 50);//((11-1)*5)
                }

                void TIM1CCInt()
                {
                    InterruptRegister::TIMInt(TIM1, 0);//((1-1)*5)
                }

                void TIM2Int()
                {
                    InterruptRegister::TIMInt(TIM2, 5);
                }

                void TIM3Int()
                {
                    InterruptRegister::TIMInt(TIM3, 10);
                }

                void TIM4Int()
                {
                    InterruptRegister::TIMInt(TIM4, 15); //((4-1)*5)
                }

                void TIM5Int()
                {
                    InterruptRegister::TIMInt(TIM5, 20); //((5-1)*5)
                }

                void TIM6Int()
                {
                    InterruptRegister::TIMInt(TIM6, 25); //((6-1)*5)
                }

                void TIM7Int()
                {
                    InterruptRegister::TIMInt(TIM7, 30); //((7-1)*5)
                }

                void UART1Int()
                {
                    InterruptRegister::UARTInt(USART1, 0);
                }

                void UART2Int()
                {
                    InterruptRegister::UARTInt(USART2, 1);
                }

                void UART3Int()
                {
                    InterruptRegister::UARTInt(USART3, 2);
                }

                void UART4Int()
                {
                    InterruptRegister::UARTInt(UART4, 3);
                }

                void UART5Int()
                {
                    InterruptRegister::UARTInt(UART5, 4);
                }
            }

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */
