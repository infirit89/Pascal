#include "pspch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <spdlog/fmt/bundled/format.h>
#include <spdlog/async.h>

#include <cstdarg>

namespace Pascal 
{
	std::shared_ptr<spdlog::logger> Logger::s_Logger;

	void Logger::Init()
	{
		spdlog::init_thread_pool(8192, 1);
		std::vector<spdlog::sink_ptr> coreSinks
		{
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
			std::make_shared<spdlog::sinks::basic_file_sink_mt>(
															"logs/Pascal.log", 
															true)
		};

		coreSinks[0]->set_pattern("%^[%T] %n: %v%$");
		coreSinks[1]->set_pattern("%^[%T] %n: %v%$");

		s_Logger = std::make_shared<spdlog::async_logger>(
												"PASCAL", 
												coreSinks.begin(), 
												coreSinks.end(),
												spdlog::thread_pool());
												
		s_Logger->set_level(spdlog::level::trace);
		spdlog::register_logger(s_Logger);
	}
}

