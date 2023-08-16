#include "pspch.h"
#include "HttpTypes.h"

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

        return "None";
    }

    const char* HttpVersionToString(HttpVersion version) 
    {
        switch(version) 
        {
        case HttpVersion::Http10:   return "HTTP/1.0";
        case HttpVersion::Http11:   return "HTTP/1.1";
        }

        return "Unknown";
    }

    const char* HttpContentTypeToString(HttpContentType contentType) 
    {
        switch(contentType) 
        {
        case HttpContentType::Application_JavaScript:   return "application/javascript";
        case HttpContentType::Applicaion_JSON:          return "application/json";
        case HttpContentType::Application_XForm:        return "application/x-www-form-urlencoded";
        case HttpContentType::Audio_MPEG:               return "audio/mpeg";
        case HttpContentType::Audio_WAV:                return "audio/x-wav";
        case HttpContentType::Image_GIF:                return "image/gif";
        case HttpContentType::Image_JPEG:               return "image/jpeg";
        case HttpContentType::Image_PNG:                return "image/png";
        case HttpContentType::Multipart_Form:           return "multipart/form-data";
        case HttpContentType::Text_CSS:                 return "text/css";
        case HttpContentType::Text_HTML:                return "text/html";
        case HttpContentType::Text_Plain:               return "text/plain";
        case HttpContentType::Text_XML:                 return "text/xml";
        case HttpContentType::Video_MPEG:               return "video/mpeg";
        case HttpContentType::Video_MP4:                return "video/mp4";
        }

        return "";
    }
}