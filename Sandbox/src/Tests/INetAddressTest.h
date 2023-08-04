#pragma once

#include "Network/INetAddress.h" 

#include "TestUtils.h"

namespace UnitTests
{
    // Create a inetadress object with a certain ip, 
    // then get the string respresentaion of that ip back 
    // and compare the result with the original ip parameter

    void INetAddressTest1() 
    {
        size_t testCount = 6;

        std::string addresses[] = 
        {
            "192.168.0.1",
            "192.173.2.12",
            "112.173.2.12",
            "112.173.20.1",
            "112.173.80.1",
            "112.173.0.117",
        };

        uint16_t ports[] = 
        {
            0,
            8080,
            69,
            111,
            5681,
            3400
        };

        std::vector<Pascal::INetAddress> inetAddresses;

        for(int i = 0; i < testCount; ++i) 
        {
            inetAddresses.push_back({ addresses[i], ports[i] });
        }

        for (int i = 0; i < testCount; ++i)
        {
            Pascal::INetAddress inetAddress = inetAddresses.at(i);
            UnitTestUtils::AssertIfNotEqual(inetAddress.GetIp(), addresses[i]);
            UnitTestUtils::AssertIfNotEqual(inetAddress.GetPort(), ports[i]);
        }
    }
}