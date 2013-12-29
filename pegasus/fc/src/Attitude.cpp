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

#include "fc/include/Engine.h"
#include "fc/include/Attitude.h"
#include "core/include/Trace.h"
#include "core/include/MainTimer.h"

namespace pegasus
{
    namespace fc
    {

        Engine *e = &engine; // alias

        Attitude::Attitude() :
                lastMillis(0)
        {
            gyroSum = {0 ,0.0f, 0.0f, 0.0f};
            accSum  = {0, 0.0f, 0.0f, 0.0f};

            pegasus::hal::InterruptRegister::registerService(this, pegasus::fc::service::DRDY_ACCGYRO);
            //register service DRDY_MAG
            //register service SONAR_UPDATE
        }

        /**
         * Handled by call Service DRDY_ACCGYRO
         */
        void Attitude::gyroAccSum()
        {
            Motion6f datas;

            e->gyroacc->getMotion6(&datas);
            gyroSum.x += datas.gyro.x;
            gyroSum.y += datas.gyro.y;
            gyroSum.z += datas.gyro.z;
            gyroSum.count++;

            accSum.x += datas.acc.x;
            accSum.y += datas.acc.y;
            accSum.z += datas.acc.z;
            accSum.count++;
        }

        /**
         * Handled by Engine timer update at 200hz
         * MPU6000 DRDY at 1Khz (5 values per call)
         */
        void Attitude::update()
        {

            att.gyro.x = (float)((float)gyroSum.x / (float)gyroSum.count);
            att.gyro.y = (float)((float)gyroSum.y / (float)gyroSum.count);
            att.gyro.z = (float)((float)gyroSum.z / (float)gyroSum.count);

            att.acc.x = (float)((float)accSum.x / (float)accSum.count);
            att.acc.y = (float)((float)accSum.y / (float)accSum.count);
            att.acc.z = (float)((float)accSum.z / (float)accSum.count);

            gyroSum = {0,0,0,0};
            accSum = {0,0,0,0};

            // Compute DCM
            // To EULER Angle
        }

        Attitude_t Attitude::getAttitude() const
        {
            return att;
        }

        /**
         * Service Call Handler Interrupt
         */
        void Attitude::svc (pegasus::fc::service::Service id) {
            switch(id) {
                case pegasus::fc::service::DRDY_ACCGYRO:
                    gyroAccSum();
                    break;
                default:
                    break;
            }
        }

    } /* namespace fc */
} /* namespace pegasus */
