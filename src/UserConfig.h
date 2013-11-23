/*
 * UserConfig.h
 *
 *  Created on: 22 nov. 2013
 *      Author: alienx
 */

#ifndef USERCONFIG_H_
#define USERCONFIG_H_


/**************************************************************
 * FRAME CONFIGURATION
 *
 * - FRAME_QUAD
 * - FRAME_X4
 * - FRAME_HEXA
 * - FRAME_X8
 * - FRAME_Y3
 * - FRAME_Y6
 * - FRAME_OCTO
 * - FRAME_OSPREY
 **************************************************************/
#define FRAME_TYPE FRAME_QUAD

/**************************************************************
 * FLIGHT CONTROLER
 **************************************************************/
#define PEGASUS_VERSION PEGASUS_VB
#define ESC_SPEED       ESC_200HZ
#define SERVO_SPEED     SERVO_50HZ

#define RC_INPUT_TYPE   RC_INPUT_SEPARATED_SIGNALS // RC_INPUT_PPM

/**************************************************************
 * ATITUDE CONFIG
 **************************************************************/
#define MAX_ALTITUDE 50        // Max altitude in meter
#define MAX_ANGLE    45        // Max atitude angle (Roll, Pitch)
#define SONAR_STALL  3         // Sonar stall in meter


#endif /* USERCONFIG_H_ */
