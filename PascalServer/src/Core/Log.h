#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


namespace Pascal 
{
	class Logger 
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#define PS_TRACE(...) ::Pascal::Logger::GetLogger()->trace(__VA_ARGS__)
#define PS_WARN(...) ::Pascal::Logger::GetLogger()->warn(__VA_ARGS__)
#define PS_ERROR(...) ::Pascal::Logger::GetLogger()->error(__VA_ARGS__)
#define PS_INFO(...) ::Pascal::Logger::GetLogger()->info(__VA_ARGS__)
