/*
 * GPIO.h
 *
 *  Created on: 29 oct. 2013
 *      Author: alienx
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#define OUTPUT 			0
#define OUTPUT_OD		1
#define INPUT  			2
#define INPUT_FLOATING 	3
#define INPUT_ANALOG	4
#define INPUT_PU		5
#define INPUT_PD		6
#define PWM                     7

#define LOW 	0
#define HIGH 	1

namespace Pegasus {

  class GPIO {
  public:
          GPIO() {}
          ~GPIO() {};

          virtual void pinMode(const char port, uint8_t bit, uint8_t output) =0;
          virtual uint8_t read(const char port, uint8_t bit) = 0;
          virtual void write(const char port, uint8_t bit, uint8_t value) = 0;
          virtual void toogle(const char port, uint8_t bit) = 0;

          virtual void pinMode(uint8_t pin, uint8_t output) =0;
          virtual uint8_t read(uint8_t pin) = 0;
          virtual void write(uint8_t pin, uint8_t value) = 0;
          virtual void toogle(uint8_t pin) = 0;
  };

}


#endif /* GPIO_H_ */
