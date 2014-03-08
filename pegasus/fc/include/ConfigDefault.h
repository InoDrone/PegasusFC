/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author: Marc Jacquier <marc@inodrone.com>
 *  Project: InoDronePegasus
 */

#ifndef CONFIGDEFAULT_H_
#define CONFIGDEFAULT_H_

#include "core/include/Defs.h"

/***************************************************************
 * DEFAULT CONFIG
 */

#define DFLT_CONFIG_VERSION 2

/* LEVEL ATTITUDE PITCH / ROLL */
#define DFLT_LVL_TILT_P     100.0f
#define DFLT_LVL_TILT_I     0.0002f
#define DFLT_LVL_TILT_D     1000.0f


/* LEVEL ATTITUDE and HEAD_LOCK PID for Yaw*/
#define DFLT_LVL_YAW_P      0.05f
#define DFLT_LVL_YAW_I      0.00002f
#define DFLT_LVL_YAW_D      0.0f

/* RATE PITCH / ROLL PID */
#define DFLT_RATE_TILT_P   0.0f
#define DFLT_RATE_TILT_I   0.0f
#define DFLT_RATE_TILT_D   1000.0f

/* RATE PITCH / ROLL PID */
#define DFLT_RATE_YAW_P   150.0f
#define DFLT_RATE_YAW_I   0.250f
#define DFLT_RATE_YAW_D   0.25f

#define DFLT_FRAME_TYPE FRAME_X4

/**************************************************************
 * FLIGHT CONTROLER
 **************************************************************/
#define DEFAULT_PEGASUS_VERSION PEGASUS_VB
#define DEFAULT_RC_INPUT_TYPE   RC_INPUT_SEPARATED_SIGNALS // RC_INPUT_PPM

/**************************************************************
 * ATITUDE CONFIG
 **************************************************************/
#define DEFAULT_MAX_ALTITUDE 50        // Max altitude in meter
#define DEFAULT_MAX_ANGLE    45        // Max atitude angle (Roll, Pitch)
#define DEFAULT_SONAR_STALL  3         // Sonar stall in meter

#endif /* CONFIGDEFAULT_H_ */
