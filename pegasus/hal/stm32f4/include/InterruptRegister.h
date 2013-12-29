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
#include "hal/include/ServicesListener.h"
#include "hal/stm32f4/include/Defs.h"
#include "fc/include/Defs.h"
#include "hal/stm32f4/include/TimerChannel.h"
#include "hal/stm32f4/include/Gpio.h"
//#include "fc/include/Engine.h"

#define HANDLER_EXT_START       0x00
#define HANDLER_EXT_SIZE        7 // (line EXTI0,1,2,3,4 + EXTI5_9 + EXTI10_15) 5 + 2 = 7 spaces
#define HANDLER_TIM_START       (EXT_START + EXT_SIZE)
#define HANDLER_TIM_SIZE        8 // TIM1(4) + TIM2,3,4 +  TIM8(4) + TIM5 + TIM6 + TIM7
#define HANDLER_SIZE            (TIM_START + TIM_SIZE)

//#define CALLSV(code) asm volatile("svc %[svcode]" ::[svcode] "I" (code))
#define CALLSV(code) pegasus::hal::stm32f4::InterruptRegister::callServiceInterrupt(code)

namespace pegasus {
    namespace hal {
        namespace stm32f4{

            class InterruptRegister
            {
                public:
                    static bool attachExternalInt(pegasus::hal::InterruptListener* listener, Gpio* io);

                    static bool registerService(pegasus::hal::ServicesListener* listener, pegasus::fc::service::Service serviceId);
                    static void callServiceInterrupt(pegasus::fc::service::Service serviceId);

                    static bool attachTimerInt(pegasus::hal::InterruptListener* listener, TimerChannel* channel);
                    static bool attachTimerInt(pegasus::hal::InterruptListener* listener, TimerBase_t* timer);
                    static void TIMInt(TIM_TypeDef* reg, uint8_t id);

                    static bool attachUARTInterrupt(pegasus::hal::ByteListener* listener, USART_TypeDef* reg);
                    static void UARTInt(USART_TypeDef*, uint8_t);

                    static pegasus::hal::InterruptListener* extHandlers[15]; // 15 interrupt
                    static pegasus::hal::ServicesListener* serviceListener[pegasus::fc::service::SERVICE_SIZE];
                    static pegasus::hal::InterruptListener* timListener[64]; // TODO calc size
                    static pegasus::hal::ByteListener* uartListener[5];
            };

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */

#endif /* INTERRUPTREGISTER_H_ */
