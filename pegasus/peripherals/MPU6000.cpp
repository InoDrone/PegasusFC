/*
 * MPU6000.cpp
 *
 *  Created on: 1 déc. 2013
 *      Author: alienx
 */

#include <cmath>
#include "MPU6000.h"
#include "core/include/MainTimer.h"
#include "core/include/Trace.h"
#include "fc/include/Defs.h"

namespace pegasus
{
    namespace peripherals
    {

        MPU6000::MPU6000(pegasus::hal::SpiDeviceBase_t* spiDevice) :
            _mSpi(spiDevice),
            _mAccRangeScale(ONEG_MSS / MPU6000_ACC_SENS_4G),
            _mGyroRangeScale(ONE_RAD / MPU6000_GYRO_SENS_1000),
            _mGyroRangeScaleRadSec((1000.0f / 180.0f) * M_PI_F),
            _mOffsetCalculated(false) {

            _mAccScale = {
                    0.0f,
                    1.0f,
                    0.0f,
                    1.0f,
                    0.0f,
                    1.0f
            };

            _mGyroScale = {
                    0.0f,
                    1.0f,
                    0.0f,
                    1.0f,
                    0.0f,
                    1.0f
            };

        }



        bool MPU6000::init()
        {
            pegasus::core::trace.log("[MPU6000] Initialization");
            _mAutoSampling = false;

            pegasus::hal::Gpio intPin(pegasus::gpio::E, pegasus::gpio::PIN8);
            pegasus::hal::InterruptRegister::attachExternalInt(this, &intPin);

            _mSpi->setSpeed(SPI_LOW);

            uint8_t buffer = 0x0;
            _mSpi->read(&buffer, MPU6000_RA_WHO_AM_I | SPI_READWRITE_CMD, 1);
            if ((buffer & 0x7E) == 0x68) {
                pegasus::core::trace.log("[MPU6000] ping OK");
            } else {
                pegasus::core::trace.log("MPU6000 Error");
                return false;
            }

            _mSpi->read(&buffer, MPU6000_RA_PRODUCT_ID | SPI_READWRITE_CMD, 1);
            pegasus::core::trace.log("[MPU6000] Product ID : %d", buffer);


            _mSpi->writeByte(MPU6000_RA_PWR_MGMT_1, 0x80); // Reset MPU
            // Wait 100 ms
            pegasus::core::mainTimer.delay(100);
            _mSpi->writeByte(MPU6000_RA_SIGNAL_PATH_RESET, 0x07); // Reset Sensors
            // Wait 100 ms
            pegasus::core::mainTimer.delay(100);

            _mSpi->writeByte(MPU6000_RA_PWR_MGMT_1, MPU6000_CLOCK_PLL_ZGYRO); // Set clock to Z Gyro
            pegasus::core::mainTimer.delay(1);
            _mSpi->writeByte(MPU6000_RA_PWR_MGMT_2, 0);
            pegasus::core::mainTimer.delay(1);

            /* Sample Rate */
            _mSpi->writeByte(MPU6000_RA_SMPLRT_DIV, 0x00); // Set to 1Khz ( if DLPF_CGF > 0 else 0x08 )
            pegasus::core::mainTimer.delay(1);

            /* Filter */
            _mSpi->writeByte(MPU6000_RA_CONFIG, MPU6000_DLPF_BW_98);// Set to Acc 184Hz, Gyro 188Hz
            pegasus::core::mainTimer.delay(1);

            /* Gyro config */
            _mSpi->writeByte(MPU6000_RA_GYRO_CONFIG, MPU6000_GYRO_FS_1000 << 3); // 1000 °/s
            pegasus::core::mainTimer.delay(1);

            /* Acc config */
            _mSpi->writeByte(MPU6000_RA_ACCEL_CONFIG, MPU6000_ACCEL_FS_4 << 3); // 4 g/s
            pegasus::core::mainTimer.delay(1);


            /* i2c AUX config */
            _mSpi->writeByte(MPU6000_RA_USER_CTRL, 0x30); // I2C_MST_EN = 1, I2C_IF_DIS = 1
            pegasus::core::mainTimer.delay(1);

            _mSpi->writeByte(MPU6000_RA_INT_PIN_CFG, 0x10); // 0x10 INT_RD_CLEAR = 1 other 0
            pegasus::core::mainTimer.delay(1);
            _mSpi->writeByte(MPU6000_RA_INT_ENABLE, 0x1); // Enable DRDY INT
            pegasus::core::mainTimer.delay(1);

            _mSpi->setSpeed(SPI_HIGH);

            // Get Offset
            getZeroOffset();

            pegasus::core::trace.log(
                    "[MPU6000] Offset -> AX : %f, AY : %f, AZ : %f, GX : %f, GY : %f, GZ : %f",
                    _mAccScale.offsetX, _mAccScale.offsetY, _mAccScale.offsetZ,
                    _mGyroScale.offsetX, _mGyroScale.offsetY, _mGyroScale.offsetZ
            );

            pegasus::core::trace.log("[MPU6000] Initialization done");

            return true;
        }

        void MPU6000::read()
        {

            _mSpi->read(_mDatas.bytes, MPU6000_RA_ACCEL_XOUT_H | SPI_READWRITE_CMD,(uint16_t) sizeof (uDatas6));

            /* SWAP BIT */
            uint8_t len = sizeof(uDatas6) / 2;
            uint8_t i = 0;

            while (len--) {
                uint8_t t = _mDatas.bytes[i];
                _mDatas.bytes[i] = _mDatas.bytes[i+1];
                _mDatas.bytes[i+1] = t;
                i += 2;
            }


            _mSensors.acc.x = ((_mDatas.val.acc.x * _mAccRangeScale) - _mAccScale.offsetX) * _mAccScale.scaleX;
            _mSensors.acc.y = ((_mDatas.val.acc.y * _mAccRangeScale) - _mAccScale.offsetY) * _mAccScale.scaleY;
            _mSensors.acc.z = ((_mDatas.val.acc.z * _mAccRangeScale) - _mAccScale.offsetZ) * _mAccScale.scaleZ;
            if (_mOffsetCalculated) {
                _mSensors.acc.z += ONEG_MSS;
            }


            _mSensors.gyro.x = ((_mDatas.val.gyro.x * _mGyroRangeScale) - _mGyroScale.offsetX) * _mGyroScale.scaleX;
            _mSensors.gyro.y = ((_mDatas.val.gyro.y * _mGyroRangeScale) - _mGyroScale.offsetY) * _mGyroScale.scaleY;
            _mSensors.gyro.z = ((_mDatas.val.gyro.z * _mGyroRangeScale) - _mGyroScale.offsetZ) * _mGyroScale.scaleZ;
        }

        void MPU6000::getMotion6(Motion6f* datas)
        {
            memcpy(datas, &_mSensors, sizeof(Motion6f));
        }

        void MPU6000::getGYRO(Axisf* axis)
        {
            memcpy(axis, &_mSensors.acc, sizeof(Axisf));
        }

        void MPU6000::getACC(Axisf* axis)
        {
            memcpy(axis, &_mSensors.gyro, sizeof(Axisf));
        }

        void MPU6000::getZeroOffset()
        {

            uint16_t cnt = 500;
            float tmpOffset[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
            for (uint16_t i=0;i<cnt;i++) {
                read();
                // Acc
                tmpOffset[0] += _mSensors.acc.x;
                tmpOffset[1] += _mSensors.acc.y;
                tmpOffset[2] += _mSensors.acc.z;

                // Gyro
                tmpOffset[3] += _mSensors.gyro.x;
                tmpOffset[4] += _mSensors.gyro.y;
                tmpOffset[5] += _mSensors.gyro.z;

                pegasus::core::mainTimer.delay(10);
            }

            _mAccScale.offsetX = (float)tmpOffset[0] / 500.0f;
            _mAccScale.offsetY = (float)tmpOffset[1] / 500.0f;
            _mAccScale.offsetZ = (float)tmpOffset[2] / 500.0f;

            _mGyroScale.offsetX = (float)tmpOffset[3] / 500.0f;
            _mGyroScale.offsetY = (float)tmpOffset[4] / 500.0f;
            _mGyroScale.offsetZ = (float)tmpOffset[5] / 500.0f;

            _mOffsetCalculated = true;

        }

        void MPU6000::calibration()
        {
            read();
        }

        /******************************************
         * INT2 DRDY
         ******************************************/
        void MPU6000::interrupt()
        {
            if (_mAutoSampling) {
                read();
                CALLSV(pegasus::fc::service::DRDY_ACCGYRO);
            }
        }

    } /* namespace peripherals */
} /* namespace pegasus */
