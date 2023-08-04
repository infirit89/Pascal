#include "pspch.h"

#include "INetAddress.h"

#include <arpa/inet.h>

namespace Pascal 
{
    // TODO: decide weather the address is ipv4 or ipv6 based on the address

    INetAddress::INetAddress(std::string address, uint16_t port, bool ipv6) 
        : m_IsIPV6(ipv6)
    {
        if(m_IsIPV6) 
        {
            memset(&m_AddressIP6, 0, sizeof(sockaddr_in6));

            if(inet_pton(AF_INET6, address.c_str(), &(m_AddressIP6.sin6_addr)) <= 0) 
            {
                PS_ERROR("Couldn't convert address to native");
                return;
            }

            m_AddressIP6.sin6_family = AF_INET6;
            m_AddressIP6.sin6_port = htons(port);
        }
        else 
        {
            memset(&m_AddressIP4, 0, sizeof(sockaddr_in));

            if(inet_pton(AF_INET, address.c_str(), &(m_AddressIP4.sin_addr)) <= 0) 
            {
                PS_ERROR("Couldn't convert address to native");
                return;
            }

            m_AddressIP4.sin_family = AF_INET;
            m_AddressIP4.sin_port = htons(port);
        }
    }

    INetAddress::INetAddress(uint16_t port, bool loopback, bool ipv6) 
        : m_IsIPV6(ipv6)
    {
        if(ipv6) 
        {
            m_AddressIP6.sin6_family = AF_INET6;
            m_AddressIP6.sin6_port = htons(port);
            m_AddressIP6.sin6_addr = loopback ? in6addr_loopback : in6addr_any;
        }
        else 
        {
            m_AddressIP4.sin_family = AF_INET;
            m_AddressIP4.sin_port = htons(port);
            m_AddressIP4.sin_addr.s_addr = loopback ? INADDR_LOOPBACK : INADDR_ANY;
        }
    }

    std::string INetAddress::GetIp() const 
    {   
        if(m_IsIPV6) 
        {
            char buf[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &m_AddressIP6.sin6_addr, buf, sizeof(buf));
            return buf;
        }
        else 
        {
            char buf[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &m_AddressIP4.sin_addr, buf, sizeof(buf));
            return buf;
        }

        return "";
    }

    uint16_t INetAddress::GetPort() const 
    {
        return ntohs(m_AddressIP4.sin_port);
    }
}
