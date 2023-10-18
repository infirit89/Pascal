#include "pspch.h"

#include <sys/eventfd.h>

#include "EventLoop.h"

namespace Pascal 
{
    EventLoop::EventLoop(int timeout)
        : m_Timeout(timeout)
    {
        m_Poller = Poller::CreatePoller();

        m_WakeUpFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

        PS_ASSERT(m_WakeUpFd >= 0, "Failed to create wakeup fd");

        m_WakeUpEvent = CreateShared<Event>(m_WakeUpFd);
        m_WakeUpEvent->EnableReading();
        m_Poller->AddEventDescription(m_WakeUpEvent.get());
    }

    EventLoop::~EventLoop() 
    {
        m_WakeUpEvent.reset();
        close(m_WakeUpFd);
    }

    void EventLoop::Quit() 
    {
        m_Quit.store(true, std::memory_order_release);
        WakeUp();
    }

    void EventLoop::AddEventDescription(Event* eventDescription) 
    {
        m_Poller->AddEventDescription(eventDescription);
    }

    void EventLoop::RemoveEventDescription(Event* eventDescription) 
    {
        m_Poller->RemoveEventDescription(eventDescription);
    }

    void EventLoop::WakeUp() 
    {
        uint64_t temp = 1;
        write(m_WakeUpFd, &temp, sizeof(uint64_t));
    }

    void EventLoop::Run() 
    {
        while(!m_Quit.load(std::memory_order_acquire)) 
        {
            m_Eventlist.clear();
            m_Poller->Poll(m_Eventlist, m_Timeout);

            for(auto event : m_Eventlist)
                event->HandleEvents();

            {
                std::scoped_lock lock(m_Mutex);
                while(!m_PostPollFunctions.empty()) 
                {
                    PostPollFunction func = m_PostPollFunctions.front();
                    func();
                    m_PostPollFunctions.pop_front();
                }
            }
        }
    }

    void EventLoop::QueuePostPollFunction(PostPollFunction postPollFunction) 
    {
        std::scoped_lock lock(m_Mutex);

        m_PostPollFunctions.push_back(postPollFunction);

        // PS_TRACE(m_PostPollFunctions.size());
        WakeUp();
    }
}

