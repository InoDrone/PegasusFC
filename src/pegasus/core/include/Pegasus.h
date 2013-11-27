/*
 * Pegasus.h
 *
 *  Created on: 25 oct. 2013
 *      Author: alienx
 */

#ifndef PEGASUS_H_
#define PEGASUS_H_

#include <stdint.h>

#include "pegasus/core/include/Defs.h"


extern "C" {
	void setup();
	void loop();
}


#if defined PEGASUS_PIC32
#include "PegasusPIC32.h"
#elif defined PEGASUS_STM32
#include "pegasus/hal/stm32f4/include/Hal.h"
#endif


#endif /* PEGASUS_H_ */
