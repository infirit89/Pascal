#include "pspch.h"
#include "HttpRequestParser.h"

namespace Pascal 
{
    uint32_t HttpRequestParser::s_MaxUriLength = 9_kb;
    static constexpr char CRLF[] = {"\r\n"};
    static constexpr uint32_t CRLFLen = 2;

    static const char* FindCRLF(const Buffer& messageBuffer) 
    {
        const char* crlf = std::search(messageBuffer.Peek(), 
                                        messageBuffer.GetWritable(), 
                                        CRLF, CRLF + 2);

        return crlf == messageBuffer.GetWritable() ? nullptr : crlf;
    }

    Shared<HttpRequest> HttpRequestParser::ParseRequest(
                                                    Buffer& messageBuffer,
                                                    Status& status) 
    {
        // TODO: error handling

        Shared<HttpRequest> request = CreateShared<HttpRequest>();

        PS_TRACE(messageBuffer);

        // try parse the request method:
        const char* methodSeperator = std::find(
                                        messageBuffer.Peek(),
                                        (const char*)messageBuffer.GetWritable(),
                                        ' ');

        if(methodSeperator == messageBuffer.GetWritable())
        {
            status = Status::IllformedRequest;
            return nullptr;
        }

        std::string method = std::string(messageBuffer.Peek(), methodSeperator);

        if(method == "GET")
            request->m_Method = HttpMethod::Get;
        else if(method == "POST")
            request->m_Method = HttpMethod::Post;
        else if(method == "PUT")
            request->m_Method = HttpMethod::Put;
        else if(method == "DELETE")
            request->m_Method = HttpMethod::Delete;
        else 
        {
            status = Status::UnexpectedMethod;
            return nullptr;
        }
        // -------------------------------

        messageBuffer.Advance(method.size() + 1);

        // try parse the request path:
        const char* pathSeperator = std::find(
                                    messageBuffer.Peek(),
                                    (const char*)messageBuffer.GetWritable(),
                                    ' ');

        if(pathSeperator == messageBuffer.GetWritable())
        {
            status = Status::IllformedRequest;
            return nullptr;
        }

        const char* querySeperator = std::find(
                                            messageBuffer.Peek(),
                                            pathSeperator,
                                            '?');

        if(querySeperator != pathSeperator) 
        {
            request->m_Query = std::string(querySeperator + 1, pathSeperator);
            
            request->m_Target = std::string(
                                        messageBuffer.Peek(), 
                                        pathSeperator - querySeperator - 1);
        }
        else 
        {
            request->m_Target = std::string(
                                        messageBuffer.Peek(), 
                                        pathSeperator);
        }


        if(request->m_Target.size() > s_MaxUriLength) 
        {
            status = Status::URITooLong;
            return nullptr;
        }
        // --------------------------------

        messageBuffer.Advance(pathSeperator - messageBuffer.Peek() + 1);

        // try parse the request version:
        const char* eol = FindCRLF(messageBuffer);

        if(eol == nullptr) 
        {
            status = Status::IllformedRequest;
            return nullptr;
        }

        std::string versionStr = std::string(messageBuffer.Peek(), eol);

        if(versionStr == "HTTP/1.1")
            request->m_Version = HttpVersion::Http11;
        else if(versionStr == "HTTP/1.0")
            request->m_Version = HttpVersion::Http10;
        else 
        {
            status = Status::HttpVersionNotSupported;
            return nullptr;
        }
        // ---------------------------------

        messageBuffer.Advance(versionStr.size() + CRLFLen);

        while(true) 
        {
            eol = FindCRLF(messageBuffer);
            const char* valueSeperator = std::find(
                                                messageBuffer.Peek(),
                                                eol,
                                                ':');
                                                
            if(valueSeperator == eol)
                break;

            std::string header = std::string(messageBuffer.Peek(), valueSeperator);

            std::transform(header.begin(), header.end(), header.begin(), 
                            [](unsigned char c) { return tolower(c); });

            do
                valueSeperator++;
            while(isspace(*valueSeperator));

            std::string value = std::string(valueSeperator, eol);

            request->m_Headers.emplace(header, value);
            
            messageBuffer.Advance((eol - messageBuffer.Peek()) + CRLFLen);
        }


        std::string contentLengthStr = request->GetHeader("content-length");
        if(contentLengthStr != "") 
        {
            messageBuffer.Advance(CRLFLen);
            uint64_t contentLength = std::stoul(contentLengthStr);
            request->m_Body = std::string(messageBuffer.Peek(), contentLength);

        }

        status = Status::Success;

        return request;
    }
}
