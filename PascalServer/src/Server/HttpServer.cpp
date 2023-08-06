#include "pspch.h"
#include "HttpServer.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

namespace Pascal 
{
    const char* response = "HTTP/1.1 200 OK\r\n";

    HttpServer::HttpServer(const Shared<EventLoop>& eventLoop)
        : m_ListenerSocket(Socket::CreateSocketNonBlocking(AF_INET)),
          m_EventLoop(eventLoop)
    {
        m_ListenerSocket.SetReuseAddress(true);
        m_ListenerSocket.SetReusePort(true);

        INetAddress address(8080);
        m_ListenerSocket.Bind(address);

        m_ListenerEvent = CreateShared<Event>(m_ListenerSocket.GetHandle());
        m_ListenerEvent->SetReadCallback(std::bind(&HttpServer::ReadCallback, this));
        m_RecvStr.resize(PS_HTTP_REQUEST_INITIAL_SIZE * 2);

        m_ListenerSocket.Listen();

        m_ListenerEvent->EnableReading();
        m_EventLoop->AddEventDescription(m_ListenerEvent.get());
    }

    HttpServer::~HttpServer()
    {
        m_EventLoop.reset();
        m_ListenerEvent.reset();
    }  

    void HttpServer::ReadCallback() 
    {
        INetAddress clientAddress;
        Socket client = m_ListenerSocket.Accept(clientAddress);

        if(clientAddress.GetAddressFamily() == AF_INET)
            PS_TRACE("IPV4");
        else if(clientAddress.GetAddressFamily() == AF_INET6)
            PS_TRACE("IPV6");

        PS_TRACE("Client address: {0}", clientAddress.GetIp());

        recv(client.GetHandle(), &m_RecvStr[0], m_RecvStr.capacity(), 0);

        PS_TRACE(m_RecvStr);

        send(client.GetHandle(), response, strlen(response), 0);
    }

    void HttpServer::Run() 
    {
        m_EventLoop->Run();
    }
}