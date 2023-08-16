#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>

#include "Core/Base.h"

namespace Pascal 
{
    using HeaderMap = std::unordered_map<std::string, std::string>;

    enum class HttpStatus
    {
        // 100S
        Continue                        = 100,
        SwitchingProtocols              = 101,
        Processing                      = 102,
        EarlyHints                      = 103,

        // 200S
        Success                         = 200,
        Created                         = 201,
        Accepted                        = 202,
        NonAuthorativeInformation       = 203,
        NoContent                       = 204,
        ResetContent                    = 205,
        PartialContent                  = 206,
        MultiStatus                     = 207,
        AlreadyReported                 = 208,
        ImUsed                          = 226,

        // 300S
        MultipleChoices                 = 300,
        MovedPermanently                = 301,
        Found                           = 302,
        SeeOther                        = 303,
        NotModified                     = 304,
        UseProxy                        = 305,
        SwitchingProxy                  = 306,
        TemporaryRedirect               = 307,
        PermenantRedirect               = 308,

        // 400s
        BadRequest                      = 400,
        Unauthorized                    = 401,
        PaymentRequired                 = 402,
        Forbidden                       = 403,
        NotFound                        = 404,
        MethodNotAllowed                = 405,
        NotAcceptable                   = 406,
        ProxyAuthenticationRequired     = 407,
        RequestTimeout                  = 408,
        Conflict                        = 409,
        Gone                            = 410,
        LengthRequired                  = 411,
        PreconditionFailed              = 412,
        PayloadTooLarge                 = 413,
        UriTooLong                      = 414,
        UnsupportedMediaType            = 415,
        RangeNotSatisfiable             = 416,
        ExpectationFailed               = 417,
        ImATeapot                       = 418,
        MisdirectedRequest              = 421,
        UnprocessableEntity             = 422,
        Locked                          = 423,
        FailedDependency                = 424,
        TooEarly                        = 425,
        UpgradeRequired                 = 426,
        PreconditionRequired            = 428,
        TooManyRequests                 = 429,
        RequestHeaderFieldsTooLarge     = 431,
        UnavailableForLegalReasons      = 451,

        // 500S
        InternalServerError             = 500,
        NotImplemented                  = 501,
        BadGateway                      = 502,
        ServiceUnavailable              = 503,
        GatewayTimeout                  = 504,
        VersionNotSupported             = 505,
        VariantAlsoNegotiates           = 506,
        InsufficientStorage             = 507,
        LoopDetected                    = 508,
        NotExtended                     = 510,
        NetworkAuthenticationRequired   = 511,
    };

    enum class HttpMethod : uint8_t
    {
        None    = 0,
        Get     = BIT(0),
        Head    = BIT(1),
        Post    = BIT(2),
        Put     = BIT(3),
        Delete  = BIT(4),
        Connect = BIT(5),
        Options = BIT(6),
        Trace   = BIT(7),

        // i don't think this will ever be used but well
        All     = Get | Head | Post | Put | 
                    Delete | Connect | Options | Trace
    };
    DEFINE_ENUM_BITMASK_OPERATORS(HttpMethod)

    enum class HttpVersion 
    {
        Unknown = 0,
        Http11,
        Http10
    };

    enum class HttpContentType 
    {
        None = 0,
        Application_JavaScript,
        Applicaion_JSON,
        Application_XForm,
        Audio_MPEG,
        Audio_WAV,
        Image_GIF,
        Image_JPEG,
        Image_PNG,
        Multipart_Form,
        Text_CSS,
        Text_HTML,
        Text_Plain,
        Text_XML,
        Video_MPEG,
        Video_MP4
    };

    static std::unordered_map<std::filesystem::path, HttpContentType> ContentTypeExtensionMap = 
    {
        { ".js", HttpContentType::Application_JavaScript },
        { ".json", HttpContentType::Applicaion_JSON },
        { ".mpeg", HttpContentType::Audio_MPEG },
        { ".mpg", HttpContentType::Audio_MPEG },
        { ".wav", HttpContentType::Audio_WAV },
        { ".gif", HttpContentType::Image_GIF },
        { ".jpeg", HttpContentType::Image_JPEG },
        { ".jpg", HttpContentType::Image_JPEG },
        { ".png", HttpContentType::Image_PNG },
        { ".css", HttpContentType::Text_CSS },
        { ".html", HttpContentType::Text_HTML },
        { ".txt", HttpContentType::Text_Plain },
        { ".xml", HttpContentType::Text_XML },
        { ".mp4", HttpContentType::Video_MP4 }
    };

    const char* HttpRequestMethodToString(HttpMethod method);
    const char* HttpVersionToString(HttpVersion version);
    const char* HttpContentTypeToString(HttpContentType version);
}

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/bundled/format.h>

template <>
struct fmt::formatter<Pascal::HttpMethod> 
{
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) 
	{
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const Pascal::HttpMethod& method, FormatContext& ctx) -> decltype(ctx.out())
	{

        if(method == Pascal::HttpMethod::None)
            return format_to(ctx.out(), "None");

        Pascal::HttpMethod temp = method;
        for(uint8_t i = 1; i <= (uint8_t)Pascal::HttpMethod::Trace; i <<= 1)
        {
            if(temp == Pascal::HttpMethod::None)
                break;

            Pascal::HttpMethod current = temp & (Pascal::HttpMethod)i;
            if(current != Pascal::HttpMethod::None) 
            {
                format_to(ctx.out(), "{} ", Pascal::HttpRequestMethodToString(current));
                temp ^= current;
            }
        }

		return ctx.out();
	}
};

template <>
struct fmt::formatter<Pascal::HttpVersion> 
{
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) 
	{
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const Pascal::HttpVersion& versio, FormatContext& ctx) -> decltype(ctx.out())
	{
		return format_to(ctx.out(), "{}", Pascal::HttpVersionToString(versio));
	}
};

