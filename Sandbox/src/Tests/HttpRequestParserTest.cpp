#include "HttpRequestParserTest.h"

#include "Core/Buffer.h"

#include "Server/HttpRequestParser.h"
#include "Server/HttpUtils.h"

namespace UnitTests 
{
    using namespace Pascal;

    void HttpRequestParserTest1() 
    {
        Buffer messageBuffer("GET /hello.html HTTP/1.1\r\n" +
                                std::string("User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n") +
                                std::string("Host: www.tutorialspoint.com\r\n") +
                                std::string("Accept-Language: en-us\r\n") +
                                std::string("Accept-Encoding: gzip, deflate\r\n") +
                                std::string("Connection: Keep-Alive\r\n"));

        HttpRequestParser::Status status;
        Shared<HttpRequest> request = HttpRequestParser::ParseRequest(messageBuffer, status);

        PS_TRACE(messageBuffer);
        PS_TRACE(request);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::Success);
        Utils::AssertIfNotEqual(request->GetMethod(), HttpMethod::Get);
        Utils::AssertIfNotEqual(request->GetTarget(), std::string("/hello.html"));
        Utils::AssertIfNotEqual((int)request->GetVersion(), (int)HttpVersion::Http11);

        for(const auto& [header, headerData] : request->GetHeaders()) 
            PS_TRACE("{}: {}", header, headerData);
    }

    void HttpRequestParserTest2() 
    {
        // miss spelled request method test:
        Buffer messageBuffer("GE /hello.html HTTP/1.1\r\n");

        HttpRequestParser::Status status;
        Shared<HttpRequest> request = HttpRequestParser::ParseRequest(messageBuffer, status);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::UnexpectedMethod);

        // missing request method test:
        messageBuffer = Buffer("/hello.html HTTP/1.1\r\n");

        request = HttpRequestParser::ParseRequest(messageBuffer, status);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::UnexpectedMethod);

        // someone had a stroke request method test:
        messageBuffer = Buffer("CUM /hello.html HTTP/1.1\r\n");

        request = HttpRequestParser::ParseRequest(messageBuffer, status);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::UnexpectedMethod);
    }

    void HttpRequestParserTest3() 
    {
        // url too long test:
        Buffer messageBuffer("GET /hello.html HTTP/1.1\r\n");

        // set to 4 just to test
        HttpRequestParser::SetMaxURILength(4);
        HttpRequestParser::Status status;
        Shared<HttpRequest> request = HttpRequestParser::ParseRequest(messageBuffer, status);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::URITooLong);


        messageBuffer = Buffer("GET /hello.htmlHTTP/1.1\r\n");

        HttpRequestParser::SetMaxURILength(1024);

        request = HttpRequestParser::ParseRequest(messageBuffer, status);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::IllformedRequest);
    }

    void HttpRequestParserTest4() 
    {
        // unsupported http version test:
        Buffer messageBuffer("GET /hello.html HTTP/.1\r\n");

        HttpRequestParser::Status status;
        Shared<HttpRequest> request = HttpRequestParser::ParseRequest(messageBuffer, status);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::HttpVersionNotSupported);

        // missing crlf test:
        messageBuffer = Buffer("GET /hello.html HTTP/1.1\r");
        request = HttpRequestParser::ParseRequest(messageBuffer, status);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::IllformedRequest);

        messageBuffer = Buffer("GET /hello.html HTTP/1.1\r\n");
        request = HttpRequestParser::ParseRequest(messageBuffer, status);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::Success);
    }

    void HttpRequestParserTest5() 
    {
        Buffer messageBuffer("GET /hello.html HTTP/1.1\r\n" +
                                std::string("User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n") +
                                std::string("Host: www.tutorialspoint.com\r\n") +
                                std::string("Accept-Language: en-us\r\n") +
                                std::string("Accept-Encoding:gzip, deflate\r\n") +
                                std::string("Connection: Keep-Alive\r\n"));

        HttpRequestParser::Status status;
        Shared<HttpRequest> request = HttpRequestParser::ParseRequest(messageBuffer, status);

        Utils::AssertIfNotEqual((int)status, (int)HttpRequestParser::Status::Success);

    }
}
