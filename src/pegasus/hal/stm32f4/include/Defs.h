/*
 * STM32Defs.h
 *
 *  Created on: 22 nov. 2013
 *      Author: alienx
 */

#ifndef STM32DEFS_H_
#define STM32DEFS_H_

#include <stm32f4xx.h>
#include <stdint.h>
#include <stdlib.h>

#include <pegasus/core/include/Defs.h>

#ifndef F_CPU
#define F_CPU 180000000 // 180 Mhz
#endif

#define _BIT(x) (1 << (x))

#define CYCLES_PER_MICROSECOND (F_CPU/1000000)
#define SYSTICK_RELOAD_VAL     ((F_CPU/1000) - 1) // for 1ms

#define INLINE inline __attribute__((always_inline))
typedef uint32_t u32;



#endif /* STM32DEFS_H_ */
