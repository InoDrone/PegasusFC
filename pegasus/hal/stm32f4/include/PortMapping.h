/*
 * PortMapping.h
 *
 *  Created on: 24 nov. 2013
 *      Author: alienx
 */

#ifndef PORTMAPPING_H_
#define PORTMAPPING_H_

#define PORT_MAPPING_MAX_PINS 20

#include "hal/stm32f4/include/Defs.h"
#include "hal/stm32f4/include/Gpio.h"
#include "hal/stm32f4/include/RCOutput.h"
#include "hal/stm32f4/include/PWMInput.h"
#include "hal/stm32f4/include/Timer.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            class PortMapping {
                public:
                    struct PinInfo {
                            gpio::portNumber_t  port;
                            gpio::bitNumber_t   bit;
                            gpio::AlternateFunction AF;
                            Timer*              timer;
                            uint8_t             channel;
                            uint16_t            freqHz;
                    };

                    void add(uint8_t index);
                    RCOutput* getRCOutput(uint8_t index);
                    PWMInput* getPWMInput(uint8_t index);
                    /*Gpio* getGpio(uint8_t index);
                    Timer* getTimer(uint8_t index);
                    TimerChannel getChannelTimer(uint8_t index);*/

                private:

                    struct Pin {
                            Gpio* io;
                            TimerChannel* timerChannel;
                            RCOutput* out;
                            PWMInput* in;
                            PinInfo pinInfo;
                    };

                    Pin _mPins[PORT_MAPPING_MAX_PINS];
            };
        }

        extern const stm32f4::PortMapping::PinInfo PORTMAP[PORT_MAPPING_MAX_PINS];

        //using PortMapping = pegasus::hal::stm32f4::PortMapping;
    }
}



#endif /* PORTMAPPING_H_ */
