/*
 * STM32.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef STM32_H_
#define STM32_H_

#include "hal/stm32f4/include/Defs.h"
#include "hal/stm32f4/include/Processor.h"
#include "hal/stm32f4/include/ThreadContext.h"
#include "hal/stm32f4/include/InterruptRegister.h"
#include "hal/stm32f4/include/Gpio.h"
#include "hal/stm32f4/include/CoreTimer.h"
#include "hal/stm32f4/include/Core.h"
#include "hal/stm32f4/include/Timer.h"
#include "hal/stm32f4/include/TimerChannel.h"
#include "hal/stm32f4/include/PWMInput.h"
#include "hal/stm32f4/include/RCOutput.h"
#include "hal/stm32f4/include/PortMapping.h"
#include "hal/stm32f4/include/SpiDriver.h"
#include "hal/stm32f4/include/SpiDevice.h"
#include "hal/stm32f4/include/UARTDriver.h"

namespace pegasus {
    namespace hal {
        typedef pegasus::hal::stm32f4::Gpio         Gpio;
        typedef pegasus::hal::stm32f4::CoreTimer    CoreTimer;
        typedef pegasus::hal::stm32f4::Core         ArchCore;
        typedef pegasus::hal::stm32f4::Timer        Timer;
        typedef pegasus::hal::stm32f4::TimerChannel TimerChannel;
        typedef pegasus::hal::stm32f4::RCOutput     RCOutput;
        typedef pegasus::hal::stm32f4::PWMInput     PWMInput;
        typedef pegasus::hal::stm32f4::PortMapping  PortMapping;

        typedef pegasus::hal::stm32f4::SpiDevice  SpiDevice;
        typedef pegasus::hal::stm32f4::SpiDriver  SpiDriver;

        typedef pegasus::hal::stm32f4::UARTDriver  UARTDriver;

        typedef pegasus::hal::stm32f4::InterruptRegister InterruptRegister;


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
