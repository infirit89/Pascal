#pragma once

#include "Core/Base.h"
#include "Core/Poller.h"
#include "Core/EventDescription.h"

#include "Network/Socket.h"

namespace Pascal 
{
    class HttpServer 
    {
    public:
        HttpServer();
        ~HttpServer();

        void Run();

    private:
        void CloseListenerSocket();

        Socket m_ListenerSocket;
        Unique<Poller> m_Poller;
        Shared<EventDescription> m_EventDescription;
    };
}