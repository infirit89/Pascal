#pragma once

#include "HttpResponse.h"
#include "Core/Base.h"
#include "HttpRequest.h"
#include "Router.h"

#include <unordered_map>
#include <string>
#include <functional>

namespace Pascal 
{
    // NOTE: a very very basic router; doesn't support query parameters
    class BasicRouter : public Router
    {
    public:
        using ResponseCallback = std::function<
                                        Shared<HttpResponse>(
                                            const Shared<HttpRequest>&)>;


        BasicRouter() { }
        virtual ~BasicRouter() override { }

        void AddRoute(
                    const std::string& path,
                    const ResponseCallback& callback,
                    HttpMethod allowedMethods = HttpMethod::Get);

        void AddRoute(
                    const std::string& path,
                    ResponseCallback&& callback,
                    HttpMethod allowedMethods = HttpMethod::Get);

        virtual Shared<HttpResponse> Route(const Shared<HttpRequest>& request) override;

    private:

        // bad bad name
        struct RouterItemData
        {
            ResponseCallback Callback;
            HttpMethod MethodMask;
        };

        using RouteMap = std::unordered_map<std::string, RouterItemData>;
        RouteMap m_Routes;
    };
}