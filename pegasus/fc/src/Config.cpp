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

#include "fc/include/Config.h"
#include "fc/include/ConfigDefault.h"

Config_t p __attribute__((section(".ccm")));

uint16_t loadDefaultConfig(Config_t* p)
{
    p->version = DFLT_CONFIG_VERSION;
    p->frameType = DFLT_FRAME_TYPE;

    p->lvlPitchPID = p->lvlRollPID = { DFLT_LVL_TILT_P, DFLT_LVL_TILT_I, DFLT_LVL_TILT_D };
    p->lvlYawPID  = { DFLT_LVL_YAW_P, DFLT_LVL_YAW_I, DFLT_LVL_YAW_D };

    p->ratePitchPID = p->rateRollPID = { DFLT_RATE_TILT_P, DFLT_RATE_TILT_I, DFLT_RATE_TILT_D };
    p->rateYawPID = {DFLT_RATE_YAW_P, DFLT_RATE_YAW_I,DFLT_RATE_YAW_D};

    p->calRcThrottle.min= p->calRcRoll.min = p->calRcPitch.min = p->calRcYaw.min = VAL_PWM_MIN;
    p->calRcThrottle.max= p->calRcRoll.max = p->calRcPitch.max = p->calRcYaw.max = VAL_PWM_MAX;
    p->calRcThrottle.center= p->calRcRoll.center = p->calRcPitch.center = p->calRcYaw.center = VAL_PWM_CENTER;

    p->escIDLE = IDLE_ESC;

    p->altSonarPID.kP = DFLT_ALT_SONAR_P;
    p->altSonarPID.kI = DFLT_ALT_SONAR_I;
    p->altSonarPID.kD = DFLT_ALT_SONAR_D;
}
