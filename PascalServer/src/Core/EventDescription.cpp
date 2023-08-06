#include "pspch.h"
#include "EventDescription.h"

#include <poll.h>

namespace Pascal 
{
    int Event::s_ReadEvent = POLLIN | POLLPRI;
    int Event::s_WriteEvent = POLLOUT;

    Event::Event(int eventHandle) 
        : m_EventHandle(eventHandle)
    { }

    
    Event::~Event() { }

    void Event::HandleEvents() 
    {
        if(m_ReturnedEventMask & (POLLNVAL | POLLERR)) 
        {
            if(m_ErrorCallback)
                m_ErrorCallback();
        }

        if((m_ReturnedEventMask & POLLHUP) && !(m_ReturnedEventMask & POLLIN))
        {
            if(m_CloseCallback)
                m_CloseCallback();
        }

        if(m_ReturnedEventMask & (POLLIN | POLLPRI)) 
        {
            if(m_ReadCallback)
                m_ReadCallback();
        }

        if(m_ReturnedEventMask & (POLLOUT)) 
        {
            if(m_WriteCallback)
                m_WriteCallback();
        }
    }
}
