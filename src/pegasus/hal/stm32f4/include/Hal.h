/*
 * STM32.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef STM32_H_
#define STM32_H_

#include "pegasus/hal/stm32f4/include/Gpio.h"
#include "pegasus/hal/stm32f4/include/RCOutput.h"
#include "pegasus/hal/stm32f4/include/CoreTimer.h"
#include "pegasus/hal/stm32f4/include/Core.h"

namespace pegasus {
    typedef pegasus::hal::stm32f4::Gpio         Gpio;
    typedef pegasus::hal::stm32f4::CoreTimer    CoreTimer;
    typedef pegasus::hal::stm32f4::Core         ArchCore;
}

#endif /* STM32_H_ */
