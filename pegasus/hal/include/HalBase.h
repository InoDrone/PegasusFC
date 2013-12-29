/*
 * CoreBase.h
 *
 *  Created on: 24 nov. 2013
 *      Author: alienx
 */

#ifndef HALBASE_H_
#define HALBASE_H_

namespace pegasus {
    namespace hal {

        class HalBase {
            public:
               static void init();
               static void reset();
        };
    }
}



#endif /* COREBASE_H_ */
