#pragma once

#include "../Core/Base.h"
#include "../Core/Buffer.h"

#include "HttpRequest.h"

namespace Pascal 
{
    class HttpRequestParser 
    {
    public:
        enum class Status 
        {
            Success = 0,
            UnexpectedMethod,
            URITooLong,
            IllformedRequest,
            HttpVersionNotSupported
        };

        static Shared<HttpRequest> ParseRequest(
                                            Buffer& messageBuffer, 
                                            Status& status);

        static uint32_t GetMaxURILength() { return s_MaxUriLength; }
        
        static void SetMaxURILength(uint32_t maxUriLength) 
        { s_MaxUriLength = maxUriLength; }

    private:
        static uint32_t s_MaxUriLength;
    };
}
