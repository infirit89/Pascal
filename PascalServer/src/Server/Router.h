#pragma once

#include "Core/Base.h"
#include "HttpResponse.h"
#include "HttpRequest.h"

#include <unordered_map>
#include <string>
#include <functional>

namespace Pascal 
{
    // NOTE: a very very basic router; doesn't support query parameters
    class Router
    {
    public:
        Router() { }
        virtual ~Router() { }
        virtual Shared<HttpResponse> Route(const Shared<HttpRequest>& request) = 0;
    };
}
