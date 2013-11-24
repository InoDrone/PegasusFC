/*
 * PIC32Timer.h
 *
 *  Created on: 31 oct. 2013
 *      Author: alienx
 */

#ifndef PIC32TIMER_H_
#define PIC32TIMER_H_

#include "PegasusPIC32.h"

#define TOGGLES_PER_SEC 1000
#define CORE_TICK_RATE	(SYS_FREQ / 2 / TOGGLES_PER_SEC)
#define CORE_TICKS_PER_MICROSECOND		(F_CPU / 2 / 1000000UL)

class IDPegasusPIC32::PIC32Timer : public IDPegasus::Timer {
public:
	PIC32Timer();

	void delayMillis(uint32_t ms);
	void delayMicro(uint32_t us);
	uint32_t millis();
	uint32_t micros();


	void attach(IDPegasus::Func, uint32_t us);
	void detach(IDPegasus::Func);

	static volatile uint32_t coreTimerCount;
	static volatile uint32_t coreTimerLastValue;
};

#endif /* PIC32TIMER_H_ */
