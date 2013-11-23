/*
 * RCOutput.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef RCOUTPUT_H_
#define RCOUTPUT_H_


namespace Pegasus {

  class RCOutput {
      public:
          RCOutput() {}

          virtual void add(const uint8_t pin, uint16_t freqHz) = 0;
          virtual void write(const uint8_t pin, uint16_t value) = 0;
  };

}


#endif /* RCOUTPUT_H_ */
