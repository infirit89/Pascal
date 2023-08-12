#pragma once

#include "../Core/Base.h"
#include "../Core/Buffer.h"

#include "HttpRequest.h"

namespace Pascal 
{
    class HttpRequestParser 
    {
    public:
        static Shared<HttpRequest> ParseRequest(const Buffer& messageBuffer);
    };
}
