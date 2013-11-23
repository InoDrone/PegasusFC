/*
 * PIC32GPIO.h
 *
 *  Created on: 29 oct. 2013
 *      Author: alienx
 */

#ifndef PIC32GPIO_H_
#define PIC32GPIO_H_

#include "PegasusPIC32.h"

#define Port2Pin(port, bit) ((port-'A')*16+bit)
#define PIC32_GPIO_COUNT 112

class IDPegasusPIC32::PIC32GPIO : public IDPegasus::GPIO {
public:
	PIC32GPIO();
	void pinMode(const char port, uint8_t bit, uint8_t output);
	uint8_t read(const char port, uint8_t bit);
	void write(const char port, uint8_t bit, uint8_t value);
	void toogle(const char port, uint8_t bit);

	void pinMode(uint8_t pin, uint8_t output);
	uint8_t read(uint8_t pin);
	void write(uint8_t pin, uint8_t value);
	void toogle(uint8_t pin);
};


typedef struct {
	volatile uint32_t       reg;
	volatile uint32_t       clr;
	volatile uint32_t       set;
	volatile uint32_t       inv;
} p32_regset;

typedef struct {
	volatile p32_regset 	tris;
	volatile p32_regset 	port;
	volatile p32_regset 	lat;
	volatile p32_regset 	odc;
} pic32_io;

typedef struct {
	volatile pic32_io* port;
	uint8_t bit;
} pic32_pin;

extern const pic32_pin PINS[];

#endif /* PIC32GPIO_H_ */
