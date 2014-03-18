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

#ifndef FC_DEFS_H_
#define FC_DEFS_H_

#include <stdint.h>

namespace pegasus {
    namespace fc {
        namespace service {

            enum Service {
                DRDY_ACCGYRO,
                SERVICE_SIZE
            };
        }

    }
}

/***********************************************
 *  Struct Attitude
 ***********************************************/
struct Axis {
        int16_t x;
        int16_t y;
        int16_t z;
};

struct Motion6 {
        Axis acc;
        Axis gyro;
};

struct Axisf {
        float x;
        float y;
        float z;
};

struct Motion6f {
        Axisf acc;
        Axisf gyro;
};

struct Attitude_t {
        Axisf acc;
        Axisf gyro;
        Axisf mag;
        struct {
               float roll;
               float pitch;
               float yaw;
        } euler;
        float sonar;
        float altitude;
        float pressure;
        float temperature;
};


#endif /* DEFS_H_ */
