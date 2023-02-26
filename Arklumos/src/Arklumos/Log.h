#pragma once
#include "akpch.h"

#include "Core.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Arklumos
{

	class ARKLUMOS_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define AK_CORE_TRACE(...) ::Arklumos::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AK_CORE_INFO(...) ::Arklumos::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AK_CORE_WARN(...) ::Arklumos::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AK_CORE_ERROR(...) ::Arklumos::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AK_CORE_FATAL(...) ::Arklumos::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define AK_TRACE(...) ::Arklumos::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AK_INFO(...) ::Arklumos::Log::GetClientLogger()->info(__VA_ARGS__)
#define AK_WARN(...) ::Arklumos::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AK_ERROR(...) ::Arklumos::Log::GetClientLogger()->error(__VA_ARGS__)
#define AK_FATAL(...) ::Arklumos::Log::GetClientLogger()->fatal(__VA_ARGS__)