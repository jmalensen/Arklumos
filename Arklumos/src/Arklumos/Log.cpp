#include "Log.h"

namespace Arklumos
{

	// Initializes two shared pointers to spdlog::logger objects
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		/*
			The spdlog::set_pattern() function is called to set the log message pattern to "%^[%T] %n: %v%$", which is a custom pattern that includes the timestamp (%T), logger name (%n), and log message (%v).
			The %^ and %$ characters are used to set the color of the log message to green.
		*/
		spdlog::set_pattern("%^[%T] %n: %v%$");

		/*
			Two logger objects are then created using the spdlog::stdout_color_mt() function, which creates a logger that writes log messages to the console with colored output.
			The first logger is named "ARKLUMOS" and is assigned to s_CoreLogger, while the second logger is named "APP" and is assigned to s_ClientLogger.

			Finally, the set_level() function is called on both logger objects to set the logging level to spdlog::level::trace, which means that all log messages with a severity level of trace or higher will be logged.
			This is the lowest severity level and includes all log messages.
		*/
		s_CoreLogger = spdlog::stdout_color_mt("ARKLUMOS");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}