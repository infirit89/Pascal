#include "pspch.h"
#include "HttpServer.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Pascal 
{
    const char* response = "HTTP/1.1 200 OK\r\n";

    HttpServer::HttpServer()
        : m_ListenerSocket(Socket::CreateSocketNonBlocking(AF_INET))
    {
        m_Poller = Poller::CreatePoller();

        INetAddress address(8080);
        m_ListenerSocket.Bind(address);

        m_EventDescription = CreateShared<EventDescription>(m_ListenerSocket.GetHandle());
    }

    HttpServer::~HttpServer() 
    {
        CloseListenerSocket();
    }  

    void HttpServer::CloseListenerSocket() 
    {
    }

    void HttpServer::Run() 
    {
        m_ListenerSocket.Listen();

        m_EventDescription->EnableReading();
        m_Poller->AddEventDescription(m_EventDescription.get());

        // epoll_event ev;
        // ev.events = EPOLLIN | EPOLLET;
        // ev.data.fd = m_ListenerSocket;

        // epoll_ctl(m_EpollFD, EPOLL_CTL_ADD, m_ListenerSocket, &ev);

        std::string recvStr;
        recvStr.resize(PS_HTTP_REQUEST_INITIAL_SIZE * 2);

        // std::vector<epoll_event> events;
        // events.resize(10);

        while(true)
        {
            std::vector<EventDescription*> events;

            m_Poller->Poll(events, -1);

            for(auto event : events) 
            {
                if(event->GetEventHandle() == m_ListenerSocket.GetHandle()) 
                {
                    INetAddress clientAddress;
                    Socket client = m_ListenerSocket.Accept(clientAddress);

                    if(clientAddress.GetAddressFamily() == AF_INET)
                        PS_TRACE("IPV4");
                    else if(clientAddress.GetAddressFamily() == AF_INET6)
                        PS_TRACE("IPV6");

                    PS_TRACE("Client address: {0}", clientAddress.GetIp());

                    recv(client.GetHandle(), &recvStr[0], recvStr.capacity(), 0);

                    PS_TRACE(recvStr);

                    send(client.GetHandle(), response, strlen(response), 0);

                    // close(client.);
                }
            }

            // int handles = epoll_wait(m_EpollFD, &events[0], events.capacity(), -1);

            // for(auto event : events)
            // {
            //     if(event.data.fd == m_ListenerSocket) 
            //     {
            //         ps_socket client = accept(event.data.fd, NULL, NULL);
            //         SetNonBlocking(client);

            //         ev.events = EPOLLIN | EPOLLET;
            //         ev.data.fd = client;

            //         epoll_ctl(m_EpollFD, EPOLL_CTL_ADD, client, &ev);
            //     }
            //     else 
            //     {
            //         ps_socket client = event.data.fd;
            //         recv(client, &recvStr[0], recvStr.capacity(), 0);

            //         PS_TRACE(recvStr);

            //         send(client, response, strlen(response), 0);

            //         epoll_ctl(m_EpollFD, EPOLL_CTL_DEL, client, &event);
            //         close(client);    
            //     }
            // }
        }
    }
}