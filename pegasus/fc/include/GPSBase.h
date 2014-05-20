/*
 * GPSBase.h
 *
 *  Created on: 17 avr. 2014
 *      Author: alienx
 */

#ifndef GPSBASE_H_
#define GPSBASE_H_


#include <inttypes.h>

namespace pegasus {
    namespace fc {

        class GPSBase {
                public:

                virtual bool init() = 0;
                virtual uint8_t check() = 0;
        };
    }
}


#endif /* GPSBASE_H_ */
