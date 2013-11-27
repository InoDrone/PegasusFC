/*
 * ThreadContext.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#include "pegasus/hal/stm32f4/include/ThreadContext.h"
#include "pegasus/core/include/ThreadManager.h"
#include "pegasus/core/include/Thread.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4  {

            uint32_t** ThreadContext::_sStack;

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

                *--stk = 0x10000000L;                // xPSR with 'T' set for interrupt (aligned 8)
                *--stk = (uint32_t) entryPoint | 1;      // Entry Point Adresse
                *--stk = (uint32_t) &Core::yield | 1;      // Return address (null) (LR) R14
                *--stk = 0x0L;                // R12
                *--stk = 0x0L;                // R3
                *--stk = 0x0L;                // R2
                *--stk = (uint32_t) thread;          // R1 //(second function argument)
                *--stk = (uint32_t) func;            // R0 //(first function argument)
                *--stk = 0x11111111L;                // R11
                *--stk = 0x10101010L;                // R10
                *--stk = 0x09090909L;                // R09
                *--stk = 0x08080808L;                // R08
                *--stk = 0x07070707L;                // R07 (fucking R7 !!!)
                *--stk = 0x06060606L;                // R06
                *--stk = 0x05050505L;                // R05
                *--stk = 0x04040404L;                // R04 <- Stack pointer


                _mStack = stk;
            }

            void ThreadContext::callSwitch()
            {
                (*((volatile unsigned long *) 0xE000ED04)) = (1 << 0x1B); // Clear pendSV bit

                //pegasus::core::threadManager.contextSwitchInterrupt();

                _sStack = pegasus::core::threadManager.getCurrent()->getContext().getPStack();
            }

            namespace InterruptHandler {
                void PendService(void)
                {
                    asm volatile
                    (
                        // Registers R0-R3 are free to use. LR = R14.
                        "       mrs r0, psp                         \n"// Get the main stack in R0
                        "                                           \n"
                        "       ldr     r3, %[stack]                \n"// Get the address of ms_ppStack
                        "       ldr     r2, [r3]                    \n"
                        "                                           \n"
            #if 0
                        "       tst r14, #0x10                      \n" // Is the task using the FPU context?
                        "       it eq                               \n"
                        "       vstmdbeq r0!, {s16-s31}             \n"// If so, push high vfp registers.
            #endif
                        "                                           \n"
                        "       stmdb r0!, {r4-r11}                 \n" // Save the core registers.
                        "                                           \n"
                        "       str r0, [r2]                        \n"// Save the new top of stack into the first member of the TCB.
                        "                                           \n"
                        "       stmdb sp!, {r3, lr}                 \n"
                        "                                           \n"
                        "       mov r0, #128                        \n"// Disable interrupts allowed to make system calls
                        "       msr basepri, r0                     \n"
                        "                                           \n"
                        "       bl %[csw]                           \n"// bl contextSwitch
                        "                                           \n"
                        "       mov r0, #0                          \n"// Setting a value of 0 will cancel masking completely,
                        "       msr basepri, r0                     \n"// enabling all interrupts
                        "                                           \n"
                        "       ldmia sp!, {r3, lr}                 \n"
                        "                                           \n"
                        "       ldr r0, [r3]                        \n"// At *ms_ppStack is the new stack pointer.
                        //"       ldr r0, [r1]                        \n"
                        "                                           \n"
                        "       ldmia r0!, {r4-r11}                 \n"// Pop the core registers.
                        "                                           \n"
            #if 0
                        "       tst r14, #0x10                      \n" // Is the task using the FPU context?
                        "       it eq                               \n"
                        "       vldmiaeq r0!, {s16-s31}             \n"// If so, pop the high vfp registers too.
            #endif
                        "                                           \n"
                        "       msr psp, r0                         \n" // Restore the main stack register
                        "       blx lr                              \n"
                        // Branch to LR (R14) to return from exception
                        /*"                                           \n"
                        "       .align 2                            \n"
                        "sStack: .word _ZN7pegasus3hal7stm32f413ThreadContext7_sStackE       \n"*/
                        : /* out */
                        : /* in */
                          [csw]  "i"(&pegasus::hal::stm32f4::ThreadContext::callSwitch),
                          [stack] "m"(pegasus::hal::stm32f4::ThreadContext::_sStack)
                        : "memory", "cc"
                    );

                    //__asm__ __volatile__ ("ldr pc, =0xFFFFFFFD");

                    /*asm volatile
                    (
                            //"mrs r0, psp            \n" // Store PSP to r0
                            "ldr r3, %[stack]       \n" // Get address of Current Stack
                            //"ldr r2, [r3]           \n" // Load r3 address in r2
                            "                       \n"
                            //"stmdb r0!, {r4-r11}    \n" //Save core register
                            //"str r0, [r2]           \n" //Save top stack
                            //"stmdb sp!, {r3, lr}    \n"
                            //"                       \n"
                            "mov r0, #128           \n"
                            "msr basepri, r0        \n"
                            "                       \n"
                            "bl %[cswh]             \n" // Call context switch
                            "                       \n"
                            "mov r0, #0             \n"
                            "msr basepri, r0        \n"
                            "                       \n"
                            "ldmia sp!, {r3, lr}    \n"
                            "                       \n"
                            "ldr r1, [r3]           \n" // New stack pointer
                            "ldr r0, [r1]           \n" //
                            "                       \n"
                            "ldmia r0!, {r4-r11}    \n" // Pop core registers.
                            "                       \n"
                            "msr psp, r0            \n" // Restore stack register
                            "                       \n"
                            "bx lr                  \n" // Call return (LR)
                            : // return
                            : // args
                              [cswh]  "i"(&pegasus::hal::stm32f4::ThreadContext::callSwitch),
                              [stack] "m"(pegasus::hal::stm32f4::ThreadContext::_sStack)
                            : "memory", "cc"
                    );*/
                }
            }
        }
    }
}

