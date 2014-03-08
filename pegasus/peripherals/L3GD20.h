/*
 * L3GD20.h
 *
 *  Created on: 29 nov. 2013
 *      Author: alienx
 */

#ifndef L3GD20_H_
#define L3GD20_H_

#include "fc/include/GyroAccBase.h"
#include "core/include/Pegasus.h"
#include "core/include/Trace.h"
#include "hal/include/InterruptListener.h"


/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
#define L3GD20_WHO_AM_I_ADDR          0x0F  /* device identification register */
#define L3GD20_CTRL_REG1_ADDR         0x20  /* Control register 1 */
#define L3GD20_CTRL_REG2_ADDR         0x21  /* Control register 2 */
#define L3GD20_CTRL_REG3_ADDR         0x22  /* Control register 3 */
#define L3GD20_CTRL_REG4_ADDR         0x23  /* Control register 4 */
#define L3GD20_CTRL_REG5_ADDR         0x24  /* Control register 5 */
#define L3GD20_REFERENCE_REG_ADDR     0x25  /* Reference register */
#define L3GD20_OUT_TEMP_ADDR          0x26  /* Out temp register */
#define L3GD20_STATUS_REG_ADDR        0x27  /* Status register */
#define L3GD20_OUT_X_L_ADDR           0x28  /* Output Register X */
#define L3GD20_OUT_X_H_ADDR           0x29  /* Output Register X */
#define L3GD20_OUT_Y_L_ADDR           0x2A  /* Output Register Y */
#define L3GD20_OUT_Y_H_ADDR           0x2B  /* Output Register Y */
#define L3GD20_OUT_Z_L_ADDR           0x2C  /* Output Register Z */
#define L3GD20_OUT_Z_H_ADDR           0x2D  /* Output Register Z */
#define L3GD20_FIFO_CTRL_REG_ADDR     0x2E  /* Fifo control Register */
#define L3GD20_FIFO_SRC_REG_ADDR      0x2F  /* Fifo src Register */

#define L3GD20_INT1_CFG_ADDR          0x30  /* Interrupt 1 configuration Register */
#define L3GD20_INT1_SRC_ADDR          0x31  /* Interrupt 1 source Register */
#define L3GD20_INT1_TSH_XH_ADDR       0x32  /* Interrupt 1 Threshold X register */
#define L3GD20_INT1_TSH_XL_ADDR       0x33  /* Interrupt 1 Threshold X register */
#define L3GD20_INT1_TSH_YH_ADDR       0x34  /* Interrupt 1 Threshold Y register */
#define L3GD20_INT1_TSH_YL_ADDR       0x35  /* Interrupt 1 Threshold Y register */
#define L3GD20_INT1_TSH_ZH_ADDR       0x36  /* Interrupt 1 Threshold Z register */
#define L3GD20_INT1_TSH_ZL_ADDR       0x37  /* Interrupt 1 Threshold Z register */
#define L3GD20_INT1_DURATION_ADDR     0x38  /* Interrupt 1 DURATION register */

/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/

namespace pegasus
{
    namespace peripherals
    {

        class L3GD20 : public pegasus::fc::GyroAccBase, public pegasus::hal::InterruptListener
        {
            public:
                L3GD20(pegasus::hal::SpiDeviceBase_t* spiDevice);

                void getMotion6(Motion6f*);
                void getGYRO(Axisf*);
                void getACC(Axisf*);

                void interrupt();

                void getZeroOffset();
                void calibration();

            private:
                pegasus::hal::SpiDeviceBase_t* _mSpi;
                uint16_t values[3];

                bool init();
                void read();
        };

    } /* namespace peripherals */
} /* namespace pegasus */

#endif /* L3GD20_H_ */
