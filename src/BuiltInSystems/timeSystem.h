#ifndef _MSCE_TIME_SYSTEM_H_
#define _MSCE_TIME_SYSTEM_H_
#include <ECS/system.h>

namespace msce
{
    class TimeSystem : public msce::System
    {
    private:
        unsigned long _total_milis;
    };
}

#endif