#include "pspch.h"
#include "HttpRequestParser.h"

namespace Pascal 
{
    Shared<HttpRequest> HttpRequestParser::ParseRequest(const Buffer& messageBuffer) 
    {
        // TODO: error handling

        std::string message = messageBuffer.GetData();

        size_t methodSeperator = message.find(' ');
        if(methodSeperator == std::string::npos)
            return nullptr;
        
        std::string method = message.substr(0, methodSeperator);

        HttpRequestMethod requestMethod = HttpRequestMethod::None;

        // TODO: add all the other request methods
        if(method == "GET")
            requestMethod = HttpRequestMethod::Get;
        else if(method == "POST")
            requestMethod = HttpRequestMethod::Post;
        else if(method == "PUT")
            requestMethod = HttpRequestMethod::Put;
        else if(method == "DELETE")
            requestMethod = HttpRequestMethod::Delete;
        else
            return nullptr;

        size_t targetSeperator = message.find(' ', methodSeperator + 1);
        if(targetSeperator == std::string::npos)
            return nullptr;
        
        std::string target = message.substr(methodSeperator + 1, targetSeperator - methodSeperator - 1);

        const char* newLineToken = "\r\n";
        const size_t newLineTokenLen = strlen(newLineToken);

        size_t eol = message.find(newLineToken, targetSeperator + 1);
        if(eol == std::string::npos)
            return nullptr;

        std::string version = message.substr(targetSeperator + 1, eol - targetSeperator - 1);

        size_t spaceSeperator = message.find(' ', eol);

        Shared<HttpRequest> request = CreateShared<HttpRequest>();
        request->m_Method = requestMethod;
        request->m_Target = target;
        request->m_VersionString = version;

        while(spaceSeperator != std::string::npos) 
        {
            std::string header = message.substr(eol + newLineTokenLen, 
                                                spaceSeperator - eol - newLineTokenLen - 1);
            eol = message.find(newLineToken, spaceSeperator);
            std::string headerData = message.substr(spaceSeperator + 1, 
                                                    eol - spaceSeperator - 1);

            request->m_Headers.emplace(header, headerData);
            spaceSeperator = message.find(' ', eol);
        }

        return request;
    }
}