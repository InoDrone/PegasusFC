/*
 * L3GD20.cpp
 *
 *  Created on: 29 nov. 2013
 *      Author: alienx
 */

#include "L3GD20.h"
#include "core/include/Trace.h"

namespace pegasus
{
    namespace peripherals
    {

        L3GD20::L3GD20(pegasus::hal::SpiDeviceBase_t* spiDevice) :
            _mSpi(spiDevice)
        {
            values[0] = 0;
            values[1] = 0;
            values[2] = 0;
            // Init Device
        }

        bool L3GD20::init()
        {
            pegasus::core::trace.log("[L3GD20] Initialization");
            uint8_t ctrl1 = 0x0;
            uint8_t ctrl4 = 0x0;
            uint8_t ctrl3 = 0x0;

            pegasus::hal::Gpio int2Pin(pegasus::gpio::A, pegasus::gpio::PIN2);
            pegasus::hal::InterruptRegister::attachExternalInt(this, &int2Pin);

            //b00111111
            ctrl1 |= 0x07; // Enable X,Y,Z axis
            ctrl1 |= 0x08; // Power mode Active
            ctrl1 |= 0xF0; // DR / BW (ODR 760Hz, cutoff 100)

            ctrl4 = 0x10; // 500 dps

            _mSpi->write(&ctrl1, L3GD20_CTRL_REG1_ADDR, 1);
            _mSpi->write(&ctrl4, L3GD20_CTRL_REG4_ADDR, 1);

            _mSpi->read(&ctrl3, L3GD20_CTRL_REG3_ADDR, 1);
            ctrl3 &= 0xF7;
            ctrl3 |= 0x08;
            _mSpi->write(&ctrl3, L3GD20_CTRL_REG3_ADDR, 1);


            read();
            pegasus::core::trace.log("[L3GD20] Initialization done");

            return true;
        }

        void L3GD20::read()
        {
            uint8_t buffer[6] = {0};
            uint8_t i;

            _mSpi->read(buffer, L3GD20_OUT_X_L_ADDR, 6);
            for (i=0;i<3;i++) {
                values[i] = (uint16_t)(((uint16_t)buffer[2*i] << 8) + buffer[2*i+1]);
            }
        }

        void L3GD20::getMotion6(Motion6f* datas)
        {
        }

        void L3GD20::getGYRO(Axisf* axis)
        {
        }

        void L3GD20::getACC(Axisf* axis)
        {
        }

        void L3GD20::getZeroOffset()
        {

        }

        void L3GD20::calibration()
        {

        }

        /******************************************
         * INT2 DRDY
         ******************************************/
        void L3GD20::interrupt()
        {
            read();
            //pegasus::hal::InterruptRegister::callService(pegasus::fc::service::ACCGYRO_UPDATE);
        }

    } /* namespace peripherals */
} /* namespace pegasus */
