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

#include "hal/stm32f4/include/DataStore.h"
#include <string.h>

namespace pegasus
{
    namespace hal
    {
        namespace stm32f4
        {

            DataStore::DataStore() :
                DataStoreBase() {}

            uint16_t DataStore::save(uint32_t * datas, unsigned int len)
            {
                uint32_t status = DS_ERROR;
                uint16_t retries = 0;
                uint16_t i;

                unlockFlash();
                FLASH->SR = FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR;

                if ( (status = eraseFlash(PAGE0_ID)) != FLASH_COMPLETE) {
                    status = DS_ERROR;
                }

                if (status == FLASH_COMPLETE) {
                    for (i=0;i<len;i++) {
                        do {
                            status = writeFlash(DS_START_ADDR + i*4, *(datas + i));
                        } while (status != FLASH_COMPLETE && retries < 5);

                        if (retries == 5) {
                            status = DS_ERROR;
                            break;
                        }
                    }

                    status = DS_SUCCESS;
                }

                lockFlash();

                return status;
            }

            uint16_t DataStore::read(uint32_t * datas, unsigned int len)
            {
                memcpy(datas, (uint32_t*) DS_START_ADDR, len);

                return DS_SUCCESS;
            }


            void DataStore::lockFlash()
            {
                FLASH->CR |= FLASH_CR_LOCK;
            }

            void DataStore::unlockFlash()
            {
                if((FLASH->CR & FLASH_CR_LOCK) != RESET)
                {
                  /* Authorize the FLASH Registers access */
                  FLASH->KEYR = FLASH_KEY1;
                  FLASH->KEYR = FLASH_KEY2;
                }

            }

            uint16_t DataStore::writeFlash(uint32_t addr, uint32_t data)
            {
                uint16_t status = FLASH_COMPLETE;

                /* Wait for last operation to be completed */
                status = waitFlashOperation();

                if(status == FLASH_COMPLETE)
                {
                  /* if the previous operation is completed, proceed to program the new data */
                  FLASH->CR &= CR_PSIZE_MASK;
                  FLASH->CR |= FLASH_PSIZE_WORD;
                  FLASH->CR |= FLASH_CR_PG;

                  *(__IO uint32_t*)addr = data;

                  /* Wait for last operation to be completed */
                  status = waitFlashOperation();

                  /* if the program operation is completed, disable the PG Bit */
                  FLASH->CR &= (~FLASH_CR_PG);
                }
                /* Return the Program Status */
                return status;
            }

            uint16_t DataStore::waitFlashOperation()
            {
              __IO uint16_t status = FLASH_COMPLETE;

              /* Check for the FLASH Status */
              status = getFlashStatus();

              /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
                 Even if the FLASH operation fails, the BUSY flag will be reset and an error
                 flag will be set */
              while(status == FLASH_BUSY)
              {
                status = getFlashStatus();
              }
              /* Return the operation status */
              return status;

            }

            uint16_t DataStore::getFlashStatus(void)
            {
                uint16_t flashstatus = FLASH_COMPLETE;

              if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
              {
                flashstatus = FLASH_BUSY;
              }
              else
              {
                if((FLASH->SR & FLASH_FLAG_WRPERR) != (uint32_t)0x00)
                {
                  flashstatus = FLASH_ERROR_WRP;
                }
                else
                {
                  if((FLASH->SR & FLASH_FLAG_RDERR) != (uint32_t)0x00)
                  {
                    flashstatus = FLASH_ERROR_RD;
                  }
                  else
                  {
                    if((FLASH->SR & (uint32_t)0xEF) != (uint32_t)0x00)
                    {
                      flashstatus = FLASH_ERROR_PROGRAM;
                    }
                    else
                    {
                      if((FLASH->SR & FLASH_FLAG_OPERR) != (uint32_t)0x00)
                      {
                        flashstatus = FLASH_ERROR_OPERATION;
                      }
                      else
                      {
                        flashstatus = FLASH_COMPLETE;
                      }
                    }
                  }
                }
              }
              /* Return the FLASH Status */
              return flashstatus;
            }

            uint16_t DataStore::eraseFlash(uint32_t addr)
            {
                uint16_t status = FLASH_COMPLETE;

                /* Wait for last operation to be completed */
                status = waitFlashOperation();

                if(status == FLASH_COMPLETE)
                {
                  /* if the previous operation is completed, proceed to erase the sector */
                  FLASH->CR &= CR_PSIZE_MASK;
                  FLASH->CR |= FLASH_PSIZE_WORD;
                  FLASH->CR &= SECTOR_MASK;
                  FLASH->CR |= FLASH_CR_SER | addr;
                  FLASH->CR |= FLASH_CR_STRT;

                  /* Wait for last operation to be completed */
                  status = waitFlashOperation();

                  /* if the erase operation is completed, disable the SER Bit */
                  FLASH->CR &= (~FLASH_CR_SER);
                  FLASH->CR &= SECTOR_MASK;
                }
                /* Return the Erase Status */
                return status;
            }

        } /* namespace stm32f4 */
    } /* namespace hal */
} /* namespace pegasus */
