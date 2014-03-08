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
#include "core/include/Trace.h"
#include "UAVLink.h"

#include <string.h>

using namespace pegasus::fc;
using namespace pegasus::core;

const pegasus::fc::Engine *e = &engine;
uavlink_message_sensor_t sensors;
uavlink_message_command_t cmds;
uavlink_message_status_t status;
uavlink_message_t msg;

Telemetry::Telemetry():
Thread("TELEMETRY", [](Telemetry* p) { p->run();}) {}

void Telemetry::run()
{
    Attitude_t att;

    while(1) {

        com.ping();
        /* if com not active disable telemetry */
        if (!com.isActive()) {
            engine.rm(ENGINE_RCCALIBRATION);
            continue;
        }

        att = e->attitude.getAttitude();
        memcpy(&status.gyro, &att.gyro, sizeof(att.gyro));
        //memcpy(&status.acc, &att.acc, sizeof(att.acc));
        //memcpy(&status.mag, &att.mag, sizeof(att.mag));
        //memcpy(&status.angle, &att.euler, sizeof(att.euler));
        status.frameType = p.frameType;
        status.version = p.version;

        msg = uavlink_message_status_encode(&status);
        com.send(msg);

        // IF engine status is RC CALIBRATION send rc raw value.
        if (engine.is(ENGINE_RCCALIBRATION)) {
            cmds.throttle = e->rc->throttle.getInput();
            cmds.roll     = e->rc->roll.getInput();
            cmds.pitch    = e->rc->pitch.getInput();
            cmds.yaw      = e->rc->yaw.getInput();

            msg = uavlink_message_command_encode(&cmds);
            com.send(msg);
        }

        // pegasus::core::com.send(uavlink_message_sensor_encode(&sensors));
        // pegasus::core::trace.debug("[RC] Throttle : %d, P: %d, R: %d, Y: %d", e->rc->throttle.getInput(), e->rc->pitch.getInput(), e->rc->roll.getInput(), e->rc->yaw.getInput());

        //pegasus::core::trace.debug("EULER %f, %f, %f", RAD2DEG(att.euler.roll), RAD2DEG(att.euler.pitch), RAD2DEG(att.euler.yaw));


        sleep(100);
    }
}

Telemetry telemetry;

