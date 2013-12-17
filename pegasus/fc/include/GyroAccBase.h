/*
 * GyroAccBase.h
 *
 *  Created on: 29 nov. 2013
 *      Author: alienx
 */

#ifndef GYROACCBASE_H_
#define GYROACCBASE_H_


namespace pegasus {
    namespace fc {

        class GyroAccBase {
            public:
                GyroAccBase() {};

                virtual void init() = 0;
                virtual void getMotion6() = 0;
                virtual void getGYRO() = 0;
                virtual void getACC() = 0;

                virtual void getZeroOffset() = 0;
                virtual void calibration() = 0;
        };
    }
}


#endif /* GYROACCBASE_H_ */
