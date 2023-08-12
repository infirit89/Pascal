#include "pspch.h"
#include "HttpUtils.h"

namespace Pascal 
{
    const char* HttpRequestMethodToString(HttpRequestMethod method) 
    {
        switch (method) 
        {
        case HttpRequestMethod::Get:        return "GET";
        case HttpRequestMethod::Head:       return "HEAD";
        case HttpRequestMethod::Post:       return "POST";
        case HttpRequestMethod::Put:        return "PUT";
        case HttpRequestMethod::Delete:     return "DELETE";
        case HttpRequestMethod::Connect:    return "CONNECT";
        case HttpRequestMethod::Options:    return "OPTIONS";
        case HttpRequestMethod::Trace:      return "TRACE";
        }

        return "";
    }    
}