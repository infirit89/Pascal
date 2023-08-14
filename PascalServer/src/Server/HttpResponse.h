#pragma once 

#include "HttpUtils.h"

#include <unordered_map>

namespace Pascal 
{
    class HttpResponse 
    {
    public:
        HttpResponse(HttpStatus status) 
            : m_Status(status) { }

        HttpResponse() 
            : m_Status(HttpStatus::Success) { }

        ~HttpResponse() {}

        void SetStatus(HttpStatus status) 
        {
            m_Status = status;
        }

        void AddHeader(const std::string& header, const std::string& headerData) 
        {
            m_Headers.emplace(header, headerData);
        }

        void SetBody(const std::string& body) 
        {
            m_Body = body;
        }

        const std::string& GetBody() const 
        {
            return m_Body;
        }

        const HeaderMap& GetHeaders() const 
        {
            return m_Headers;
        }

        void SetContentType(const std::string& type) 
        {
            m_Headers.emplace("Content-Type", type);
        }

    private:
        // TODO: http version
        HttpStatus m_Status;
        HeaderMap m_Headers;
        std::string m_Body;

        friend class HttpResponseBuilder;
    };
}