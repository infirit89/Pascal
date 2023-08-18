#pragma once

#include "HttpTypes.h"

#include <string>
#include <unordered_map>

namespace Pascal 
{
    struct HttpRequest 
    {
        HttpRequest() {}
        ~HttpRequest() {}

        HttpMethod GetMethod() const { return m_Method; }
        const std::string& GetTarget() const { return m_Target; }
        HttpVersion GetVersion() const { return m_Version; }
        const HeaderMap& GetHeaders() const { return m_Headers; }
        const std::string& GetBody() const { return m_Body; }

        std::string GetHeader(const std::string& header) 
        {
            if(m_Headers.find(header) != m_Headers.end())
                return m_Headers.at(header);

            return "";
        }

        size_t GetHeaderCount() const { return m_Headers.size(); }

        std::string GetParamater(const std::string& paramName)
        {
            if(!m_HasParsedParameters)
                ParseParameters();

            if(m_Parameters.find(paramName) != m_Parameters.end())
                return m_Parameters.at(paramName);

            return "";
        }
        
        const std::string& GetQuery() const { return m_Query; }

    private:
        void ParseParameters();
        HttpVersion m_Version;
        HttpMethod m_Method;
        std::string m_Target;
        std::string m_Query;
        HeaderMap m_Headers;
        std::string m_Body;
        bool m_HasParsedParameters = false;
        std::unordered_map<std::string, std::string> m_Parameters;

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
	auto format(
            const Pascal::Shared<Pascal::HttpRequest>& request,
            FormatContext& ctx) -> decltype(ctx.out())
	{
        format_to(ctx.out(), "\nRequest:\n");
        format_to(
                ctx.out(),
                "\tMethod: {}\n",
                Pascal::HttpRequestMethodToString(request->GetMethod()));

        format_to(ctx.out(), "\tTarget: {}\n", request->GetTarget());

        if(request->GetQuery() != "")
            format_to(ctx.out(), "\tQuery: {}\n", request->GetQuery());

        format_to(ctx.out(), "\tVersion: {}\n", request->GetVersion());

        format_to(ctx.out(), "\tHeaders:\n");
        for (const auto& [header, headerData] : request->GetHeaders())
            format_to(ctx.out(), "\t\t{}: {}\n", header, headerData);

        if(request->GetHeader("content-length") != "") 
        {
            format_to(ctx.out(), "\tBody:\n");
            format_to(ctx.out(), "\t\t{}", request->GetBody());
        }
        
		return ctx.out();
	}
};
