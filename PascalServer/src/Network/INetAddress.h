#pragma once

#include <string>

#include <netinet/in.h>
#include <sys/socket.h>

namespace Pascal 
{
    class INetAddress 
    {
    public:
        INetAddress();
        INetAddress(std::string address, uint16_t port, bool ipv6 = false);
        INetAddress(uint16_t port, bool loopback = false, bool ipv6 = false);
        INetAddress(sockaddr_in6 addrin);

        ~INetAddress() { }

        const sockaddr* GetSocketAddress() const 
        {
            return static_cast<const sockaddr*>((void*)(&m_AddressIP6));
        }

        uint16_t GetAddressFamily() { return m_AddressIP4.sin_family; }

        std::string GetIp() const;
        uint16_t GetPort() const;

        bool IsIPV6() const { return m_IsIPV6; }

    private:
        union 
        {
            sockaddr_in m_AddressIP4;
            sockaddr_in6 m_AddressIP6;
        };

        bool m_IsIPV6;
    };
}
