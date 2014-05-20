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

#include "MS5611.h"
#include "core/include/MainTimer.h"
#include "core/include/Trace.h"
#include "core/include/CriticalSection.h"
#include "core/include/Math.h"
#include <cmath>

namespace pegasus
{
    namespace peripherals
    {

        using namespace pegasus::core;

        MS5611::MS5611(pegasus::hal::SpiDeviceBase_t* spiDevice) :
                _mSpi(spiDevice),
                D1(0.0f),
                D2(0.0f),
                _mState(0),
                _mDataOk(false)
        {

        }

        bool MS5611::init()
        {
            bool initStatus = false;
            pegasus::core::trace.log("[MS5611] Initialization");

            _mSpi->setSpeed(SPI_LOW);

            _mSpi->writeByte(MS561101BA_RESET);
            pegasus::core::mainTimer.delay(4);

            memset(_PROMDatas.bytes, 0, sizeof(_PROMDatas));
            _mSpi->read(&_PROMDatas.bytes[0], MS5611_PROM_SETUP, 2);
            _mSpi->read(&_PROMDatas.bytes[2], MS5611_PROM_C1, 2);
            _mSpi->read(&_PROMDatas.bytes[4], MS5611_PROM_C2, 2);
            _mSpi->read(&_PROMDatas.bytes[6], MS5611_PROM_C3, 2);
            _mSpi->read(&_PROMDatas.bytes[8], MS5611_PROM_C4, 2);
            _mSpi->read(&_PROMDatas.bytes[10], MS5611_PROM_C5, 2);
            _mSpi->read(&_PROMDatas.bytes[12], MS5611_PROM_C6, 2);
            _mSpi->read(&_PROMDatas.bytes[14], MS5611_PROM_CRC, 2);

            /* SWAP BIT */
            uint8_t len = sizeof(_PROMDatas) / 2;
            uint8_t i = 0;

            while (len--) {
                uint8_t t = _PROMDatas.bytes[i];
                _PROMDatas.bytes[i] = _PROMDatas.bytes[i+1];
                _PROMDatas.bytes[i+1] = t;
                i += 2;
            }

            pegasus::core::trace.debug("[MS5611] read prom : %d, %d, %d, %d, %d, %d, %d, %d", _PROMDatas.val.MSSETUP, _PROMDatas.val.C1, _PROMDatas.val.C2, _PROMDatas.val.C3, _PROMDatas.val.C4, _PROMDatas.val.C5, _PROMDatas.val.C6, _PROMDatas.val.MSCRC);

            if (!checkCRC()) {
                pegasus::core::trace.error("[MS5611] CRC error.");
                initStatus = false;
            } else {
                pegasus::core::trace.debug("[MS5611] CRC ok.");
                initStatus = true;
            }


            _mSpi->setSpeed(SPI_HIGH);

            /* read temperature */
            _mSpi->writeByte(MS561101BA_D2_4096);
            _mState = 0;
            _mTemp = 0;
            _mPress = 0;
            _mD1Sum = 0;
            _mD2Sum = 0;
            _mD1Count = _mD2Count = 0;
            _mAltitude = 0;

            update();
            pegasus::core::mainTimer.delay(10);
            update();
            pegasus::core::mainTimer.delay(10);
            calculate();

            pegasus::core::trace.log("[MS5611] Done");

            return initStatus;

        }

        // from AN520
        bool MS5611::checkCRC()
        {
            uint16_t crc = 0x00;
            uint8_t i, n;
            uint8_t crcRead = (uint8_t)(_PROMDatas.val.MSCRC & 0xF); // Save 4 bit
            _PROMDatas.val.MSCRC = 0xFF00 & _PROMDatas.val.MSCRC;

            for (i=0;i < 16;i++)
            {
                if (i%2 == 1) crc ^= (_PROMDatas.word[i>>1] & 0x00FF);
                else crc ^= (_PROMDatas.word[i>>1] >> 8);
                for (n=8; n > 0; n--) {
                       if (crc & 0x8000) {
                           crc = (crc << 1) ^ 0x3000;
                       } else {
                           crc = (crc << 1);
                       }
                }
            }

            crc = (0x000F & (crc >> 12));
            _PROMDatas.val.MSCRC = crcRead;
            return (crcRead == (uint8_t)crc);
        }

        uint32_t MS5611::readAdc()
        {
            uint8_t buffer[3];
            _mSpi->read(buffer, MS561101BA_ADC, 3);

            return (((uint32_t)buffer[0]) << 16) | (((uint32_t)buffer[1]) << 8) | buffer[2];
        }

        // called at 100Hz
        void MS5611::update()
        {

            //pegasus::hal::Processor::disableInterrupts();
            if (_mState == 0) {
                _mD2Sum += readAdc();
                _mD2Count++;

                if (_mD2Count == 32) {
                    _mD2Sum >>= 1;
                    _mD2Count = 16;
                }

                _mState++;
                _mSpi->writeByte(MS561101BA_D1_4096);
            } else {
                _mD1Sum += readAdc();
                _mD1Count++;

                if (_mD1Count == 128) {
                    _mD1Sum >>= 1;
                    _mD1Count = 64;
                }

                _mState++;
                _mDataOk = true;
                if (_mState == 5) {
                    _mSpi->writeByte(MS561101BA_D2_4096);
                    _mState = 0;
                } else {
                    _mSpi->writeByte(MS561101BA_D1_4096);
                }
            }
            //pegasus::hal::Processor::enableInterrupts();

        }

        void MS5611::calculate()
        {
            int32_t dT;
            int32_t TEMP;
            int64_t OFF;
            int64_t SENS;

            /* Update value */
            {
                pegasus::core::CriticalSection cs;

                if (_mDataOk) {
                    if (_mD1Count != 0) {
                        D1 = ((float)_mD1Sum) / _mD1Count;
                    }

                    if (_mD2Count != 0) {
                        D2 = ((float)_mD2Sum) / _mD2Count;
                    }

                    _mDataOk = false;
                }
            }

            // Calculate temperature
            dT = D2 - (((uint32_t)_PROMDatas.val.C5) << 8); // D2 - C5 * 2^8
            TEMP = (int64_t)dT * _PROMDatas.val.C6 / 8388608 + 2000;
            _mTemp = TEMP * 0.01f;


            // Calculate compensated pressure
            OFF = ((int64_t)_PROMDatas.val.C2 << 16) + ((int64_t)dT * _PROMDatas.val.C4 >> 7);
            if (OFF > 12884705280 ) {
                OFF = 12884705280;
            } else if( OFF < -8589672450 ) {
                OFF = -8589672450;
            }
            SENS = ((int64_t)_PROMDatas.val.C1 << 15) + ((int64_t)dT * _PROMDatas.val.C3 >> 8);
            if (SENS > 6442352640 ) {
                SENS = 6442352640;
            } else if (SENS < -4294836225) {
                SENS = -4294836225;
            }

            // TODO Compensate temperature if < 0

            _mPress = (D1 * SENS / 2097152 - OFF) * (1.0f / 32768.0f);

            if (_mPress > 0) {
                //float scaling = att.pressure / 101325.0f;
                //float tempS = att.temperature + 273.15f;
                float altitude = (1.0f - powf((_mPress / 101325.0f), 0.19f)) * (1.0f / 22.558e-6f);

                if (_mAltitude == 0) {
                    _mAltitude = altitude;
                } else {
                    _mAltitude = Math::filter(altitude, _mAltitude, 0.1f);
                }

                CALLSV(pegasus::fc::service::BARO_UPDATED);
            }

        }

    } /* namespace peripherals */
} /* namespace pegasus */
