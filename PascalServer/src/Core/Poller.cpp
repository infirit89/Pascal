#include "pspch.h"
#include "Poller.h"

#include "Platform/Pollers/EPollPoller.h"

namespace Pascal 
{
    Unique<Poller> Poller::CreatePoller() 
    {
        #if defined __linux__
        return CreateUnique<EPollPoller>();
        #endif
    }
}