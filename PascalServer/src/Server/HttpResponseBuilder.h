#pragma once

#include "Core/Base.h"

#include "HttpResponse.h"

namespace Pascal 
{
    class HttpResponseBuilder 
    {
    public:
        static std::string BuildResponse(const Shared<HttpResponse>& response);
    };
}