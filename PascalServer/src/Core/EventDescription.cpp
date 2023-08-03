#include "pspch.h"
#include "EventDescription.h"

#include <poll.h>

namespace Pascal 
{
    int EventDescription::s_ReadEvent = POLLIN | POLLPRI;

    EventDescription::EventDescription(int eventHandle) 
        : m_EventHandle(eventHandle)
    { }

    
    EventDescription::~EventDescription() { }
}
