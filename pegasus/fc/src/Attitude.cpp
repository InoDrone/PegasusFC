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
#include "core/include/Math.h"

namespace pegasus
{
    namespace fc
    {

        using namespace pegasus::core;

        Engine *e = &engine; // alias

        Attitude::Attitude() :
                lastMillis(0),
                dcm(),
                _mAltitudeUpdate(0)
        {
            gyroSum = {0 ,0.0f, 0.0f, 0.0f};
            accSum  = {0, 0.0f, 0.0f, 0.0f};

            pegasus::hal::InterruptRegister::registerService(this, pegasus::fc::service::DRDY_ACCGYRO);
            pegasus::hal::InterruptRegister::registerService(this, pegasus::fc::service::BARO_UPDATED);
            pegasus::hal::InterruptRegister::registerService(this, pegasus::fc::service::SONAR_UPDATED);
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
         * Handled by call Service BARO_UPDATED
         */
        void Attitude::baroUpdated()
        {
        	att.pressure = e->baro->getPressure();
        	att.temperature = e->baro->getTemp();
        	att.altitude = e->baro->getAltitude();
        }

        /**
         * Handled by call Service SONAR_UPDATED
         */
        void Attitude::sonarUpdated()
        {
        	att.sonar = e->sonar->getDistance();
        }

        /**
         * Handled by Engine timer update at 200hz
         * MPU6000 DRDY at 1Khz (5 values per call)
         */
        void Attitude::update()
        {

            if (gyroSum.count == 0 || accSum.count == 0) return;

            att.gyro.x = -(float)((float)gyroSum.x / (float)gyroSum.count);
            att.gyro.y = -(float)((float)gyroSum.y / (float)gyroSum.count);
            att.gyro.z = (float)((float)gyroSum.z / (float)gyroSum.count);

            att.acc.x = -(float)((float)accSum.x / (float)accSum.count);
            att.acc.y = -(float)((float)accSum.y / (float)accSum.count);
            att.acc.z = (float)((float)accSum.z / (float)accSum.count);

            gyroSum = {0,0,0,0};
            accSum = {0,0,0,0};

            // Compute DCM
            dcm.update(&att, ATT_LOOP_SEC); // G_DT 5ms
        }

        const Attitude_t &Attitude::getAttitude() const
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
                case pegasus::fc::service::BARO_UPDATED:
                	baroUpdated();
                	break;
                case pegasus::fc::service::SONAR_UPDATED:
                	sonarUpdated();
                	break;
                default:
                    break;
            }
        }

    } /* namespace fc */
} /* namespace pegasus */
