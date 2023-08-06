#pragma once

#include "Core/Poller.h"

#include <unordered_map>

struct epoll_event;

namespace Pascal
{
    class EPollPoller : public Poller
    {
    public:
        EPollPoller();
        virtual ~EPollPoller() override;

        virtual void Poll(EventList& eventDescriptions, int timeout) override;
        virtual void AddEventDescription(Event* eventDescription) override;
        virtual void RemoveEventDescription(Event* eventDesription) override;

    private:
        void FillEventDescriptionList(int eventCount, EventList& eventDescriptions);
        void UpdateEventDescription(int operation, Event* eventDescription);

        int m_EpollFd;
        std::vector<epoll_event> m_Events;
        std::unordered_map<int, Event*> m_ChannelMap;
    };  
} 