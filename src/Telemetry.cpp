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


const pegasus::fc::Engine *e = &pegasus::fc::engine;


Telemetry::Telemetry():
Thread("TELEMETRY", [](Telemetry* p) { p->run();}) {}

void Telemetry::run()
{
    while(1) {

        Attitude_t att = e->attitude.getAttitude();

        sleep(100);
    }
}

Telemetry telemetry;

