/*
 * Timer.h
 *
 *  Created on: 31 oct. 2013
 *      Author: alienx
 */

#ifndef TIMERBASE_H_
#define TIMERBASE_H_

#include <stdint.h>
#include "core/include/Defs.h"

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

                  virtual uint8_t getUniqId() = 0;

                  Reg_t* getReg();

              protected:
                  Reg_t* _mReg;

          };

          template<class Reg_t>
          inline Reg_t* TimerBase<Reg_t>::getReg() {
              return _mReg;
          }
    }
}

#endif /* TIMERBASE_H_ */
