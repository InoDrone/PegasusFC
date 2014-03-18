/*
 * PegasusSTM32VB.h
 *
 *  Created on: 23 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSSTM32VB_H_
#define PEGASUSSTM32VB_H_

#include "Config.h"

#include "core/include/Pegasus.h"
#include "fc/include/Mixing.h"
#include "fc/include/Engine.h"

#include "peripherals/MPU6000.h"
#include "peripherals/MS5611.h"
#include "peripherals/MaxSonar.h"

#define ESC_SPEED       ESC_200HZ
#define SERVO_SPEED     SERVO_50HZ

#define RC_PPM             RC1
#define RC_THROTTLE        RC1
#define RC_ROLL            RC2
#define RC_PITCH           RC3
#define RC_YAW             RC4
#define RC_ALT_HOLD        RC5


void initPlatform();

#endif /* PEGASUSSTM32VB_H_ */
