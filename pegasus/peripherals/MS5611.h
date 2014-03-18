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

#ifndef MS5611_H_
#define MS5611_H_

#include "fc/include/BaroBase.h"
#include "core/include/Pegasus.h"

#define MS561101BA_RESET        0x1E
/* D1 */
#define MS561101BA_D1_256       0x40
#define MS561101BA_D1_512       0x42
#define MS561101BA_D1_1024      0x44
#define MS561101BA_D1_2048      0x46
#define MS561101BA_D1_4096      0x48
/* D2 */
#define MS561101BA_D2_256       0x50
#define MS561101BA_D2_512       0x52
#define MS561101BA_D2_1024      0x54
#define MS561101BA_D2_2048      0x56
#define MS561101BA_D2_4096      0x58

/* ADC */
#define MS561101BA_ADC          0x00

/* PROM */
#define MS5611_PROM_SETUP 0xA0
#define MS5611_PROM_C1 0xA2
#define MS5611_PROM_C2 0xA4
#define MS5611_PROM_C3 0xA6
#define MS5611_PROM_C4 0xA8
#define MS5611_PROM_C5 0xAA
#define MS5611_PROM_C6 0xAC
#define MS5611_PROM_CRC 0xAE

namespace pegasus
{
    namespace peripherals
    {

        class MS5611 : public pegasus::fc::BaroBase
        {
            public:
                MS5611(pegasus::hal::SpiDeviceBase_t* spiDevice);

                bool init();
                void update();
                void calculate();

                float getPressure();
                float getTemp();


            private:
                pegasus::hal::SpiDeviceBase_t* _mSpi;

                union PROMDatas {
                     uint8_t  bytes[];
                     uint16_t word[];
                     struct {
                             uint16_t MSSETUP;
                             uint16_t C1;
                             uint16_t C2;
                             uint16_t C3;
                             uint16_t C4;
                             uint16_t C5;
                             uint16_t C6;
                             uint16_t MSCRC;
                     } val;
                };

                PROMDatas _PROMDatas;
                float D1, D2;

                uint8_t _mState;
                float _mTemp, _mPress;
                uint8_t _mD1Count, _mD2Count;
                uint32_t _mD1Sum, _mD2Sum;
                bool _mDataOk;

                bool checkCRC();
                uint32_t readAdc();
        };

        INLINE float MS5611::getPressure()
        {
            return _mPress;
        }

        INLINE float MS5611::getTemp()
        {
            return _mTemp;
        }

    } /* namespace peripherals */
} /* namespace pegasus */

#endif /* MS5611_H_ */
