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

#ifndef BAROBASE_H_
#define BAROBASE_H_

#include <inttypes.h>

namespace pegasus {
    namespace fc {

        class BaroBase {
                public:

                virtual bool init() = 0;
                virtual void update() = 0;
                virtual void calculate() = 0;

                virtual float getPressure() = 0;
                virtual float getTemp() = 0;

                float getAltitude() {
                    return _mAltitude;
                }

                uint32_t lastUpdate;
                float _mAltitude;
        };
    }
}


#endif /* BAROBASE_H_ */
