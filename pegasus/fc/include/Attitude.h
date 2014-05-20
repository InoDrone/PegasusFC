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

#ifndef ATTITUDE_H_
#define ATTITUDE_H_

#include "fc/include/Defs.h"
#include "fc/include/GyroAccBase.h"
#include "fc/include/DCM.h"
#include "hal/include/ServicesListener.h"

namespace pegasus
{
    namespace fc
    {

        class Attitude : public pegasus::hal::ServicesListener
        {
            public:
                Attitude();
                void gyroAccSum();
                void update();

                void baroUpdated();
                void sonarUpdated();

                const Attitude_t &getAttitude() const;

                void svc(pegasus::fc::service::Service id);

            private:
                long lastMillis;
                DCM dcm;
                uint32_t _mAltitudeUpdate;

                struct AxisSum_t {
                    int16_t count;

                    float x;
                    float y;
                    float z;
                };

                AxisSum_t gyroSum;
                AxisSum_t accSum;

                Attitude_t att;
        };

    } /* namespace fc */
} /* namespace pegasus */

#endif /* ATTITUDE_H_ */
