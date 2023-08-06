#include "pspch.h"
#include "Socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Pascal 
{
    Socket::Socket() 
        : m_Handle(PS_INVALID_SOCKET) { }

    Socket::Socket(ps_socket socket)
        : m_Handle(socket) { }

    Socket::~Socket() 
    {
        if(m_Handle != PS_INVALID_SOCKET)
        {
            PS_TRACE("closing socket: {0}", m_Handle);
            close(m_Handle);
        }
    }

    void Socket::SetReuseAddress(bool enable) 
    {
        int temp = enable ? 1 : 0;

        setsockopt(m_Handle, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int));
    }

    void Socket::SetReusePort(bool enable) 
    {
        int temp = enable ? 1 : 0;
        setsockopt(m_Handle, SOL_SOCKET, SO_REUSEPORT, &temp, sizeof(int));
    }

    void Socket::Listen(int backlog)
    {
        PS_ASSERT(m_Handle != PS_INVALID_SOCKET, "Cant start listening on an invalid socket");

        int res = listen(m_Handle, backlog);

        PS_ASSERT(res == 0, "Cant start listening");
    }

    void Socket::Bind(const INetAddress& address) 
    {
        PS_ASSERT(m_Handle != PS_INVALID_SOCKET, "Cant bind an invalid socket");

        int res = 0;
        if(address.IsIPV6()) 
            res = bind(m_Handle, address.GetSocketAddress(), sizeof(sockaddr_in6));
        else
            res = bind(m_Handle, address.GetSocketAddress(), sizeof(sockaddr_in));

        PS_ASSERT(res == 0, "Couldn't bind socket {0}", errno);
    }

    // TODO: make it choose the type of sockaddr based on the family?
    ps_socket Socket::Accept(INetAddress& address) 
    {   
        PS_ASSERT(m_Handle != PS_INVALID_SOCKET, "Cant bind an invalid socket");

        sockaddr_in6 addrIn;
        socklen_t addrInSize = sizeof(sockaddr_in6);

        ps_socket client = accept(m_Handle, (sockaddr*)&addrIn, &addrInSize);
        
        PS_ASSERT(client != PS_INVALID_SOCKET, "Invalid client");

        address = INetAddress(addrIn);

        return client;
    }

    static void SetNonBlocking(ps_socket socket) 
    {
        int settings = fcntl(socket, F_GETFL);
        settings = settings | O_NONBLOCK;

        fcntl(socket, F_SETFL, settings);
    }

    Socket Socket::CreateSocketNonBlocking(uint16_t family) 
    {
        ps_socket socket = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
        PS_ASSERT(socket != PS_INVALID_SOCKET, "Couldn't create tcp socket");
        SetNonBlocking(socket);
        return socket;
    }
}
