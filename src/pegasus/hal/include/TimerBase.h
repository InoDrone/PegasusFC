/*
 * Timer.h
 *
 *  Created on: 31 oct. 2013
 *      Author: alienx
 */

#ifndef TIMERBASE_H_
#define TIMERBASE_H_

#include <stdint.h>

namespace pegasus {
    namespace hal {
          namespace timer {
              enum Mode {
                  IC,
                  OC
              };
          }

          template<class Reg_t>
          class TimerBase {
              public:
                  TimerBase(Reg_t* reg) :
                          _mReg(reg) {};

                  virtual void enable() = 0;
                  virtual void disable() = 0;
                  virtual void setFreq(uint32_t freqHz) = 0;
                  virtual void setMod(uint8_t channel, timer::Mode mode) = 0;
                  virtual void setValue(uint16_t channel, uint16_t value) = 0;

              protected:
                  Reg_t* _mReg;

          };
    }
}

#endif /* TIMERBASE_H_ */
