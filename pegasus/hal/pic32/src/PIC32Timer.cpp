/*
 * PIC32Timer.cpp
 *
 *  Created on: 31 oct. 2013
 *      Author: alienx
 */

#include "PIC32Timer.h"

using namespace IDPegasusPIC32;

volatile uint32_t PIC32Timer::coreTimerCount = 0;
volatile uint32_t PIC32Timer::coreTimerLastValue = 0;


PIC32Timer::PIC32Timer() {
	// Init Core Timer
	OpenCoreTimer(CORE_TICK_RATE);
	mConfigIntCoreTimer(CT_INT_ON | CT_INT_PRIOR_7 | CT_INT_SUB_PRIOR_0);
}

void PIC32Timer::delayMillis(uint32_t ms)
{
	unsigned long start = coreTimerCount;
	while( (coreTimerCount - start) < ms);
}

void PIC32Timer::delayMicro(uint32_t us)
{
	unsigned long start = micros();
	while( (micros() - start) < us);
}

uint32_t PIC32Timer::millis()
{
	return coreTimerCount;
}

uint32_t PIC32Timer::micros()
{
	uint32_t result;
	uint32_t currentTimerVal;
	uint32_t status;

	status = INTDisableInterrupts();
	result = coreTimerCount + 1000;
	currentTimerVal = ReadCoreTimer();
	currentTimerVal -= coreTimerLastValue;
	currentTimerVal /= CORE_TICKS_PER_MICROSECOND;
	INTRestoreInterrupts(status);


	return (result + currentTimerVal);
}


void PIC32Timer::attach(IDPegasus::Func, uint32_t us)
{

}

void PIC32Timer::detach(IDPegasus::Func)
{

}

extern "C" {
	void __ISR(_CORE_TIMER_VECTOR, ipl7) CoreTimerHandler(void)
	{
		PIC32Timer::coreTimerCount++;
		PIC32Timer::coreTimerLastValue = ReadCoreTimer();
		UpdateCoreTimer(CORE_TICK_RATE);

		mCTClearIntFlag();
	}
}

