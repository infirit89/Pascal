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

        virtual void Poll(EventDescriptionList& eventDescriptions, int timeout) override;
        virtual void AddEventDescription(EventDescription* eventDescription) override;
        virtual void RemoveEventDescription(EventDescription* eventDesription) override;

    private:
        void FillEventDescriptionList(int eventCount, EventDescriptionList& eventDescriptions);
        void UpdateEventDescription(int operation, EventDescription* eventDescription);

        int m_EpollFd;
        std::vector<epoll_event> m_Events;
        std::unordered_map<int, EventDescription*> m_ChannelMap;
    };  
} 