/*
 * ThreadContext.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "hal/stm32f4/include/ThreadContext.h"
#include "core/include/ThreadManager.h"
#include "core/include/Thread.h"

uint32_t** _sStack;

namespace pegasus {
    namespace hal {
        namespace stm32f4  {

            //uint32_t** ThreadContext::_sStack;

            ThreadContext::ThreadContext() {}
            ThreadContext::~ThreadContext() {}

            void ThreadContext::init(pegasus::hal::Stack_t* stack, pegasus::core::ThreadEntry_t entryPoint,
                    void* func, void* thread)
            {
                uint32_t* stk = stack->top;
                *--stk = 0x12345678L;                // Use for debug

                // Stack align(8)

                while ( ((uint32_t)stk & 7) != 0) {
                    *--stk = 0x12345678L;            // value used for debug
                }

                *--stk = 0x01000000L;                   // xPSR with 'T' set for interrupt (aligned 8)
                *--stk = (uint32_t) entryPoint;         // Entry Point Adresse
                *--stk = 0x0;                           // Return address (null) (LR) R14
                *--stk = 0x0L;                          // R12
                *--stk = 0x0L;                          // R3
                *--stk = 0x0L;                          // R2
                *--stk = (uint32_t) thread;             // R1 //(second function argument)
                *--stk = (uint32_t) func;               // R0 //(first function argument)

                *--stk = 0xFFFFFFFD;                    // LR

                *--stk = 0x0;                // R11
                *--stk = 0x0;                // R10
                *--stk = 0x0;                // R09
                *--stk = 0x0;                // R08
                *--stk = 0x0;                // R07
                *--stk = 0x0;                // R06
                *--stk = 0x0;                // R05
                *--stk = 0x0;                // R04 <- Stack pointer


                _mStack = stk;
            }

            void ThreadContext::callSwitch()
            {
                (*((volatile unsigned long *) 0xE000ED04)) = (1 << 0x1B); // Clear pendSV bit

                pegasus::core::threadManager.contextSwitchInterrupt();

                _sStack = pegasus::core::threadManager.getCurrent()->getContext().getPStack();
            }

            namespace InterruptHandler {
                void PendService(void)
                {
//                    (*((volatile unsigned long *) 0xE000ED04)) = (1 << 0x1B); // Clear pendSV bit

                    if (!_sStack) {
                        _sStack = pegasus::core::threadManager.getCurrent()->getContext().getPStack();

                        asm(
                                "ldr r2, %[pps]                      \n"
                                "ldr r0, [r2]                        \n"
                                "ldmia r0!, {r4-r11, lr}            \n"// Pop the core registers.
                                "                                    \n"
                                "MSR PSP, r0                         \n"
                                "orr lr,lr, #4                       \n"
                                "bx lr                               \n"
                                : /* out */
                                : /* in */
                                  [pps] "m"(_sStack)
                                : "memory", "cc"
                        );


                    }

                    asm volatile (
                            // Registers R0-R3 are free to use. LR = R14.
                            "       mrs     r12, psp                    \n"// Get the main stack in R12
                            "       stmdb   r12!, {r4-r11, lr}          \n"// push other register
                            "                                           \n"
                            "       ldr     r3, ppStack                 \n"// Get the address of sStack
                            "       ldr     r2, [r3]                    \n"
                            "       str     r12,[r2]                    \n"
                            "                                           \n"
                            "       stmdb sp!, {r3, lr}                 \n" // Save EXC_RETURN LR
                            "                                           \n"
                            "                                           \n"
                            "       bl %[csw]                           \n"// bl contextSwitch
                            "                                           \n"
                            "                                           \n"
                            "       ldmia sp!, {r3, lr}                 \n" // Restore EXC_RETURN LR
                            "                                           \n"
                            "       ldr r1,  [r3]                       \n"// At *ms_ppStack is the new stack pointer.
                            "       ldr r0,  [r1]                       \n"// At *ms_ppStack is the new stack pointer.
                            "                                           \n"
                            "       ldmia r0!, {r4-r11, LR}             \n"// Pop the core registers.
                            "                                           \n"
                            "                                           \n"
                            "       msr psp, r0                         \n" // Restore the main stack register
                            "       ORR lr, lr, #4                      \n"
                            "                                           \n"
                            "       bx lr                               \n"// Branch to LR (R14) to return from exception
                            "       .align 2                            \n"
                            "ppStack: .word _sStack                           \n"// .word _ZN7pegasus3hal7stm32f413ThreadContext7_sStackE                \n"
                            : /* out */
                            : /* in */
                            [csw] "i"(&pegasus::hal::stm32f4::ThreadContext::callSwitch)
                            : "memory", "cc"
                    );
                }
            }
        }
    }
}

