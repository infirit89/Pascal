#pragma once

#include <unordered_map>
#include <string>

namespace Pascal 
{
    using HeaderMap = std::unordered_map<std::string, std::string>;

    enum class HttpRequestMethod 
    {
        None = 0,
        Get,
        Head,
        Post,
        Put,
        Delete,
        Connect,
        Options,
        Trace
    };

    const char* HttpRequestMethodToString(HttpRequestMethod method);
}

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/bundled/format.h>

template <>
struct fmt::formatter<Pascal::HttpRequestMethod> 
{
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) 
	{
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const Pascal::HttpRequestMethod& method, FormatContext& ctx) -> decltype(ctx.out())
	{
		return format_to(ctx.out(), "{}", Pascal::HttpRequestMethodToString(method));
	}
};

