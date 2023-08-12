#pragma once

#include "Core/Buffer.h"

#include "Server/HttpRequestParser.h"
#include "Server/HttpUtils.h"

#include "TestUtils.h"

namespace UnitTests
{
    void HttpRequestParserTest1() 
    {
        Pascal::Buffer messageBuffer("GET /hello.html HTTP/1.1\r\n" +
                                std::string("User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n") +
                                std::string("Host: www.tutorialspoint.com\r\n") +
                                std::string("Accept-Language: en-us\r\n") +
                                std::string("Accept-Encoding: gzip, deflate\r\n") +
                                std::string("Connection: Keep-Alive\r\n"));

        Pascal::Shared<Pascal::HttpRequest> request = Pascal::HttpRequestParser::ParseRequest(messageBuffer);

        PS_TRACE(messageBuffer);
        PS_TRACE(request);

        Utils::AssertIfNotEqual(request->GetMethod(), Pascal::HttpRequestMethod::Get);
        Utils::AssertIfNotEqual(request->GetTarget(), std::string("/hello.html"));
        Utils::AssertIfNotEqual(request->GetVersion(), std::string("HTTP/1.1"));

        for(const auto& [header, headerData] : request->GetHeaders()) 
            PS_TRACE("{}: {}", header, headerData);
    }
}