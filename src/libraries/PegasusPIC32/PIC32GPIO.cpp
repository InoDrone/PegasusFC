/*
 * PIC32GPIO.cpp
 *
 *  Created on: 29 oct. 2013
 *      Author: alienx
 */

#include "PIC32GPIO.h"

using namespace IDPegasusPIC32;

PIC32GPIO::PIC32GPIO() {}

void PIC32GPIO::pinMode(const char port, uint8_t bit, uint8_t mode)
{
	uint8_t pin = Port2Pin(port, bit);
	pinMode(pin, mode);

}

void PIC32GPIO::pinMode(uint8_t pin, uint8_t mode)
{
	if (pin > PIC32_GPIO_COUNT) {
		return;
	}

	switch(mode) {
		case INPUT:
			PINS[pin].port->lat.clr = (1 << PINS[pin].bit);
			PINS[pin].port->tris.set = (1 << PINS[pin].bit);
			break;
		case OPEN:
			PINS[pin].port->tris.clr = (1 << PINS[pin].bit);
			PINS[pin].port->odc.set = (1 << PINS[pin].bit);
			break;
		default:
			PINS[pin].port->tris.clr = (1 << PINS[pin].bit);
			PINS[pin].port->odc.clr = (1 << PINS[pin].bit);
			break;

	}
}

/**
 * read pin
 *
 * @param char 		port
 * @param uint8_t	bit
 *
 * @return uint8_t
 */
uint8_t PIC32GPIO::read(const char port, uint8_t bit)
{
	uint8_t pin = Port2Pin(port, bit);
	return read(pin);
}

/**
 * read pin
 *
 * @param uint8_t	port
 *
 * @return uint8_t
 */
uint8_t PIC32GPIO::read(uint8_t pin)
{
	if (pin > PIC32_GPIO_COUNT) {
		return 0;
	}


	return (PINS[pin].port->port.reg & PINS[pin].bit != 0 ? HIGH : LOW);
}

void PIC32GPIO::write(const char port, uint8_t bit, uint8_t value)
{
	uint8_t pin = Port2Pin(port, bit);
	write(pin, value);
}

void PIC32GPIO::write(uint8_t pin, uint8_t value)
{
	if (pin > PIC32_GPIO_COUNT) {
		return;
	}

	if (value == LOW) {
		PINS[pin].port->lat.clr = (1 << PINS[pin].bit);
	} else {
		PINS[pin].port->lat.set = (1 << PINS[pin].bit);
	}
}

/**
 *
 */
void PIC32GPIO::toogle(const char port, uint8_t bit)
{
	uint8_t pin = Port2Pin(port, bit);
	toogle(pin);
}

/**
 *
 */
void PIC32GPIO::toogle(uint8_t pin)
{
	if (pin > PIC32_GPIO_COUNT) {
		return;
	}

	PINS[pin].port->lat.inv = (1 << PINS[pin].bit);
}

const pic32_pin PINS[PIC32_GPIO_COUNT] = {
		//RA
		{(pic32_io *)&TRISA, 0}, // RA0
		{(pic32_io *)&TRISA, 1}, // RA1
		{(pic32_io *)&TRISA, 2}, // RA2
		{(pic32_io *)&TRISA, 3}, // RA3
		{(pic32_io *)&TRISA, 4}, // RA4
		{(pic32_io *)&TRISA, 5}, // RA5
		{(pic32_io *)&TRISA, 6}, // RA6
		{(pic32_io *)&TRISA, 7}, // RA7
		{0,0}, // RA8 -> not in tris
		{(pic32_io *)&TRISA, 9}, // RA9
		{(pic32_io *)&TRISA, 10}, // RA10
		{0,0}, // RA11
		{0,0}, // RA12
		{0,0}, // RA13
		{(pic32_io *)&TRISA, 14}, // RA14
		{(pic32_io *)&TRISA, 15}, // RA15

		// RB
		{(pic32_io *)&TRISB, 0}, // RB0
		{(pic32_io *)&TRISB, 1}, // RB1
		{(pic32_io *)&TRISB, 2}, // RB2
		{(pic32_io *)&TRISB, 3}, // RB3
		{(pic32_io *)&TRISB, 4}, // RB4
		{(pic32_io *)&TRISB, 5}, // RB5
		{(pic32_io *)&TRISB, 6}, // RB6
		{(pic32_io *)&TRISB, 7}, // RB7
		{(pic32_io *)&TRISB, 8}, // RB8
		{(pic32_io *)&TRISB, 9}, // RB9
		{(pic32_io *)&TRISB, 10}, // RB10
		{(pic32_io *)&TRISB, 11}, // RB11
		{(pic32_io *)&TRISB, 12}, // RB12
		{(pic32_io *)&TRISB, 13}, // RB13
		{(pic32_io *)&TRISB, 14}, // RB14
		{(pic32_io *)&TRISB, 15}, // RB15

		// RC
		{0,0}, // RC0
		{(pic32_io *)&TRISC, 1}, // RC1
		{(pic32_io *)&TRISC, 2}, // RC2
		{(pic32_io *)&TRISC, 3}, // RC3
		{(pic32_io *)&TRISC, 4}, // RC4
		{0,0}, // RC5
		{0,0}, // RC6
		{0,0}, // RC7
		{0,0}, // RC8
		{0,0}, // RC9
		{0,0}, // RC10
		{0,0}, // RC11
		{(pic32_io *)&TRISC, 12}, // RC12
		{(pic32_io *)&TRISC, 13}, // RC13
		{(pic32_io *)&TRISC, 14}, // RC14
		{(pic32_io *)&TRISC, 15}, // RC15

		// RD
		{(pic32_io *)&TRISD, 0}, // RD0
		{(pic32_io *)&TRISD, 1}, // RD1
		{(pic32_io *)&TRISD, 2}, // RD2
		{(pic32_io *)&TRISD, 3}, // RD3
		{(pic32_io *)&TRISD, 4}, // RD4
		{(pic32_io *)&TRISD, 5}, // RD5
		{(pic32_io *)&TRISD, 6}, // RD6
		{(pic32_io *)&TRISD, 7}, // RD7
		{(pic32_io *)&TRISD, 8}, // RD8
		{(pic32_io *)&TRISD, 9}, // RD9
		{(pic32_io *)&TRISD, 10}, // RD10
		{(pic32_io *)&TRISD, 11}, // RD11
		{(pic32_io *)&TRISD, 12}, // RD12
		{(pic32_io *)&TRISD, 13}, // RD13
		{(pic32_io *)&TRISD, 14}, // RD14
		{(pic32_io *)&TRISD, 15}, // RD15

		// RE
		{(pic32_io *)&TRISE, 0}, // RE0
		{(pic32_io *)&TRISE, 1}, // RE1
		{(pic32_io *)&TRISE, 2}, // RE2
		{(pic32_io *)&TRISE, 3}, // RE3
		{(pic32_io *)&TRISE, 4}, // RE4
		{(pic32_io *)&TRISE, 5}, // RE5
		{(pic32_io *)&TRISE, 6}, // RE6
		{(pic32_io *)&TRISE, 7}, // RE7
		{(pic32_io *)&TRISE, 8}, // RE8
		{(pic32_io *)&TRISE, 9}, // RE9
		{0,0}, // RE10
		{0,0}, // RE11
		{0,0}, // RE12
		{0,0}, // RE13
		{0,0}, // RE14
		{0,0}, // RE15

		// RF
		{(pic32_io *)&TRISF, 0}, // RF0
		{(pic32_io *)&TRISF, 1}, // RF1
		{(pic32_io *)&TRISF, 2}, // RF2
		{(pic32_io *)&TRISF, 3}, // RF3
		{(pic32_io *)&TRISF, 4}, // RF4
		{(pic32_io *)&TRISF, 5}, // RF5
		{0,0}, // RF6
		{0,0}, // RF7
		{(pic32_io *)&TRISF, 8}, // RF8
		{0,0}, // RF9
		{0,0}, // RF10
		{0,0}, // RF11
		{(pic32_io *)&TRISF, 12}, // RF12
		{(pic32_io *)&TRISF, 13}, // RF13
		{0,0}, // RF14
		{0,0}, // RF15


		// RG
		{(pic32_io *)&TRISG, 0}, // RG0
		{(pic32_io *)&TRISG, 1}, // RG1
		{(pic32_io *)&TRISG, 2}, // RG2
		{(pic32_io *)&TRISG, 3}, // RG3
		{0,0}, // RG4
		{0,0}, // RG5
		{(pic32_io *)&TRISG, 6}, // RG6
		{(pic32_io *)&TRISG, 7}, // RG7
		{(pic32_io *)&TRISG, 8}, // RG8
		{(pic32_io *)&TRISG, 9}, // RG9
		{0,0}, // RG10
		{0,0}, // RG11
		{(pic32_io *)&TRISG, 12}, // RG12
		{(pic32_io *)&TRISG, 13}, // RG13
		{(pic32_io *)&TRISG, 14}, // RG14
		{(pic32_io *)&TRISG, 15} // RG15
};

