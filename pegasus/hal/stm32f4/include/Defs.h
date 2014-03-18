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

#include <core/include/Defs.h>

#ifndef F_CPU
#define F_CPU 168000000 // 180 Mhz
#endif

#define _BIT(x) (1 << (x))

#define CYCLES_PER_MICROSECOND (F_CPU/1000000)
#define SYSTICK_RELOAD_VAL     ((F_CPU/1000) - 1) // for 1ms

//#define BT_MODE

typedef uint32_t u32;

/**
 * Interrupt priority
 */
#define SYSTICK_INTERRUPT_PRIORITY 0xFF
#define PENDSV_INTERRUPT_PRIORITY  0xFF

//TODO move to HAL
#define STACK_SIZE 1024

namespace pegasus {
    namespace hal {
        struct Stack_t {
                uint32_t *top;
                uint32_t  stack[STACK_SIZE];// / sizeof(uint32_t)];
        };
    }
}

#endif /* STM32DEFS_H_ */
