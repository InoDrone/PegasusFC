/*
 * InterruptRegister.cpp
 *
 *  Created on: 29 nov. 2013
 *      Author: alienx
 */

#include "hal/stm32f4/include/InterruptRegister.h"
#include "hal/stm32f4/include/Processor.h"

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {
            pegasus::hal::InterruptListener* InterruptRegister::extHandlers[15]; // 15 interrupt
            pegasus::hal::InterruptListener* InterruptRegister::serviceHandler[pegasus::fc::service::SERVICE_SIZE];
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
                tmp = ((uint32_t) 0x0F) << (bit * 0x04);
                SYSCFG->EXTICR[bit >> 0x02] &= ~tmp;
                SYSCFG->EXTICR[bit >> 0x02] |= port << (bit * 0x04);

                if (bit > 4 && bit < 10) {
                    irq = EXTI9_5_IRQn;
                } else if (bit > 9) {
                    irq = EXTI15_10_IRQn;
                } else {
                    irq = EXTI0_IRQn + bit;
                }

                NVIC->IP[irq] = 0x0F << 0x04; // TODO change to dynamic prio
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
                } else {
                    return false; // TODO finish
                }

                NVIC->IP[irq] = 0x0A << 0x04; // TODO change to dynamic prio
                NVIC->ISER[irq >> 0x05] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);

                timListener[timerId] = listener;
                return true;
            }

            bool InterruptRegister::registerService(pegasus::hal::InterruptListener* listener, pegasus::fc::service::Service serviceId)
            {
                if (serviceHandler[serviceId]) {
                    return false;
                }

                serviceHandler[serviceId] = listener;
                return true;
            }

            void InterruptRegister::callService(pegasus::fc::service::Service serviceId) {
                if (serviceHandler[serviceId]) {
                    pegasus::hal::InterruptListener::trampoline(serviceHandler[serviceId]);
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
                }

                if (uartListener[id]) return false;

                reg->CR1 |= USART_CR1_RXNEIE; // Enable receive Int
                NVIC->IP[irq] = 0x0A << 0x04; // TODO change to dynamic prio
                NVIC->ISER[irq >> 0x05] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);

                uartListener[id] = listener;

                return true;
            }

            /**
             * Global USART interrupt
             */
            void InterruptRegister::UARTInt(USART_TypeDef* reg, uint8_t id)
            {
                Processor::disableInterrupts();
                if (reg->SR & USART_SR_RXNE) {
                    uint8_t byte = reg->DR;
                    if (uartListener[id]) {
                        pegasus::hal::ByteListener::trampoline(uartListener[id], byte);
                    }
                    reg->SR &= ~(USART_SR_RXNE); // Clear int flag
                } else if (reg->SR & USART_SR_TXE) {
                    reg->SR &= ~(USART_SR_TXE); // Clear int flag
                }
                Processor::enableInterrupts();
            }

            namespace InterruptHandler
            {
                using namespace pegasus::hal::stm32f4;

                void EXTIx() {
                    Processor::disableInterrupts();
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
                    Processor::enableInterrupts();
                }

                void TIM2Int()
                {

                }

                void TIM3Int()
                {
                    Processor::disableInterrupts();
                    uint32_t state = ((TIM3->DIER  & TIM3->SR) >> 1) & 0xF;// & 0x5F;
                    uint8_t initPos = 8, j=0, bit=0;
                    while (j<4) {
                        bit = _BIT(j);
                        if ( (state) & (bit) ) {
                            if (InterruptRegister::timListener[initPos+j]) {
                                pegasus::hal::InterruptListener::trampoline(InterruptRegister::timListener[initPos+j]);
                            }

                            TIM3->SR &= ~(bit << 1);
                        }
                        j++;
                    }
                    Processor::enableInterrupts();
                }

                void TIM4Int()
                {

                }

                void TIM5Int()
                {

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
