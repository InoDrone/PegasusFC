/*
 * PegasusPIC32.h
 *
 *  Created on: 27 oct. 2013
 *      Author: alienx
 */

#ifndef PEGASUSPIC32_H_
#define PEGASUSPIC32_H_

#include <p32xxxx.h>
#include <plib.h>
#include <sys/attribs.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <Pegasus.h>
#include "PegasusPIC32Namespace.h"

#include "PIC32GPIO.h"
#include "PIC32Timer.h"

#define SYS_FREQ 					(F_CPU)
#define	getSystemClock() 			(SYS_FREQ)
#define	getPeripheralClock()		(getSystemClock()/(1 >> OSCCONbits.PBDIV))


class PegasusPIC32 : public IDPegasus::Pegasus {
public:
	PegasusPIC32();
};

//extern const PegasusPIC32 pegasusPIC32;

#endif /* PEGASUSPIC32_H_ */
