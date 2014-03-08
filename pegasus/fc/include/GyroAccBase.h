/*
 * GyroAccBase.h
 *
 *  Created on: 29 nov. 2013
 *      Author: alienx
 */

#ifndef GYROACCBASE_H_
#define GYROACCBASE_H_

#include <stdint.h>
#include "fc/include/Defs.h"

namespace pegasus {
    namespace fc {

        class GyroAccBase {
            public:
                GyroAccBase() :
                    _mAutoSampling(0) {};

                virtual bool init() = 0;
                virtual void getMotion6(Motion6f*) = 0;
                virtual void getGYRO(Axisf*) = 0;
                virtual void getACC(Axisf*) = 0;

                virtual void calibration() = 0;
                void setAutoSampling(bool f) {
                    _mAutoSampling = f;
                }

                struct accScale_t {
                        float offsetX;
                        float scaleX;
                        float offsetY;
                        float scaleY;
                        float offsetZ;
                        float scaleZ;
                };

                struct gyroScale_t {
                        float offsetX;
                        float scaleX;
                        float offsetY;
                        float scaleY;
                        float offsetZ;
                        float scaleZ;
                };

            protected:
                bool _mAutoSampling;


        };
    }
}


#endif /* GYROACCBASE_H_ */
