/*
 * STM32.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef STM32_H_
#define STM32_H_

#include "pegasus/hal/stm32f4/include/Processor.h"
#include "pegasus/hal/stm32f4/include/ThreadContext.h"
#include "pegasus/hal/stm32f4/include/Gpio.h"
#include "pegasus/hal/stm32f4/include/CoreTimer.h"
#include "pegasus/hal/stm32f4/include/Core.h"
#include "pegasus/hal/stm32f4/include/Timer.h"
#include "pegasus/hal/stm32f4/include/TimerChannel.h"
#include "pegasus/hal/stm32f4/include/RCOutput.h"
#include "pegasus/hal/stm32f4/include/PortMapping.h"

namespace pegasus {
    namespace hal {
        typedef pegasus::hal::stm32f4::Gpio         Gpio;
        typedef pegasus::hal::stm32f4::CoreTimer    CoreTimer;
        typedef pegasus::hal::stm32f4::Core         ArchCore;
        typedef pegasus::hal::stm32f4::Timer        Timer;
        typedef pegasus::hal::stm32f4::TimerChannel TimerChannel;
        typedef pegasus::hal::stm32f4::RCOutput     RCOutput;
        typedef pegasus::hal::stm32f4::PortMapping  PortMapping;


        extern PortMapping portMapping;

        static INLINE PortMapping::PinInfo PORTMAPPING (char port, pegasus::gpio::bitNumber_t bit, TIM_TypeDef*  timer,uint8_t channel, uint16_t freqHz) {
            PortMapping::PinInfo pin;
            gpio::portNumber_t portNumber = (port-'A');

            pin.port = portNumber;
            pin.bit = bit;
            pin.timer = timer;
            pin.channel = channel;
            pin.freqHz = freqHz;

            return pin;
        }
    }
}

#endif /* STM32_H_ */
