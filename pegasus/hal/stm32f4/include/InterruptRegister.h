/*
 * InterruptRegister.h
 *
 *  Created on: 29 nov. 2013
 *      Author: alienx
 */

#ifndef INTERRUPTREGISTER_H_
#define INTERRUPTREGISTER_H_

#include "hal/include/InterruptListener.h"
#include "hal/include/ByteListener.h"
#include "hal/stm32f4/include/Defs.h"
#include "hal/stm32f4/include/TimerChannel.h"
#include "hal/stm32f4/include/Gpio.h"
#include "fc/include/Engine.h"

#define HANDLER_EXT_START       0x00
#define HANDLER_EXT_SIZE        7 // (line EXTI0,1,2,3,4 + EXTI5_9 + EXTI10_15) 5 + 2 = 7 spaces
#define HANDLER_TIM_START       (EXT_START + EXT_SIZE)
#define HANDLER_TIM_SIZE        8 // TIM1(4) + TIM2,3,4 +  TIM8(4) + TIM5 + TIM6 + TIM7
#define HANDLER_SIZE            (TIM_START + TIM_SIZE)

namespace pegasus {
    namespace hal {
        namespace stm32f4{

            class InterruptRegister
            {
                public:
                    InterruptRegister() = delete;

                    static bool attachExternalInt(pegasus::hal::InterruptListener* listener, Gpio* io);

                    static bool registerService(pegasus::hal::InterruptListener* listener, pegasus::fc::service::Service serviceId);
                    static void callService(pegasus::fc::service::Service serviceId);

                    static bool attachTimerInt(pegasus::hal::InterruptListener* listener, TimerChannel* channel);

                    static bool attachUARTInterrupt(pegasus::hal::ByteListener* listener, USART_TypeDef* reg);
                    static void UARTInt(USART_TypeDef*, uint8_t);

                    static pegasus::hal::InterruptListener* extHandlers[15]; // 15 interrupt
                    static pegasus::hal::InterruptListener* serviceHandler[pegasus::fc::service::SERVICE_SIZE];
                    static pegasus::hal::InterruptListener* timListener[64]; // TODO calc size
                    static pegasus::hal::ByteListener* uartListener[5];
            };

           /* template<class Entry_t>
            bool InterruptRegister::attachExternalInt(Entry_t handler, Gpio* io) {
                    Func h = reinterpret_cast<Func>(*handler);
                    return attachExternalInt(h, io);
            }

            template<class Entry_t>
            bool InterruptRegister::attachExternalInt(Entry_t* handler, void *member, Gpio* io) {
                    //Func h = reinterpret_cast<Func>(*handler);
                    return false;//attachExternalInt(h, io);
            }*/

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */

#endif /* INTERRUPTREGISTER_H_ */
