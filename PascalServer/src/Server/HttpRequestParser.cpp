#include "pspch.h"
#include "HttpRequestParser.h"

namespace Pascal 
{
    uint32_t HttpRequestParser::s_MaxUriLength = 9_kb;

    Shared<HttpRequest> HttpRequestParser::ParseRequest(const Buffer& messageBuffer, Status& status) 
    {
        // TODO: error handling

        std::string message = messageBuffer.GetData();

        size_t methodSeperator = message.find(' ');
        if(methodSeperator == std::string::npos) 
        {
            status = Status::IllformedRequest;
            return nullptr;
        }
        
        std::string method = message.substr(0, methodSeperator);

        HttpMethod requestMethod = HttpMethod::None;

        // TODO: add all the other request methods
        if(method == "GET")
            requestMethod = HttpMethod::Get;
        else if(method == "POST")
            requestMethod = HttpMethod::Post;
        else if(method == "PUT")
            requestMethod = HttpMethod::Put;
        else if(method == "DELETE")
            requestMethod = HttpMethod::Delete;
        else 
        {
            status = Status::UnexpectedMethod;
            return nullptr;
        }

        size_t targetSeperator = message.find(' ', methodSeperator + 1);
        if(targetSeperator == std::string::npos) 
        {
            status = Status::IllformedRequest;
            return nullptr;
        }
        
        std::string target = message.substr(methodSeperator + 1, targetSeperator - methodSeperator - 1);
        if(target.size() > s_MaxUriLength) 
        {
            status = Status::URITooLong;
            return nullptr;
        }

        const char* newLineToken = "\r\n";
        const size_t newLineTokenLen = strlen(newLineToken);

        size_t eol = message.find(newLineToken, targetSeperator + 1);
        if(eol == std::string::npos) 
        {
            status = Status::IllformedRequest;
            return nullptr;
        }

        std::string versionStr = message.substr(targetSeperator + 1, eol - targetSeperator - 1);

        HttpVersion version = HttpVersion::Unknown;

        if(versionStr == "HTTP/1.1")
            version = HttpVersion::Http11;
        else if(versionStr == "HTTP/1.0")
            version = HttpVersion::Http10;
        else 
        {
            status = Status::HttpVersionNotSupported;
            return nullptr;
        }

        const char* headerToken = ":";
        const size_t headerTokenLen = strlen(headerToken);

        size_t valueSeperator = message.find(headerToken, eol);

        Shared<HttpRequest> request = CreateShared<HttpRequest>();
        request->m_Method = requestMethod;
        request->m_Target = target;
        request->m_VersionString = versionStr;
        request->m_Version = version;

        while(valueSeperator != std::string::npos)
        {
            std::string header = message.substr(eol + newLineTokenLen, 
                                                valueSeperator - eol - headerTokenLen - 1);

            std::transform(header.begin(), header.end(), header.begin(), 
                            [](unsigned char c) { return tolower(c); });


            eol = message.find(newLineToken, valueSeperator);
            if(eol == std::string::npos) 
            {
                status = Status::IllformedRequest;
                return nullptr;
            }

            int offset = 0;
            while(isspace(message[valueSeperator + headerTokenLen + offset]))
                offset++;

            std::string value = message.substr(valueSeperator + headerTokenLen + offset, 
                                                    eol - valueSeperator - newLineTokenLen);

            request->m_Headers.emplace(header, value);
            valueSeperator = message.find(headerToken, eol);
        }

        status = Status::Success;

        return request;
    }
}
