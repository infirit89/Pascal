#include "pspch.h"
#include "EPollPoller.h"

#include <sys/epoll.h>
#include <poll.h>

namespace Pascal 
{
    #define EVENT_COUNT 16

    EPollPoller::EPollPoller() 
        : m_EpollFd(epoll_create1(EPOLL_CLOEXEC))
    {
        m_Events.resize(EVENT_COUNT);
    }

    EPollPoller::~EPollPoller() 
    {
        close(m_EpollFd);
    }

    void EPollPoller::Poll(EventList& eventDescriptions, int timeout) 
    {
        int eventCount = epoll_wait(m_EpollFd, &m_Events[0], m_Events.capacity(), timeout);

        if(eventCount > 0) 
        {
            FillEventDescriptionList(eventCount, eventDescriptions);
            if(eventCount == m_Events.size())
                m_Events.resize(m_Events.size() * 2);
        }
    }

    void EPollPoller::FillEventDescriptionList(int eventCount, EventList& eventDescriptions) 
    {
        for(int i = 0; i < eventCount; ++i) 
        {
            epoll_event event = m_Events[i];
            Event* eventDescription = static_cast<Event*>(event.data.ptr);
            int eventHandle = eventDescription->GetEventHandle();

            PS_ASSERT(m_ChannelMap.find(eventHandle) == m_ChannelMap.end(), "Event description doesn't exist");

            eventDescription->SetReturnedEventMask(event.events);
            eventDescriptions.push_back(eventDescription);
        }
    }

    void EPollPoller::AddEventDescription(Event* eventDescription) 
    {
        int eventHandle = eventDescription->GetEventHandle();

        if(m_ChannelMap.find(eventHandle) != m_ChannelMap.end()) 
        {
            UpdateEventDescription(EPOLL_CTL_MOD, eventDescription);
            return;
        }

        UpdateEventDescription(EPOLL_CTL_ADD, eventDescription);
    }

    void EPollPoller::UpdateEventDescription(int operation, Event* eventDescription) 
    {
        int eventHandle = eventDescription->GetEventHandle();
        epoll_event ev;
        ev.events = eventDescription->GetEventMask();
        ev.data.ptr = eventDescription;
        epoll_ctl(m_EpollFd, operation, eventHandle, &ev);
    }

    void EPollPoller::RemoveEventDescription(Event* eventDesription) 
    {
        int eventHandle = eventDesription->GetEventHandle();

        if(m_ChannelMap.find(eventHandle) != m_ChannelMap.end())
            return;

        UpdateEventDescription(EPOLL_CTL_DEL, eventDesription);
    }
}
