#pragma once

#include "HttpUtils.h"

#include <string>
#include <unordered_map>

namespace Pascal 
{
    class HttpRequest 
    {
    public:
        HttpRequest() {}
        ~HttpRequest() {}

        HttpRequestMethod GetMethod() const { return m_Method; }
        const std::string& GetTarget() const { return m_Target; }
        const std::string& GetVersion() const { return m_VersionString; }
        const HeaderMap& GetHeaders() const { return m_Headers; }
        
    private:
        HttpRequestMethod m_Method;
        std::string m_Target;
        std::string m_VersionString;
        HeaderMap m_Headers;

        friend class HttpRequestParser;
    };
}

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/bundled/format.h>

template <>
struct fmt::formatter<Pascal::Shared<Pascal::HttpRequest>> 
{
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) 
	{
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const Pascal::Shared<Pascal::HttpRequest>& request, FormatContext& ctx) -> decltype(ctx.out())
	{
        format_to(ctx.out(), "\nRequest:\n");
        format_to(ctx.out(), "\tMethod: {}\n", Pascal::HttpRequestMethodToString(request->GetMethod()));
        format_to(ctx.out(), "\tTarget: {}\n", request->GetTarget());
        format_to(ctx.out(), "\tVersion: {}\n", request->GetVersion());

        format_to(ctx.out(), "\tHeaders:\n");
        for (const auto& [header, headerData] : request->GetHeaders())
            format_to(ctx.out(), "\t\t{}: {}\n", header, headerData);
        
		return ctx.out();
	}
};
