/*
 * TimerChannel.h
 *
 *  Created on: 24 nov. 2013
 *      Author: alienx
 */

#ifndef TIMERCHANNEL_H_
#define TIMERCHANNEL_H_


#include "pegasus/hal/include/TimerBase.h"
#include "pegasus/hal/stm32f4/include/Timer.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            class TimerChannel
            {
                public:
                    TimerChannel (Timer* timer, uint8_t channel);

                    void setValue(uint16_t value);
                    void setToPwm(uint16_t freqHz);
                    Timer* getTimer();

                private:
                    Timer* _mTimer;
                    uint8_t _mChannel;
            };

            inline TimerChannel::TimerChannel(Timer* timer, uint8_t channel) :
                    _mTimer(timer),
                    _mChannel(channel) {

            }

            INLINE void TimerChannel::setValue(uint16_t value) {
                _mTimer->setValue(_mChannel, value);
            }

            INLINE Timer* TimerChannel::getTimer()
            {
                return _mTimer;
            }

            INLINE void TimerChannel::setToPwm(uint16_t freqHz)
            {
                _mTimer->setMod(_mChannel, timer::Mode::OC);
                _mTimer->setFreq(freqHz);
            }
        }
    }
}



#endif /* TIMERCHANNEL_H_ */
