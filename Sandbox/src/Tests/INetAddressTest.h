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

    void INetAddressTest2() 
    {
        size_t testCount = 6;

        std::string addresses[] = 
        {
            "2001:db8:c9d2:aee5:73e3:934a:a5ae:9551",
            "2001:ab8:c9d2:a5e5:73e3:934a:2222:9551",
            "2001:d3b8:c9d2:aee5:73e3:934a:a5ee:9551",
            "2001:abb8:c3d2:a4e5:78e3:d3ea:a5a7:9551",
            "2031:5db8:c9d2:aee5:73e3:933a:15ae:9551",
            "2001:ddb8:c2d2:a6e5:79d3:931a:a52e:9551",
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
            inetAddresses.push_back({ addresses[i], ports[i], true });
        }

        for (int i = 0; i < testCount; ++i)
        {
            Pascal::INetAddress inetAddress = inetAddresses.at(i);
            UnitTestUtils::AssertIfNotEqual(inetAddress.GetIp(), addresses[i]);
            UnitTestUtils::AssertIfNotEqual(inetAddress.GetPort(), ports[i]);
        }
    }
}