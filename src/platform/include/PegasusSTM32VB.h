/*
 * PegasusSTM32VB.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSSTM32VB_H_
#define PEGASUSSTM32VB_H_

#include "core/include/Pegasus.h"
#include "fc/include/Mixing.h"
#include "fc/include/Engine.h"

#include "peripherals/L3GD20.h"
#include "peripherals/MaxSonar.h"

#define MOTOR1_PIN pegasus::hal::PORTMAPPING('C', 6, TIM8, pegasus::timer::CHANNEL_1, ESC_SPEED) // C6 (TIM8 CH1)
#define MOTOR2_PIN pegasus::hal::PORTMAPPING('C', 7, TIM8, pegasus::timer::CHANNEL_2, ESC_SPEED) // C7 (TIM8 CH2)
#define MOTOR3_PIN pegasus::hal::PORTMAPPING('C', 8, TIM8, pegasus::timer::CHANNEL_3, ESC_SPEED) // C8 (TIM8 CH3)
#define MOTOR4_PIN pegasus::hal::PORTMAPPING('C', 9, TIM8, pegasus::timer::CHANNEL_4, ESC_SPEED) // C9 (TIM8 CH4)
#define MOTOR5_PIN pegasus::hal::PORTMAPPING('A', 0, TIM5, pegasus::timer::CHANNEL_1, ESC_SPEED) // A0 (TIM5 CH1)
#define MOTOR6_PIN pegasus::hal::PORTMAPPING('A', 1, TIM5, pegasus::timer::CHANNEL_2, ESC_SPEED) // A1 (TIM5 CH2)
#define MOTOR7_PIN pegasus::hal::PORTMAPPING('A', 2, TIM5, pegasus::timer::CHANNEL_3, ESC_SPEED) // A2 (TIM5 CH3)
#define MOTOR8_PIN pegasus::hal::PORTMAPPING('A', 3, TIM5, pegasus::timer::CHANNEL_4, ESC_SPEED) // A3 (TIM5 CH4)

#define SERVO1_PIN pegasus::hal::PORTMAPPING('E',  5, TIM9, pegasus::timer::CHANNEL_1, SERVO_SPEED) // E5 (TIM9 CH1)
#define SERVO2_PIN pegasus::hal::PORTMAPPING('E',  6, TIM9, pegasus::timer::CHANNEL_2, SERVO_SPEED) // E6 (TIM9 CH2)
#define SERVO3_PIN pegasus::hal::PORTMAPPING('B', 10, TIM2, pegasus::timer::CHANNEL_3, SERVO_SPEED) // B10 (TIM2 CH3)
#define SERVO4_PIN pegasus::hal::PORTMAPPING('B', 11, TIM2, pegasus::timer::CHANNEL_4, SERVO_SPEED)  // B11 (TIM2 CH4)

#define RC1_PIN pegasus::hal::PORTMAPPING('E',  9, TIM1, pegasus::timer::CHANNEL_1, 0 /* TODO */) // E9  (TIM1 CH1)
#define RC2_PIN pegasus::hal::PORTMAPPING('E', 11, TIM1, pegasus::timer::CHANNEL_2, 0 /* TODO */) // E11 (TIM1 CH2)
#define RC3_PIN pegasus::hal::PORTMAPPING('E', 13, TIM1, pegasus::timer::CHANNEL_3, 0 /* TODO */) // E13 (TIM1 CH3)
#define RC4_PIN pegasus::hal::PORTMAPPING('E', 14, TIM1, pegasus::timer::CHANNEL_4, 0 /* TODO */) // E14 (TIM1 CH4)
#define RC5_PIN pegasus::hal::PORTMAPPING('D', 12, TIM4, pegasus::timer::CHANNEL_1, 0 /* TODO */) // D12 (TIM4 CH1)
#define RC6_PIN pegasus::hal::PORTMAPPING('D', 13, TIM4, pegasus::timer::CHANNEL_2, 0 /* TODO */) // D13 (TIM4 CH2)
#define RC7_PIN pegasus::hal::PORTMAPPING('D', 14, TIM4, pegasus::timer::CHANNEL_3, 0 /* TODO */) // D14 (TIM4 CH3)
#define RC8_PIN pegasus::hal::PORTMAPPING('D', 15, TIM4, pegasus::timer::CHANNEL_4, 0 /* TODO */) // D15 (TIM4 CH4)

#define RC_PPM             RC1
#define RC_THROTTLE        RC1
#define RC_ROLL            RC2
#define RC_PITCH           RC3
#define RC_YAW             RC4

void initPlatform();

#endif /* PEGASUSSTM32VB_H_ */
