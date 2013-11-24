/*
 * PegasusSTM32VB.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSSTM32VB_H_
#define PEGASUSSTM32VB_H_

#include "pegasus/core/include/Defs.h"

#define MOTOR1 Port2Pin('C', 6)         // C6 (TIM8 CH1)
#define MOTOR2 Port2Pin('C', 7)         // C7 (TIM8 CH2)
#define MOTOR3 Port2Pin('C', 8)         // C8 (TIM8 CH3)
#define MOTOR4 Port2Pin('C', 9)         // C9 (TIM8 CH4)
#define MOTOR5 Port2Pin('A', 0)         // A0 (TIM5 CH1)
#define MOTOR6 Port2Pin('A', 1)         // A1 (TIM5 CH2)
#define MOTOR7 Port2Pin('A', 2)         // A2 (TIM5 CH3)
#define MOTOR8 Port2Pin('A', 3)         // A3 (TIM5 CH4)

#define SERVO1 Port2Pin('E', 5)         // E5 (TIM9 CH1)
#define SERVO2 Port2Pin('E', 6)         // E6 (TIM9 CH2)
#define SERVO3 Port2Pin('B', 10)        // B10 (TIM2 CH3)
#define SERVO4 Port2Pin('B', 11)        // B11 (TIM2 CH4)

#define RC1 Port2Pin('E', 9)    // E9  (TIM1 CH1)
#define RC2 Port2Pin('E', 11)   // E11 (TIM1 CH2)
#define RC3 Port2Pin('E', 13)   // E13 (TIM1 CH3)
#define RC4 Port2Pin('E', 14)   // E14 (TIM1 CH4)
#define RC5 Port2Pin('D', 12)   // D12 (TIM4 CH1)
#define RC6 Port2Pin('D', 13)   // D13 (TIM4 CH2)
#define RC7 Port2Pin('D', 14)   // D14 (TIM4 CH3)
#define RC8 Port2Pin('D', 15)   // D15 (TIM4 CH4)

#define RC_PPM             RC1
#define RC_THROTTLE        RC1
#define RC_ROLL            RC2
#define RC_PITCH           RC3
#define RC_YAW             RC4


static inline void initMix() {

    pegasus::core::trace.debug("[MIXING] Initialization");
    pegasus::core::mix.init(FRAME_TYPE);
    switch (FRAME_TYPE) {
        case FRAME_QUAD:
        case FRAME_X4:
            /*Pegasus::core::mix.add(MOTOR1, 0, ESC_SPEED);
            Pegasus::core::mix.add(MOTOR2, 1, ESC_SPEED);
            Pegasus::core::mix.add(MOTOR3, 2, ESC_SPEED);
            Pegasus::core::mix.add(MOTOR4, 3, ESC_SPEED);*/
            break;
        default:
            pegasus::core::trace.error("[MIXING] Error no frame found");
    }
    pegasus::core::trace.debug("[MIXING] Initialized done");
}

static inline void initCommunication() {
    pegasus::core::trace.debug("[DEBUG SERIAL] Not implemented");
    //TODO init debug serial
    pegasus::core::trace.debug("[BLUETOOTH] Not implemented");
    //TODO init serial over Bluetooth and UavLink
}


static inline void initPlatform() {
    initCommunication();

    initMix();
}

#endif /* PEGASUSSTM32VB_H_ */
