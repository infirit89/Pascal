#include "pspch.h"
#include "BasicRouter.h"

namespace Pascal 
{
    void BasicRouter::AddRoute(
                        const std::string& path,
                        const ResponseCallback& callback,
                        HttpMethod allowedMethods) 
    {
        RouterItemData data = { .Callback = callback, .MethodMask = allowedMethods };

        m_Routes.emplace(path, data);
    }

    void BasicRouter::AddRoute(const std::string& path, ResponseCallback&& callback, HttpMethod allowedMethods) 
    {
        RouterItemData data = { .Callback = callback, .MethodMask = allowedMethods };
        m_Routes.emplace(path, data);
    }

    Shared<HttpResponse> BasicRouter::Route(const Shared<HttpRequest>& request) 
    {
        auto iter = m_Routes.find(request->GetTarget());

        if(iter == m_Routes.end())
            return nullptr;

        // method not allowed
        if((iter->second.MethodMask & request->GetMethod()) == HttpMethod::None)
            return nullptr;

        Shared<HttpResponse> response = iter->second.Callback(request);
        return response;
    }
}