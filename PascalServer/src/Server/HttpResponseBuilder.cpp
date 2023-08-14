#include "pspch.h"

#include "HttpResponseBuilder.h"
#include "Core/Buffer.h"

#include <spdlog/fmt/fmt.h>

namespace Pascal 
{
    static const char* StatusCodeToString(HttpStatus status) 
    {
        switch (status) 
        {
        // 100s
        case HttpStatus::Continue:
            return "Continue";
        case HttpStatus::SwitchingProtocols:
            return "Switching Protocols";
        case HttpStatus::Processing:
            return "Processing";
        case HttpStatus::EarlyHints:
            return "Early Hints";
        
        // 200s
        case HttpStatus::Success:
            return "OK";
        case HttpStatus::Created:
            return "Created";
        case HttpStatus::Accepted:
            return "Accepted";
        case HttpStatus::NonAuthorativeInformation:
            return "Non-Authoritative Information";
        case HttpStatus::NoContent:
            return "No Content";
        case HttpStatus::ResetContent:
            return "Reset Content";
        case HttpStatus::PartialContent:
            return "Partial Content";
        case HttpStatus::MultiStatus:
            return "Multi-Status";
        case HttpStatus::AlreadyReported:
            return "Already Reported";
        case HttpStatus::ImUsed:
            return "IM Used";

        // 300s
        case HttpStatus::MultipleChoices:
            return "Multiple Choices";
        case HttpStatus::MovedPermanently:
            return "Moved Permanently";
        case HttpStatus::Found:
            return "Found";
        case HttpStatus::SeeOther:
            return "See Other";
        case HttpStatus::NotModified:
            return "Not Modified";
        case HttpStatus::UseProxy:
            return "Use Proxy";
        case HttpStatus::SwitchingProxy:
            return "Switch Proxy";
        case HttpStatus::TemporaryRedirect:
            return "Temporary Redirect";
        case HttpStatus::PermenantRedirect:
            return "Permanent Redirect";

        // 400s
        case HttpStatus::BadRequest:
            return "Bad Request";
        case HttpStatus::Unauthorized:
            return "Unauthorized";
        case HttpStatus::PaymentRequired:
            return "Payment Required";
        case HttpStatus::Forbidden:
            return "Forbidden";
        case HttpStatus::NotFound:
            return "Not Found";
        case HttpStatus::MethodNotAllowed:
            return "Method Not Allowed";
        case HttpStatus::NotAcceptable:
            return "Not Acceptable";
        case HttpStatus::ProxyAuthenticationRequired:
            return "Proxy Authentication Required";
        case HttpStatus::RequestTimeout:
            return "Request Timeout";
        case HttpStatus::Conflict:
            return "Conflict";
        case HttpStatus::Gone:
            return "Gone";
        case HttpStatus::LengthRequired:
            return "Length Required";
        case HttpStatus::PreconditionFailed:
            return "Precondition Failed";
        case HttpStatus::PayloadTooLarge:
            return "Payload Too Large";
        case HttpStatus::UriTooLong:
            return "URI Too Long";
        case HttpStatus::UnsupportedMediaType:
            return "Unsupported Media Type";
        case HttpStatus::RangeNotSatisfiable:
            return "Range Not Satisfiable";
        case HttpStatus::ExpectationFailed:
            return "Expectation Failed";
        case HttpStatus::ImATeapot:
            return "I'm a teapot";
        case HttpStatus::MisdirectedRequest:
            return "Misdirected Request";
        case HttpStatus::UnprocessableEntity:
            return "Unprocessable Entity";
        case HttpStatus::Locked:
            return "Locked";
        case HttpStatus::FailedDependency:
            return "Failed Dependency";
        case HttpStatus::TooEarly:
            return "Too Early";
        case HttpStatus::UpgradeRequired:
            return "Upgrade Required";
        case HttpStatus::PreconditionRequired:
            return "Precondition Required";
        case HttpStatus::TooManyRequests:
            return "Too Many Requests";
        case HttpStatus::RequestHeaderFieldsTooLarge:
            return "Request Header Fields Too Large";
        case HttpStatus::UnavailableForLegalReasons:
            return "Unavailable For Legal Reasons";

        // 500s
        case HttpStatus::InternalServerError:
            return "Internal Server Error";
        case HttpStatus::NotImplemented:
            return "Not Implemented";
        case HttpStatus::BadGateway:
            return "Bad Gateway";
        case HttpStatus::ServiceUnavailable:
            return "Service Unavailable";
        case HttpStatus::GatewayTimeout:
            return "Gateway Timeout";
        case HttpStatus::VersionNotSupported:
            return "HTTP Version Not Supported";
        case HttpStatus::VariantAlsoNegotiates:
            return "Variant Also Negotiates";
        case HttpStatus::InsufficientStorage:
            return "Insufficient Storage";
        case HttpStatus::LoopDetected:
            return "Loop Detected";
        case HttpStatus::NotExtended:
            return "Not Extended";
        case HttpStatus::NetworkAuthenticationRequired:
            return "Network Authentication Required";
        }

        return "";
    }

    std::string HttpResponseBuilder::BuildResponse(const Shared<HttpResponse>& response) 
    {
        Buffer buffer(PS_HTTP_REQUEST_INITIAL_SIZE);
        
        // write the status code to the response buffer
        buffer.WriteFormatted("HTTP/1.1 {0} {1}\r\n", 
                            (int)response->m_Status,
                            StatusCodeToString(response->m_Status));

        // write the content length
        buffer.WriteFormatted("Content-Length: {0}\r\n", response->GetBody().size());

        // write all the headers
        for(const auto& [header, headerData] : response->GetHeaders())
            buffer.WriteFormatted("{0}: {1}\r\n", header, headerData);

        buffer.Write("\r\n");

        if(response->GetBody().size() > 0)
            buffer.WriteFormatted("{0}\r\n", response->GetBody());

        return buffer.GetData();
    }
}