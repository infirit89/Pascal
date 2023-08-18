#include "pspch.h"
#include "Router.h"

namespace Pascal 
{
    Shared<HttpResponse> RouterManager::TryRoute(
                                const Shared<HttpRequest>& request,
                                Router::RoutingError& error) 
    {
        for(const auto& router : m_Routers) 
        {
            Shared<HttpResponse> response = router->Route(request, error);

            // try to find another route that may have that request target
            if(error == Router::RoutingError::ResourceNotFound)
                continue;
            
            return response;
        }

        return nullptr;
    }
}
