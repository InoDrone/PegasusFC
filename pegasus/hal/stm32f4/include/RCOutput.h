/*
 * STM32RCOutput.h
 *
 *  Created on: 21 nov. 2013
 *      Author: alienx
 */

#ifndef STM32RCOUTPUT_H_
#define STM32RCOUTPUT_H_

#include "hal/stm32f4/include/Defs.h"
#include "hal/include/RCOutputBase.h"
#include "hal/stm32f4/include/Gpio.h"
#include "hal/stm32f4/include/TimerChannel.h"

#define MAX_RCOUTPUT 8

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            class RCOutput : public pegasus::hal::RCOutputBase<Gpio, TimerChannel>
            {
                public:
                  RCOutput(Gpio* pin, TimerChannel* timer, uint16_t freqHz);

                  void write(uint16_t value);
            };

        } /* namespace STM32 */
    } /* namespace arch */
} /* namespace Pegasus */

#endif /* STM32RCOUTPUT_H_ */
