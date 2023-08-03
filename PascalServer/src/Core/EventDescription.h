#pragma once

#include <functional>

namespace Pascal 
{
    class EventDescription 
    {
    public:
        using EventCallback = std::function<void()>;

        EventDescription(int eventHandle);
        ~EventDescription();

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

    private:
        static int s_ReadEvent; 

        EventCallback m_ReadCallback;
        EventCallback m_CloseCallback;
        int m_EventHandle;
        int m_EventMask;
    };
}
