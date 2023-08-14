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
        using ResponseCallback = std::function<Shared<HttpResponse>(const Shared<HttpRequest>&)>;
        using RouteMap = std::unordered_map<std::string, ResponseCallback>;

        Router();
        ~Router();

        void AddRoute(const std::string& path, const ResponseCallback& callback);
        void AddRoute(const std::string& path, ResponseCallback&& callback);

        Shared<HttpResponse> Route(const Shared<HttpRequest>& request);

    private:
        RouteMap m_Routes;
    };
}
