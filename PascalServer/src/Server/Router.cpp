#include "pspch.h"
#include "Router.h"

namespace Pascal 
{
    Router::Router() { }
    Router::~Router() { }

    void Router::AddRoute(const std::string& path, const ResponseCallback& callback) 
    {
        m_Routes.emplace(path, callback);
    }

    void Router::AddRoute(const std::string& path, ResponseCallback&& callback) 
    {
        m_Routes.emplace(path, callback);
    }

    Shared<HttpResponse> Router::Route(const Shared<HttpRequest>& request) 
    {
        if(m_Routes.find(request->GetTarget()) == m_Routes.end())
            return nullptr;

        ResponseCallback& callback = m_Routes.at(request->GetTarget());
        Shared<HttpResponse> response = callback(request);
        return response;
    }
}
