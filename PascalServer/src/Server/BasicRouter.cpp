#include "pspch.h"
#include "BasicRouter.h"

#include <regex>

namespace Pascal 
{
    void BasicRouter::AddRoute(
                        const std::string& path,
                        const Shared<HttpParameterBinderBase>& binder,
                        HttpMethod allowedMethods) 
    {
        size_t query = path.find('?');

        std::string params, tempPath = path;
        if(query != std::string::npos) 
        {
            params = path.substr(query + 1);
            tempPath = path.substr(0, query);
        }

        std::vector<std::string> parameterNames;

        if(!params.empty()) 
        {
            // making this static fixes an access violation error
            // i can't find out why
            static const std::regex regex("([^&]*)=\\{([^&]*)\\}&*");
            std::smatch match;

            while(std::regex_search(params, match, regex)) 
            {
                if(match.size() > 2) 
                {
                    parameterNames.emplace_back(match[1].str());
                }

                params = match.suffix();
            }
        }

        RouterItemData data = 
        {
            .Binder = binder,
            .ParameterNames = parameterNames,
            .MethodMask = allowedMethods
        };

        m_Routes.emplace(tempPath, data);
    }

    Shared<HttpResponse> BasicRouter::Route(
                                        const Shared<HttpRequest>& request,
                                        RoutingError& error) 
    {
        auto iter = m_Routes.find(request->GetTarget());

        if(iter == m_Routes.end())
        {
            error = RoutingError::ResourceNotFound;
            return nullptr;
        }

        // method not allowed
        if((iter->second.MethodMask & request->GetMethod()) == HttpMethod::None)
        {
            error = RoutingError::MethodNotAllowed;
            return nullptr;
        }

        std::deque<std::string> params;

        for(const auto& parameterName : iter->second.ParameterNames) 
        {
            std::string param = request->GetParamater(parameterName);

            if(param != "")
                params.push_back(param);
        }

        Shared<HttpParameterBinderBase> binder = iter->second.Binder;

        Shared<HttpResponse> response = binder->Handle(params, request);

        error = RoutingError::None;
        return response;
    }
}
