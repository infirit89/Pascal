#include "pspch.h"
#include "HttpServer.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace Pascal 
{
    const char* response = "HTTP/1.1 200 OK\r\n";

    static void SetNonBlocking(ps_socket socket) 
    {
        int settings = fcntl(socket, F_GETFL);
        settings = settings | O_NONBLOCK;

        fcntl(socket, F_SETFL, settings);
    }

    HttpServer::HttpServer() 
    {
        m_Poller = Poller::CreatePoller();

        m_ListenerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(m_ListenerSocket == PS_INVALID_SOCKET)
            PS_ERROR("i cum");

        SetNonBlocking(m_ListenerSocket);

        PS_ASSERT(m_ListenerSocket != PS_INVALID_SOCKET, "Couldn't create tcp socket");

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);

        addr.sin_addr.s_addr = INADDR_ANY;

        if(bind(m_ListenerSocket, (sockaddr*)&addr, sizeof(sockaddr)) == -1) 
        {
            CloseListenerSocket();
            PS_ASSERT(false, "Couldn't bind socket");
        }

        m_EventDescription = CreateShared<EventDescription>(m_ListenerSocket);
    }

    HttpServer::~HttpServer() 
    {
        CloseListenerSocket(); 
    }  

    void HttpServer::CloseListenerSocket() 
    {
        close(m_ListenerSocket);
    }

    void HttpServer::Run() 
    {
        if(listen(m_ListenerSocket, 16) == -1) 
        {
            CloseListenerSocket();
            PS_ASSERT(false, "Couldn't bind socket");
        }

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
                if(event->GetEventHandle() == m_ListenerSocket) 
                {
                    ps_socket client = accept(m_ListenerSocket, NULL, NULL);

                    recv(client, &recvStr[0], recvStr.capacity(), 0);

                    PS_TRACE(recvStr);

                    send(client, response, strlen(response), 0);

                    close(client);
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