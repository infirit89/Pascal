#pragma once

#include "EventDescription.h"
#include "Base.h"

#include <vector>

namespace Pascal 
{
    class Poller 
    {
    public:
        using EventDescriptionList = std::vector<EventDescription*>;

        static Unique<Poller> CreatePoller();

        Poller() { }
        virtual ~Poller() {}

        virtual void Poll(EventDescriptionList& eventDescriptions, int timeout) = 0;
        virtual void AddEventDescription(EventDescription* eventDescription) = 0;
        virtual void RemoveEventDescription(EventDescription* eventDesription) = 0;
    };
}