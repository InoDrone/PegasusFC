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

#ifndef EEPROM_H_
#define EEPROM_H_

#include "hal/include/DataStoreBase.h"
#include "stm32f4xx_flash.h"

#define PAGE_SIZE               (uint32_t)0x4000        /* Page size = 16KBytes */
#define DS_START_ADDR           ((uint32_t) 0x08008000)

#define PAGE0_BASE_ADDR         ((uint32_t) (DS_START_ADDR + 0x0000))
#define PAGE0_END_ADDR          ((uint32_t) (DS_START_ADDR + (PAGE_SIZE -1)))

#define PAGE1_BASE_ADDR         ((uint32_t) (DS_START_ADDR + 0x4000))
#define PAGE1_END_ADDR          ((uint32_t) (DS_START_ADDR + 2 * PAGE_SIZE))

#define PAGE0_ID        0x010
#define PAGE1_ID        0x018

#define DS_STATUS_ADDR      ((uint32_t) PAGE0_BASE_ADDR)
#define DS_VADDR(ptr)       ((uint32_t)(PAGE0_BASE_ADDR + (uint32_t)&(((StoredDatas*) 0)->ptr)) + 4)

#define DS_ERROR        1
#define DS_SUCCESS      0
#define DS_FORMATED     ((uint32_t)0x00005555)
#define DS_VALID        2

#ifndef SECTOR_MASK
#define SECTOR_MASK               ((uint32_t)0xFFFFFF07)
#endif

#define DS_VALID_PAGE   ((uint32_t) 0x555666)
#define DS_RECEIVE_PAGE ((uint32_t) 0x888999)
#define DS_ERASED_PAGE  ((uint32_t) 0x111222)

#define DS_WRITE   1
#define DS_READ    2

#define DS_PAGE0   0x10
#define DS_PAGE1   0x11

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {

            class DataStore : public DataStoreBase
            {
                public:
                    DataStore();


                    uint16_t save(uint32_t * datas, unsigned int len);
                    uint16_t read(uint32_t * datas, unsigned int len);
                private:
                    void lockFlash();
                    void unlockFlash();
                    uint16_t writeFlash(uint32_t addr, uint32_t data);
                    uint16_t waitFlashOperation();
                    uint16_t getFlashStatus();
                    uint16_t eraseFlash(uint32_t addr);
            };

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */

#endif /* EEPROM_H_ */
