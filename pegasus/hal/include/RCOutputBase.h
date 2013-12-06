/*
 * RCOutput.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef RCOUTPUTBASE_H_
#define RCOUTPUTBASE_H_


namespace pegasus {
    namespace hal {
          template<class GpioBase_t, class TimerChannel_t>
          class RCOutputBase {
              public:
                  RCOutputBase(GpioBase_t* pin, TimerChannel_t* timer, uint16_t freqHz) :
                      _mPin(pin),
                      _mTimerChannel(timer),
                      _mFreqHz(freqHz)
                      {}
                  virtual void write(uint16_t value) = 0;
              protected:
                  GpioBase_t*           _mPin;
                  TimerChannel_t*       _mTimerChannel;
                  uint16_t              _mFreqHz;
          };
    }

}


#endif /* RCOUTPUTBASE_H_ */
