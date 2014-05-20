/*
 * GPS.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: alienx
 */

#include "fc/include/GPS.h"
#include "core/include/Trace.h"

namespace pegasus {
	namespace fc {

		GPSDatas_t gpsDatas;
		GPSBase* GPS::_mGPS;

		GPS::GPS():
    	    Thread("GPS", [](GPS* p) { p->run();}) {}

		bool GPS::init(GPSBase* periph) {
			pegasus::core::trace.log("[GPS] initialization.");
			GPS::_mGPS = periph;

			return periph->init();
		}

		void GPS::run() {
			pegasus::core::trace.log("[GPS] task started.");

			while (1) {
				GPS::_mGPS->check();

				sleep(1);
			}

			pegasus::core::trace.log("[GPS] task ended.");
		}

		GPS gpsTask;

	} /* namespace fc */
} /* namespace pegasus */
