#pragma once

#include "Core/Base.h"

#include "Poller.h"

#include <atomic>
#include <mutex>
#include <deque>
#include <functional>

namespace Pascal 
{
    class EventLoop 
    {
    public:
        using PostPollFunction = std::function<void()>;

        EventLoop(int timeout = 10000);
        ~EventLoop();

        void AddEventDescription(Event* eventDescription);
        void RemoveEventDescription(Event* eventDescription);

        void Run();

        void Quit();

        bool IsRunning() { return !m_Quit.load(std::memory_order_acquire); }

        void QueuePostPollFunction(PostPollFunction postPollFunction);

    private:
        void WakeUp();

        Poller::EventList m_Eventlist; 
        Shared<Poller> m_Poller;
        int m_Timeout;
        std::atomic_bool m_Quit;

        std::mutex m_Mutex;
        std::deque<PostPollFunction> m_PostPollFunctions;
        int m_WakeUpFd;
        Shared<Event> m_WakeUpEvent;
    };
}
