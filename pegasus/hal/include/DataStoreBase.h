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

#ifndef HAL_DATASTORE_H_
#define HAL_DATASTORE_H_

#include <stdint.h>
#include "fc/include/Defs.h"

namespace pegasus {
    namespace hal {

        class DataStoreBase {
            public:
                DataStoreBase() {};

                virtual uint16_t save(uint32_t * datas, unsigned int len) = 0;
                virtual uint16_t read(uint32_t * datas, unsigned int len) = 0;

        };

    } // end hal
}// end pagasus

#endif /* HAL_EEPROM_H_ */
