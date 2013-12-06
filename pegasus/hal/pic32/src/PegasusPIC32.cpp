/*
 * PegasusPIC32.cpp
 *
 *  Created on: 27 oct. 2013
 *      Author: alienx
 */

#include "PegasusPIC32.h"

using namespace IDPegasusPIC32;

static PIC32GPIO _gpio;
static PIC32Timer _timer;

PegasusPIC32::PegasusPIC32() :
	IDPegasus::Pegasus(&_gpio, &_timer)
	{}


void init()
{
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	INTEnableSystemMultiVectoredInt();
	INTEnableInterrupts();

}

int main()
{
	init();
	setup();

	while(1) { loop(); }

	return 0;
}



//const PegasusPIC32 pegasusPIC32;


