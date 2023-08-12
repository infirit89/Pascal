#pragma once

#include "Core/Base.h"
#include "Core/Poller.h"
#include "Core/EventDescription.h"
#include "Core/EventLoop.h"

#include "Network/Socket.h"

#include "Connection.h"

#include <set>

namespace Pascal 
{
    class HttpServer 
    {
    public:
        HttpServer(const Shared<EventLoop>& eventLoop);
        ~HttpServer();

        void Run();
        
    private:
        void HandleReadPeer(Shared<Connection> connection, const Buffer& buffer);
        void HandleWritePeer(Shared<Connection> connection);
        void HandleClosePeer(Shared<Connection> connection);

        void HandleAccept();
        
        Socket m_ListenerSocket;
        Shared<EventLoop> m_EventLoop;
        Shared<Event> m_ListenerEvent;
        std::string m_RecvStr;
        std::set<std::shared_ptr<Connection>> m_Connections;
    };
}