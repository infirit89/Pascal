#pragma once 

#include "HttpUtils.h"

#include <unordered_map>

namespace Pascal 
{
    enum class HttpStatus
    {
        // 100S
        Continue = 100,
        SwitchingProtocols = 101,
        Processing = 102,
        EarlyHints = 103,

        // 200S
        Success = 200,
        Created = 201,
        Accepted = 202,
        NonAuthorativeInformation = 203,
        NoContent = 204,
        ResetContent = 205,
        PartialContent = 206,
        MultiStatus = 207,
        AlreadyReported = 208,
        ImUsed = 226,

        // 300S
        MultipleChoices = 300,
        MovedPermanently = 301,
        Found = 302,
        SeeOther = 303,
        NotModified = 304,
        UseProxy = 305,
        SwitchingProxy = 306,
        TemporaryRedirect = 307,
        PermenantRedirect = 308,

        // 400s
        BadRequest = 400,
        Unauthorized = 401,
        PaymentRequired = 402,
        Forbidden = 403,
        NotFound = 404,
        MethodNotAllowed = 405,
        NotAcceptable = 406,
        ProxyAuthenticationRequired = 407,
        RequestTimeout = 408,
        Conflict = 409,
        Gone = 410,
        LengthRequired = 411,
        PreconditionFailed = 412,
        PayloadTooLarge = 413,
        UriTooLong = 414,
        UnsupportedMediaType = 415,
        RangeNotSatisfiable = 416,
        ExpectationFailed = 417,
        ImATeapot = 418,
        MisdirectedRequest = 421,
        UnprocessableEntity = 422,
        Locked = 423,
        FailedDependency = 424,
        TooEarly = 425,
        UpgradeRequired = 426,
        PreconditionRequired = 428,
        TooManyRequests = 429,
        RequestHeaderFieldsTooLarge = 431,
        UnavailableForLegalReasons = 451,

        // 500S
        InternalServerError = 500,
        NotImplemented = 501,
        BadGateway = 502,
        ServiceUnavailable = 503,
        GatewayTimeout = 504,
        VersionNotSupported = 505,
        VariantAlsoNegotiates = 506,
        InsufficientStorage = 507,
        LoopDetected = 508,
        NotExtended = 510,
        NetworkAuthenticationRequired = 511,
    };

    class HttpResponse 
    {
    public:
        HttpResponse() {}
        ~HttpResponse() {}

        void SetStatus(HttpStatus status) 
        {
            m_Status = status;
        }

        void AddHeader(const std::string& header, const std::string& headerData) 
        {
            m_Headers.emplace(header, headerData);
        }

        void SetBody(const std::string& body) 
        {
            m_Body = body;
        }

        const std::string& GetBody() const 
        {
            return m_Body;
        }

        void SetContentType(const std::string& type) 
        {
            m_Headers.emplace("Content-Type", type);
        }

    private:
        // TODO: http version
        HttpStatus m_Status;
        HeaderMap m_Headers;
        std::string m_Body;

        friend class HttpResponseBuilder;
    };
}