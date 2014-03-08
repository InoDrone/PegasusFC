/*
 * MPU6000.h
 *
 *  Created on: 1 déc. 2013
 *      Author: alienx
 */

#ifndef MPU6000_H_
#define MPU6000_H_

#include "fc/include/Defs.h"
#include "fc/include/GyroAccBase.h"
#include "core/include/Pegasus.h"
#include "hal/include/InterruptListener.h"


#define MPU6000_RA_XG_OFFS_TC       0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6000_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6000_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6000_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
#define MPU6000_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
#define MPU6000_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
#define MPU6000_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS
#define MPU6000_RA_XA_OFFS_L_TC     0x07
#define MPU6000_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS
#define MPU6000_RA_YA_OFFS_L_TC     0x09
#define MPU6000_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS
#define MPU6000_RA_ZA_OFFS_L_TC     0x0B
#define MPU6000_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR
#define MPU6000_RA_XG_OFFS_USRL     0x14
#define MPU6000_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR
#define MPU6000_RA_YG_OFFS_USRL     0x16
#define MPU6000_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR
#define MPU6000_RA_ZG_OFFS_USRL     0x18
#define MPU6000_RA_SMPLRT_DIV       0x19
#define MPU6000_RA_CONFIG           0x1A
#define MPU6000_RA_GYRO_CONFIG      0x1B
#define MPU6000_RA_ACCEL_CONFIG     0x1C
#define MPU6000_RA_FF_THR           0x1D
#define MPU6000_RA_FF_DUR           0x1E
#define MPU6000_RA_MOT_THR          0x1F
#define MPU6000_RA_MOT_DUR          0x20
#define MPU6000_RA_ZRMOT_THR        0x21
#define MPU6000_RA_ZRMOT_DUR        0x22
#define MPU6000_RA_FIFO_EN          0x23
#define MPU6000_RA_I2C_MST_CTRL     0x24
#define MPU6000_RA_I2C_SLV0_ADDR    0x25
#define MPU6000_RA_I2C_SLV0_REG     0x26
#define MPU6000_RA_I2C_SLV0_CTRL    0x27
#define MPU6000_RA_I2C_SLV1_ADDR    0x28
#define MPU6000_RA_I2C_SLV1_REG     0x29
#define MPU6000_RA_I2C_SLV1_CTRL    0x2A
#define MPU6000_RA_I2C_SLV2_ADDR    0x2B
#define MPU6000_RA_I2C_SLV2_REG     0x2C
#define MPU6000_RA_I2C_SLV2_CTRL    0x2D
#define MPU6000_RA_I2C_SLV3_ADDR    0x2E
#define MPU6000_RA_I2C_SLV3_REG     0x2F
#define MPU6000_RA_I2C_SLV3_CTRL    0x30
#define MPU6000_RA_I2C_SLV4_ADDR    0x31
#define MPU6000_RA_I2C_SLV4_REG     0x32
#define MPU6000_RA_I2C_SLV4_DO      0x33
#define MPU6000_RA_I2C_SLV4_CTRL    0x34
#define MPU6000_RA_I2C_SLV4_DI      0x35
#define MPU6000_RA_I2C_MST_STATUS   0x36
#define MPU6000_RA_INT_PIN_CFG      0x37
#define MPU6000_RA_INT_ENABLE       0x38
#define MPU6000_RA_DMP_INT_STATUS   0x39
#define MPU6000_RA_INT_STATUS       0x3A
#define MPU6000_RA_ACCEL_XOUT_H     0x3B
#define MPU6000_RA_ACCEL_XOUT_L     0x3C
#define MPU6000_RA_ACCEL_YOUT_H     0x3D
#define MPU6000_RA_ACCEL_YOUT_L     0x3E
#define MPU6000_RA_ACCEL_ZOUT_H     0x3F
#define MPU6000_RA_ACCEL_ZOUT_L     0x40
#define MPU6000_RA_TEMP_OUT_H       0x41
#define MPU6000_RA_TEMP_OUT_L       0x42
#define MPU6000_RA_GYRO_XOUT_H      0x43
#define MPU6000_RA_GYRO_XOUT_L      0x44
#define MPU6000_RA_GYRO_YOUT_H      0x45
#define MPU6000_RA_GYRO_YOUT_L      0x46
#define MPU6000_RA_GYRO_ZOUT_H      0x47
#define MPU6000_RA_GYRO_ZOUT_L      0x48
#define MPU6000_RA_EXT_SENS_DATA_00 0x49
#define MPU6000_RA_EXT_SENS_DATA_01 0x4A
#define MPU6000_RA_EXT_SENS_DATA_02 0x4B
#define MPU6000_RA_EXT_SENS_DATA_03 0x4C
#define MPU6000_RA_EXT_SENS_DATA_04 0x4D
#define MPU6000_RA_EXT_SENS_DATA_05 0x4E
#define MPU6000_RA_EXT_SENS_DATA_06 0x4F
#define MPU6000_RA_EXT_SENS_DATA_07 0x50
#define MPU6000_RA_EXT_SENS_DATA_08 0x51
#define MPU6000_RA_EXT_SENS_DATA_09 0x52
#define MPU6000_RA_EXT_SENS_DATA_10 0x53
#define MPU6000_RA_EXT_SENS_DATA_11 0x54
#define MPU6000_RA_EXT_SENS_DATA_12 0x55
#define MPU6000_RA_EXT_SENS_DATA_13 0x56
#define MPU6000_RA_EXT_SENS_DATA_14 0x57
#define MPU6000_RA_EXT_SENS_DATA_15 0x58
#define MPU6000_RA_EXT_SENS_DATA_16 0x59
#define MPU6000_RA_EXT_SENS_DATA_17 0x5A
#define MPU6000_RA_EXT_SENS_DATA_18 0x5B
#define MPU6000_RA_EXT_SENS_DATA_19 0x5C
#define MPU6000_RA_EXT_SENS_DATA_20 0x5D
#define MPU6000_RA_EXT_SENS_DATA_21 0x5E
#define MPU6000_RA_EXT_SENS_DATA_22 0x5F
#define MPU6000_RA_EXT_SENS_DATA_23 0x60
#define MPU6000_RA_MOT_DETECT_STATUS    0x61
#define MPU6000_RA_I2C_SLV0_DO      0x63
#define MPU6000_RA_I2C_SLV1_DO      0x64
#define MPU6000_RA_I2C_SLV2_DO      0x65
#define MPU6000_RA_I2C_SLV3_DO      0x66
#define MPU6000_RA_I2C_MST_DELAY_CTRL   0x67
#define MPU6000_RA_SIGNAL_PATH_RESET    0x68
#define MPU6000_RA_MOT_DETECT_CTRL      0x69
#define MPU6000_RA_USER_CTRL        0x6A
#define MPU6000_RA_PWR_MGMT_1       0x6B
#define MPU6000_RA_PWR_MGMT_2       0x6C
#define MPU6000_RA_BANK_SEL         0x6D
#define MPU6000_RA_MEM_START_ADDR   0x6E
#define MPU6000_RA_MEM_R_W          0x6F
#define MPU6000_RA_DMP_CFG_1        0x70
#define MPU6000_RA_DMP_CFG_2        0x71
#define MPU6000_RA_FIFO_COUNTH      0x72
#define MPU6000_RA_FIFO_COUNTL      0x73
#define MPU6000_RA_FIFO_R_W         0x74
#define MPU6000_RA_WHO_AM_I         0x75
#define MPU6000_RA_PRODUCT_ID       0x0C

#define MPU6000_ACCEL_FS_2          0x00
#define MPU6000_ACCEL_FS_4          0x01
#define MPU6000_ACCEL_FS_8          0x02
#define MPU6000_ACCEL_FS_16         0x03

#define MPU6000_GYRO_FS_250         0x00
#define MPU6000_GYRO_FS_500         0x01
#define MPU6000_GYRO_FS_1000        0x02
#define MPU6000_GYRO_FS_2000        0x03

#define MPU6000_WHO_AM_I_BIT        6
#define MPU6000_WHO_AM_I_LENGTH     6

#define MPU6000_PWR1_CLKSEL_BIT         2
#define MPU6000_PWR1_CLKSEL_LENGTH      3

#define MPU6000_CLOCK_INTERNAL          0x00
#define MPU6000_CLOCK_PLL_XGYRO         0x01
#define MPU6000_CLOCK_PLL_YGYRO         0x02
#define MPU6000_CLOCK_PLL_ZGYRO         0x03
#define MPU6000_CLOCK_PLL_EXT32K        0x04
#define MPU6000_CLOCK_PLL_EXT19M        0x05
#define MPU6000_CLOCK_KEEP_RESET        0x07

#define MPU6000_DLPF_BW_256         0x00
#define MPU6000_DLPF_BW_188         0x01
#define MPU6000_DLPF_BW_98          0x02
#define MPU6000_DLPF_BW_42          0x03
#define MPU6000_DLPF_BW_20          0x04
#define MPU6000_DLPF_BW_10          0x05
#define MPU6000_DLPF_BW_5           0x06

                                            // Product ID Description for MPU6000
                                            // high 4 bits  low 4 bits
                                            // Product Name Product Revision
#define MPU6000ES_REV_C4            0x14    // 0001         0100
#define MPU6000ES_REV_C5            0x15    // 0001         0101
#define MPU6000ES_REV_D6            0x16    // 0001         0110
#define MPU6000ES_REV_D7            0x17    // 0001         0111
#define MPU6000ES_REV_D8            0x18    // 0001         1000
#define MPU6000_REV_C4              0x54    // 0101         0100
#define MPU6000_REV_C5              0x55    // 0101         0101
#define MPU6000_REV_D6              0x56    // 0101         0110
#define MPU6000_REV_D7              0x57    // 0101         0111
#define MPU6000_REV_D8              0x58    // 0101         1000
#define MPU6000_REV_D9              0x59    // 0101         1001

#define MPU6000_ACC_SENS_2G         16384.0f
#define MPU6000_ACC_SENS_4G         8192.0f
#define MPU6000_ACC_SENS_8G         4096.0f
#define MPU6000_ACC_SENS_16G        2048.0f

#define MPU6000_GYRO_SENS_250       131.0f
#define MPU6000_GYRO_SENS_500       65.5f
#define MPU6000_GYRO_SENS_1000      32.8f
#define MPU6000_GYRO_SENS_2000      16.4f

namespace pegasus
{
    namespace peripherals
    {

        class MPU6000 : public pegasus::fc::GyroAccBase, public pegasus::hal::InterruptListener
        {
            public:
                MPU6000(pegasus::hal::SpiDeviceBase_t* spiDevice);
                bool init();

                void getMotion6(Motion6f*);
                void getGYRO(Axisf*);
                void getACC(Axisf*);

                void interrupt();


                void calibration();

            private:
                pegasus::hal::SpiDeviceBase_t* _mSpi;
                float _mAccRangeScale;
                float _mGyroRangeScale;
                float _mGyroRangeScaleRadSec;
                bool _mOffsetCalculated;

                void read();
                void getZeroOffset();

                void setAutoSampling(bool);

                union uDatas6 {
                     uint8_t  bytes[];
                     uint16_t word[];
                     struct {
                             Axis acc;
                             int16_t temp;
                             Axis gyro;
                     } val;
                };

                accScale_t  _mAccScale;
                gyroScale_t  _mGyroScale;
                Motion6f _mSensors;
                uDatas6 _mDatas;
                volatile uDatas6* _pmDatas;
        };

    } /* namespace peripherals */
} /* namespace pegasus */

#endif /* MPU6000_H_ */
