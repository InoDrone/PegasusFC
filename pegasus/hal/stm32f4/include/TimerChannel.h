/*
 * TimerChannel.h
 *
 *  Created on: 24 nov. 2013
 *      Author: alienx
 */

#ifndef TIMERCHANNELSTM32F4_H_
#define TIMERCHANNELSTM32F4_H_


#include "hal/include/TimerBase.h"
#include "hal/stm32f4/include/Timer.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            class TimerChannel
            {
                public:
                    TimerChannel (Timer* timer, uint8_t channel);

                    void setValue(uint16_t value);
                    void setModeOC(uint16_t freqHz);
                    void setModeIC(uint16_t freqHz);
                    Timer* getTimer();
                    uint8_t getChannel();

                    uint8_t getUniqId();



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

            INLINE uint8_t TimerChannel::getChannel()
            {
                return _mChannel;
            }

            INLINE void TimerChannel::setModeOC(uint16_t freqHz)
            {
                _mTimer->disable();
                _mTimer->setFreq(freqHz);
                _mTimer->setMod(_mChannel, timer::OC);
                _mTimer->enable();
            }

            INLINE void TimerChannel::setModeIC(uint16_t freqHz)
            {
                _mTimer->disable();
                _mTimer->setFreq(freqHz);
                _mTimer->setMod(_mChannel, timer::IC);
                _mTimer->enable();
            }
        }
    }
}



#endif /* TIMERCHANNEL_H_ */
