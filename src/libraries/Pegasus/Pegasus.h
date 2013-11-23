/*
 * Pegasus.h
 *
 *  Created on: 25 oct. 2013
 *      Author: alienx
 */

#ifndef PEGASUS_H_
#define PEGASUS_H_

#include <stdint.h>

#include "Defs.h"
#include "GPIO.h"
#include "Timer.h"
#include "RCOutput.h"
#include "Mixing.h"
#include "Trace.h"

/*namespace Pegasus {
    namespace core {
        class Mixing;
        class Trace;

        extern Mixing mix;
        extern Trace trace;
    }
}*/

extern "C" {
	void setup();
	void loop();
}


#if defined PEGASUS_PIC32
#include "PegasusPIC32.h"
#elif defined PEGASUS_STM32
#include "STM32.h"
#endif

#endif /* PEGASUS_H_ */
