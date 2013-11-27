/*
 * GPIO.h
 *
 *  Created on: 29 oct. 2013
 *      Author: alienx
 */

#ifndef GPIOBASE_H_
#define GPIOBASE_H_

#include <stdint.h>

#define LOW 	0
#define HIGH 	1

namespace pegasus {

    namespace gpio {
        typedef uint32_t bitNumber_t;
        typedef uint32_t portNumber_t;

        enum class Mode : uint8_t {
            OUTPUT                = 0,
            OUTPUT_OD             = 1,
            INPUT                 = 2,
            INPUT_FLOATING        = 3,
            INPUT_ANALOG          = 4,
            INPUT_PU              = 5,
            INPUT_PD              = 6,
            PWM                   = 7
        };
    }

    namespace hal {

          namespace gpio = pegasus::gpio;

          class GpioBase {
              public:
                  GpioBase(const gpio::portNumber_t portNumber, const gpio::bitNumber_t bitNumber) :
                      _mBitMask(1 << bitNumber),
                      _mPortNumber(portNumber),
                      _mBitNumber(bitNumber)
                  {

                  };

                  virtual void mode(const pegasus::gpio::Mode mode) = 0;
                  virtual void low(void) = 0;
                  virtual void high(void) = 0;
                  virtual void toggle(void) = 0;
                  virtual bool read(void) = 0;

              protected:
                  const uint16_t _mBitMask;
                  const pegasus::gpio::portNumber_t _mPortNumber;
                  const pegasus::gpio::bitNumber_t _mBitNumber;
          };
    }

}


#endif /* GPIOBASE_H_ */
