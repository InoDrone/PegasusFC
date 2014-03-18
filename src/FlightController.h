/*
 * FlightController.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef FLIGHTCONTROLLER_H_
#define FLIGHTCONTROLLER_H_

#include "core/include/Thread.h"

using namespace pegasus::core;

class FlightController : public Thread
{
    public:
        FlightController();
        ~FlightController();

        static void waitingMode();
        static void flightMode();

    protected:
        void run();

    private:
       static pegasus::Func _pFunc;
};

extern FlightController fc;

#endif /* FLIGHTCONTROLLER_H_ */
