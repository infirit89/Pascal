#pragma once

#include <functional>

namespace Pascal 
{
    class Event 
    {
    public:
        using EventCallback = std::function<void()>;

        Event(int eventHandle);
        ~Event();

        void SetReadCallback(const EventCallback& callback) 
        {
            m_ReadCallback = callback;
        }

        void SetCloseCallback(const EventCallback& callback) 
        {
            m_CloseCallback = callback;
        }

        int GetEventHandle() const { return m_EventHandle; } 

        int GetEventMask() const { return m_EventMask; }

        void EnableReading() 
        {
            m_EventMask |= s_ReadEvent;
        }

        void EnableWriting() 
        {
            m_EventMask |= s_WriteEvent;
        }

        void HandleEvents();

    private:
        static int s_ReadEvent; 
        static int s_WriteEvent;

        void SetReturnedEventMask(int mask) 
        {
            m_ReturnedEventMask = mask;
        }

        EventCallback m_ReadCallback;
        EventCallback m_CloseCallback;
        EventCallback m_ErrorCallback;
        EventCallback m_WriteCallback;

        int m_EventHandle;
        int m_EventMask;

        // the event mask from the polled event
        int m_ReturnedEventMask;

        friend class Poller;
        friend class EPollPoller;
    };
}
