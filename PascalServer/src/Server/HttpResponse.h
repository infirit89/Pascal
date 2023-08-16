#pragma once 

#include "HttpTypes.h"

#include <unordered_map>

namespace Pascal 
{
    class HttpResponse 
    {
    public:
        HttpResponse(HttpStatus status) 
            : m_Status(status), m_ContentLength(0) { }

        HttpResponse(const std::string& body, HttpStatus status) 
            : m_Status(status), m_Body(body), m_ContentLength(body.size()) { }

        HttpResponse() 
            : m_Status(HttpStatus::Success), m_ContentLength(0) { }

        ~HttpResponse() {}

        void SetStatus(HttpStatus status) 
        {
            m_Status = status;
        }

        void AddHeader(const std::string& header, const std::string& headerData) 
        {
            m_Headers.emplace(header, headerData);
        }

        void SetBody(const std::string& body, size_t length) 
        {
            m_Body = body;
            m_ContentLength = length;
        }

        void SetBody(const std::string& body) 
        {
            m_Body = body;
            m_ContentLength = body.size();
        }

        const std::string& GetBody() const { return m_Body; }
        const HeaderMap& GetHeaders() const { return m_Headers; }
        size_t GetContentLength() const { return m_ContentLength; }

        void SetContentType(HttpContentType contentType) 
        {
            m_Headers.emplace("Content-Type", HttpContentTypeToString(contentType));
        }

    private:
        // TODO: http version
        HttpStatus m_Status;
        HeaderMap m_Headers;
        std::string m_Body;
        size_t m_ContentLength;

        friend class HttpResponseBuilder;
    };
}