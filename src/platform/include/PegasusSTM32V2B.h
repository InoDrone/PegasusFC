/*
 * PegasusV2B.h
 *
 *  Created on: 16 avr. 2014
 *      Author: alienx
 */

#ifndef PEGASUSV2B_H_
#define PEGASUSV2B_H_

#include "Config.h"

#include "core/include/Pegasus.h"
#include "fc/include/Mixing.h"
#include "fc/include/Engine.h"

#include "peripherals/MPU6000.h"
#include "peripherals/MS5611.h"
#include "peripherals/MaxSonar.h"
#include "peripherals/UBlox.h"

#define ESC_SPEED       ESC_400HZ
#define SERVO_SPEED     SERVO_50HZ

#define RC_PPM             RC1
#define RC_THROTTLE        RC1
#define RC_ROLL            RC2
#define RC_PITCH           RC3
#define RC_YAW             RC4
#define RC_ALT_HOLD        RC5


void initPlatform();

#endif /* PEGASUSV2B_H_ */
