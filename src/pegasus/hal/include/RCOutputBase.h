/*
 * RCOutput.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef RCOUTPUTBASE_H_
#define RCOUTPUTBASE_H_

/*#include "pegasus/hal/include/GpioBase.h"*/

namespace pegasus {

  class RCOutputBase {
      public:
          RCOutputBase() {};

          virtual void write(const uint8_t pin, uint16_t value) = 0;
  };

}


#endif /* RCOUTPUTBASE_H_ */
