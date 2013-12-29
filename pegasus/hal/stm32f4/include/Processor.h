/*
 * Processor.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_
#include "hal/stm32f4/include/Defs.h"

/*
 * AMR CORTEX M-4 Core register (see: http://infocenter.arm.com/help/topic/com.arm.doc.dui0553a/CHDBIBGJ.html)
 * -------------------------------------------
 * R0-R12       |       General registers
 * MSP          |       Main Stack Pointer (On reset load 0x00000000)
 * PSP          |       Process Stack Pointer
 * LR           |       Link register
 * PC           |       Program Counter
 * PSR          |       Program Status Register
 * ASPR         |       Application Program Status Register
 * ISPR         |       Interrupt Program Status Register
 * EPSR         |       Execution Program Status Register
 * PRIMASK      |       Priority Mask Register
 * FAULTMASK    |       Fault Mask Register
 * BASEPRI      |       Base Priority Mask Register
 * CONTROL      |       CONTROL Register
 *
 *
 * MSP / PSP
 * ---------------------------------------------------
 * Thread mode bit[1] CONTROL = 1 (Switch to PSP Stack)
 * Normal mode bit[1] CONTROL = 0 (Switch to MSP Stack)
 */

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            class Processor {
                public:

                    static void enableInterrupts(void);
                    static void disableInterrupts(void);

                    /* STACK SELECTOR */
                    static void toMSP(void);
                    static void toPSP(void);

                    /* STACK VALUES */
                    static uint32_t getPSP(void);
                    static void setPSP(uint32_t);

                    static uint32_t getMSP(void);
                    static void setMSP(uint32_t);


                    /* REGISTERS */
                    static uint32_t getCONTROL(void);
                    static void setCONTROL(uint32_t);
            };

            INLINE void Processor::enableInterrupts(void)
            {
                asm volatile
                (
                    " CPSIE i       \n"

                    : // output
                    : // input
                    : // clobber
                );
            }

            INLINE void Processor::disableInterrupts(void)
            {
                asm volatile
                (
                    " CPSID i       \n"

                    : // output
                    : // input
                    : // clobber
                );
            }

            INLINE void Processor::toMSP(void)
            {
                setCONTROL(getCONTROL() & ~(0x02));
            }

            INLINE void Processor::toPSP(void)
            {
                setCONTROL(0x02);
            }

            INLINE void Processor::setCONTROL(uint32_t value)
            {
                asm volatile
                (
                    "msr control, %0       \n"

                    : // output
                    : "r" (value) // input
                    : // clobber
                );
            }

            INLINE uint32_t Processor::getCONTROL()
            {
                uint32_t value;
                asm volatile
                (
                    "mrs %0, control"

                    : "=r" (value) // output
                    : // input
                    : // clobber
                );

                return value;
            }

            INLINE void Processor::setPSP(uint32_t value)
            {
                asm volatile
                (
                    "msr psp, %0       \n"

                    : // output
                    : "r" (value) // input
                    : // clobber
                );
            }

            INLINE uint32_t Processor::getPSP()
            {
                uint32_t value;
                asm volatile
                (
                    "mrs %0, psp"

                    : "=r" (value) // output
                    : // input
                    : // clobber
                );

                return value;
            }

            INLINE void Processor::setMSP(uint32_t value)
            {
                asm volatile
                (
                    "msr msp, %0       \n"

                    : // output
                    : "r" (value) // input
                    : // clobber
                );
            }

            INLINE uint32_t Processor::getMSP()
            {
                uint32_t value;
                asm volatile
                (
                    "mrs %0, msp"

                    : "=r" (value) // output
                    : // input
                    : // clobber
                );

                return value;
            }


        }

        typedef pegasus::hal::stm32f4::Processor Processor;
    }
}



#endif /* PROCESSOR_H_ */
