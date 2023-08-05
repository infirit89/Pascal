#pragma once

#include "Core/Base.h"
#include "INetAddress.h"

namespace Pascal 
{
    class Socket 
    {
    public:
        Socket();
        Socket(ps_socket socket);
        ~Socket();

        void Listen(int backlog = 16);
        void Bind(const INetAddress& address);

        // returns the base socket type and the peer address
        ps_socket Accept(INetAddress& address);

        ps_socket GetHandle() const { return m_Handle; }

        static Socket CreateSocketNonBlocking(uint16_t family);

    private:
        ps_socket m_Handle;
    };
}
