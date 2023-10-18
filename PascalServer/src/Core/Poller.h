#pragma once

#include "EventDescription.h"
#include "Base.h"

#include <vector>

namespace Pascal 
{
    class Poller 
    {
    public:
        using EventList = std::vector<Event*>;

        static Unique<Poller> CreatePoller();

        Poller() { }
        virtual ~Poller() {}

        virtual void Poll(EventList& eventDescriptions, int timeout) = 0;
        virtual void AddEventDescription(Event* eventDescription) = 0;
        virtual void RemoveEventDescription(Event* eventDesription) = 0;
    };
}
