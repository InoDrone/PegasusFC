
#include "pegasus/core/include/IdleThread.h"

namespace pegasus {

    namespace core {

        IdleThread::IdleThread() :
            Thread("IDLE", [](IdleThread* p) { p->run();}) {}

        IdleThread::~IdleThread() {}

        void IdleThread::run()
        {
            while(1) {
                //TODO idle action
            }
        }

    }
}


