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

#include "Telemetry.h"
#include "fc/include/Engine.h"
#include "fc/include/Supervisor.h"
#include "core/include/Trace.h"
#include "core/include/CriticalSection.h"
#include "UAVLink.h"

#include <string.h>
#include <stdio.h>

using namespace pegasus::fc;
using namespace pegasus::core;

const pegasus::fc::Engine *e = &engine;

Telemetry::Telemetry():
Thread("TELEMETRY", [](Telemetry* p) { p->run();}) {}

void Telemetry::run()
{
    uavlink_message_status_t status;
    uint16_t counter = 0;

    while(1) {

        if (counter == 10) { // 500ms
            com.ping();
            counter = 0;
        }

        counter++;


        /* if com not active disable telemetry */
        if (!com.isActive()) {
            continue;
        }

        addAttitude(&status);

        /* GPS */
        status.gps.iTOW = gpsDatas.iTOW;
        status.gps.lat = gpsDatas.lat;
        status.gps.lon = gpsDatas.lon;
        status.gps.height = gpsDatas.height;
        status.gps.vAcc = gpsDatas.vAcc;
        status.gps.hAcc = gpsDatas.hAcc;
        status.gps.sats = gpsDatas.sats;
        status.gps.fixType = gpsDatas.fixType;
        // TODO : use memcpy

        status.rc.throttle = e->rc->throttle.getInput();
        status.rc.roll = e->rc->roll.getInput();
        status.rc.pitch = e->rc->pitch.getInput();
        status.rc.yaw = e->rc->yaw.getInput();

        status.frameType = p.frameType;
        status.version = p.version;
        status.fcLoopTimeUS = sv.getAttitudeLoopTime();
        status.fcLoopReloadTimeUS = (uint16_t)(ATT_LOOP_SEC * 1000000);

        {
            pegasus::core::CriticalSection cs;

            com.send(uavlink_message_status_encode(&status));
        }

        // IF engine status is RC CALIBRATION send rc raw value.
        /*if (engine.is(ENGINE_RCCALIBRATION)) {
            cmds.throttle = e->rc->throttle.getInput();
            cmds.roll     = e->rc->roll.getInput();
            cmds.pitch    = e->rc->pitch.getInput();
            cmds.yaw      = e->rc->yaw.getInput();

            msg = uavlink_message_command_encode(&cmds);
            com.send(msg);
        }*/

        // pegasus::core::com.send(uavlink_message_sensor_encode(&sensors));
        // pegasus::core::trace.debug("[RC] Throttle : %d, P: %d, R: %d, Y: %d", e->rc->throttle.getInput(), e->rc->pitch.getInput(), e->rc->roll.getInput(), e->rc->yaw.getInput());

        //pegasus::core::trace.debug("EULER %f, %f, %f", RAD2DEG(att.euler.roll), RAD2DEG(att.euler.pitch), RAD2DEG(att.euler.yaw));

        sleep(100);
    }
}

void Telemetry::addAttitude(uavlink_message_status_t * status)
{
    Attitude_t att = e->attitude.getAttitude();
    status->gyro.x = (int16_t)(att.gyro.x * 100.0f);
    status->gyro.y = (int16_t)(att.gyro.y * 100.0f);
    status->gyro.z = (int16_t)(att.gyro.z * 100.0f);

    status->acc.x = (int16_t)(att.acc.x * 100.0f);
    status->acc.y = (int16_t)(att.acc.y * 100.0f);
    status->acc.z = (int16_t)(att.acc.z * 100.0f);

    status->angle.roll = (int16_t)(RAD2DEG(att.euler.roll) * 100.0f);
    status->angle.pitch = (int16_t)(RAD2DEG(att.euler.pitch) * 100.0f);
    status->angle.yaw = (int16_t)(RAD2DEG(att.euler.yaw) * 100.0f);

    status->altSonar = att.sonar;
    status->altBaro = att.altitude;
}

Telemetry telemetry;

