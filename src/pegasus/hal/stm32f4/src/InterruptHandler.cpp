/*
 * STM32InterruptHandler.cpp
 *
 *  Created on: 7 nov. 2013
 *      Author: alienx
 */

#include "pegasus/hal/stm32f4/include/InterruptHandler.h"

namespace pegasus {
    namespace hal {
          namespace stm32f4 {
                namespace InterruptHandler {
                  void NMI(void) {
                    while (true) {}
                  }

                  void HardFault(void) {

                      __asm volatile
                      (
                          " tst lr, #4                                                \n"
                          " ite eq                                                    \n"
                          " mrseq r0, msp                                             \n"
                          " mrsne r0, psp                                             \n"
                          " ldr r1, [r0, #24]                                         \n"
                          " ldr r2, handler2_address_const                            \n"
                          " bx r2                                                     \n"
                          " handler2_address_const: .word prvGetRegistersFromStack    \n"
                      );
                  }

                  void MemManage(void) {

                    while (true) {}
                  }

                  void BusFault(void) {

                    while (true) {}
                  }

                  void UsageFault(void) {

                    while (true) {}
                  }

                  void ServiceCall(void) {

                    while (true) {}
                  }

                  void PendService(void) {

                    while (true) {}
                  }

                  void DebugMon(void) {
                    while (true) {}
                  }
                }

          } /* namespace STM32 */
    }
} /* namespace IDPegasus */
