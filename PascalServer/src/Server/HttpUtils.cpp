#include "pspch.h"
#include "HttpUtils.h"

namespace Pascal 
{
    const char* HttpRequestMethodToString(HttpMethod method) 
    {
        switch (method) 
        {
        case HttpMethod::Get:        return "GET";
        case HttpMethod::Head:       return "HEAD";
        case HttpMethod::Post:       return "POST";
        case HttpMethod::Put:        return "PUT";
        case HttpMethod::Delete:     return "DELETE";
        case HttpMethod::Connect:    return "CONNECT";
        case HttpMethod::Options:    return "OPTIONS";
        case HttpMethod::Trace:      return "TRACE";
        }

        return "";
    }    
}