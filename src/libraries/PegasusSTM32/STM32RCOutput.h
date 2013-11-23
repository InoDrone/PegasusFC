/*
 * STM32RCOutput.h
 *
 *  Created on: 21 nov. 2013
 *      Author: alienx
 */

#ifndef STM32RCOUTPUT_H_
#define STM32RCOUTPUT_H_

#include "STM32Defs.h"
#include "STM32GPIO.h"
#include "RCOutput.h"

#define MAX_RCOUTPUT 8

namespace Pegasus {
    namespace arch {
        namespace STM32 {

            class STM32RCOutput : public Pegasus::RCOutput
            {
                public:
                  STM32RCOutput();

                  void add(const uint8_t pin, uint16_t freqHz);
                  void write(const uint8_t pin, uint16_t value);
            };

            extern STM32RCOutput rcout;

        } /* namespace STM32 */
    } /* namespace arch */
} /* namespace Pegasus */

#endif /* STM32RCOUTPUT_H_ */
