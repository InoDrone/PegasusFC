/*
 * GPS.h
 *
 *  Created on: 17 avr. 2014
 *      Author: alienx
 */

#ifndef GPS_H_
#define GPS_H_

#include "core/include/Thread.h"
#include "fc/include/GPSBase.h"

#define GPS_NOFIX 	0
#define GPS_FIX2D 	1
#define GPS_FIX3D 	2

namespace pegasus {
	namespace fc {

		using namespace pegasus::core;

		extern GPSDatas_t gpsDatas;

		class GPS  : public Thread {
			public:
				GPS();
				bool init(GPSBase* periph);

		    protected:
		        void run();

		    private:
		        static GPSBase* _mGPS;

		};

		extern GPS gpsTask;

	} /* namespace fc */
} /* namespace pegasus */

#endif /* GPS_H_ */
