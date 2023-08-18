#pragma once

#include "Core/Base.h"
#include "HttpResponse.h"
#include "HttpRequest.h"

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

namespace Pascal 
{
    // NOTE: a very very basic router; doesn't support query parameters
    class Router
    {
    public:
        enum class RoutingError 
        {
            None = 0,
            ResourceNotFound,
            Forbidden,
            MethodNotAllowed
        };

        Router() { }
        virtual ~Router() { }
        virtual Shared<HttpResponse> Route(
                                        const Shared<HttpRequest>& request,
                                        RoutingError& error) = 0;
    };

    class RouterManager 
    {
    public:
        RouterManager() { }
        ~RouterManager() { }

        void AddRouter(Shared<Router> router)  
        {
            m_Routers.emplace_back(router);
        }

        Shared<HttpResponse> TryRoute(
                                const Shared<HttpRequest>& request,
                                Router::RoutingError& error);

    private:
        std::vector<Shared<Router>> m_Routers;
    };
}
