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
        m_ListenerEvent->SetReadCallback(std::bind(&HttpServer::HandleAccept, this));
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

    void HttpServer::HandleAccept() 
    {
        INetAddress clientAddress;
        ps_socket clientHandle = m_ListenerSocket.Accept(clientAddress);

        PS_ASSERT(clientHandle != PS_INVALID_SOCKET, "Couldn't accept a valid client");

        std::shared_ptr<Connection> connection = std::make_shared<Connection>(m_EventLoop, clientHandle);

        connection->SetReadCallback(std::bind(&HttpServer::HandleReadPeer, this, std::placeholders::_1, std::placeholders::_2));
        connection->SetCloseCallback(std::bind(&HttpServer::HandleClosePeer, this, std::placeholders::_1));

        m_Connections.insert(connection);

        connection->EstablishConnection();
    }

    void HttpServer::HandleReadPeer(Shared<Connection> connection, const Buffer& buffer) 
    {
        std::string temp = buffer.ReadAll();

        PS_TRACE(temp);

        connection->Send(response, strlen(response));
    }
    
    void HttpServer::HandleWritePeer(Shared<Connection> connection) 
    {
        // PS_TRACE("i shael cum now!");
    }

    void HttpServer::HandleClosePeer(Shared<Connection> connection) 
    {
        m_Connections.erase(connection);
    }

    void HttpServer::Run() 
    {
        m_EventLoop->Run();
    }
}