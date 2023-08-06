#pragma once

#include "Core/Base.h"
#include "Core/Poller.h"
#include "Core/EventDescription.h"
#include "Core/EventLoop.h"

#include "Network/Socket.h"

namespace Pascal 
{
    class HttpServer 
    {
    public:
        HttpServer(const Shared<EventLoop>& eventLoop);
        ~HttpServer();

        void Run();
        
    private:
        void ReadCallback();
        void CloseListenerSocket();

        Socket m_ListenerSocket;
        Shared<EventLoop> m_EventLoop;
        Shared<Event> m_ListenerEvent;
        std::string m_RecvStr;
    };
}