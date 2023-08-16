#include "pspch.h"
#include "HttpRequest.h"

#include "Utils/Utils.h"

namespace Pascal 
{
    void HttpRequest::ParseParameters() 
    {
        size_t nextParameterIndex = 0;
        while(true) 
        {
            size_t valueSeperator = m_Body.find('=', nextParameterIndex + 1);
            std::string parameterName = m_Body.substr(
                                            nextParameterIndex + (
                                                nextParameterIndex > 0 
                                                ? 1 
                                                : 0),

                                            valueSeperator - (
                                                nextParameterIndex > 0 
                                                    ? nextParameterIndex + 1 
                                                    : nextParameterIndex));

            // decode parameter name if encoded:
            parameterName = Utils::DecodeURIComponent(parameterName);

            nextParameterIndex = m_Body.find('&', valueSeperator);

            std::string value = m_Body.substr(valueSeperator + 1, 
                                            nextParameterIndex - valueSeperator - 1);

            // decode parameter value if encoded:
            value = Utils::DecodeURIComponent(value);
            
            m_Parameters.emplace(parameterName, value);

            if(nextParameterIndex == std::string::npos)
                break;
        }

        m_HasParsedParameters = true;
    }
}
