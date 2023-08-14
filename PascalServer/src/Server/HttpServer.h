#pragma once

#include "Core/Base.h"
#include "Core/Poller.h"
#include "Core/EventDescription.h"
#include "Core/EventLoop.h"

#include "Network/Socket.h"

#include "Connection.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

#include <set>
#include <functional>

namespace Pascal 
{
    class HttpServer 
    {
    public:
        using HttpRequestCallback = std::function<Shared<HttpResponse>(const Shared<HttpRequest>&)>;

        HttpServer(const Shared<EventLoop>& eventLoop);
        ~HttpServer();

        void Run();

        void SetHttpMessageCallback(const HttpRequestCallback& callback) 
        {
            m_OnHttpRequest = callback;
        }
        
    private:
        void HandleReadPeer(Shared<Connection> connection, const Buffer& buffer);
        void HandleWritePeer(Shared<Connection> connection);
        void HandleClosePeer(Shared<Connection> connection);
        void SendResponse(const Shared<Connection>& connection, const Shared<HttpResponse>& response);

        void HandleAccept();
        
        Socket m_ListenerSocket;
        Shared<EventLoop> m_EventLoop;
        Shared<Event> m_ListenerEvent;
        std::string m_RecvStr;
        std::set<std::shared_ptr<Connection>> m_Connections;
        HttpRequestCallback m_OnHttpRequest;
    };
}