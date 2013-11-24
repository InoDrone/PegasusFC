/*
 * Timer.h
 *
 *  Created on: 31 oct. 2013
 *      Author: alienx
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include "Defs.h"

namespace pegasus {

  class TimerBase {
      public:
              Timer() {}

              virtual void delayMillis(uint32_t ms) = 0;
              virtual void delayMicro(uint32_t us) = 0;
              virtual uint32_t millis() = 0;
              virtual uint32_t micros() = 0;


              virtual void attach(Func, uint32_t us) = 0;
              virtual void detach(Func) = 0;
  };

}

#endif /* TIMER_H_ */
