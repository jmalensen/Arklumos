#include "akpch.h"
#include "Arklumos/Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Arklumos
{

	// Initializes two shared pointers to spdlog::logger objects
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// Declares a vector of shared pointers to spdlog sinks
		std::vector<spdlog::sink_ptr> logSinks;

		/*
			Adds a sink to the vector using the emplace_back method.
			The sink being added is created using the std::make_shared function with spdlog::sinks::stdout_color_sink_mt() as an argument.
			This sink writes log messages to the console (stdout) with colored output.
		*/
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		// Saves the log to the file Arklumos.log
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Arklumos.log", true));

		/*
			Sets the logging pattern for the first sink in the logSinks vector. The pattern is specified using the set_pattern method, which takes a string argument that defines the format of the log messages.
			In this case, the pattern includes the time of the log message (%T), the name of the logger (%n), and the log message itself (%v).
			The %^ and %$ sequences are used to colorize the output using ANSI escape codes, making the log messages appear in a specific color (in this case, green)
		*/
		logSinks[0]->set_pattern("%^[%T] %n: %v%$");

		/*
			Sets the logging pattern for the second sink in the logSinks vector.
			The pattern includes the time of the log message (%T), the level of the log message (%l), the name of the logger (%n), and the log message itself (%v).
			The level of the log message is specified using %l, which can be one of several values (e.g., trace, debug, info, warn, error, critical), depending on the severity of the message
		*/
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		///
		// Creates a new spdlog logger with the name "ARKLUMOS" and a range of sinks specified by begin(logSinks) and end(logSinks). The std::make_shared function is used to create a shared pointer to the logger
		s_CoreLogger = std::make_shared<spdlog::logger>("ARKLUMOS", begin(logSinks), end(logSinks));

		// Registers the s_CoreLogger with spdlog, making it available for use in the program
		spdlog::register_logger(s_CoreLogger);

		// Sets the logging level for the s_CoreLogger to trace, which is the lowest and most verbose logging level. This means that all log messages with a severity level of trace, debug, info, warn, error, and critical will be logged by the s_CoreLogger
		s_CoreLogger->set_level(spdlog::level::trace);

		// Sets the s_CoreLogger to flush the log buffer to the sinks immediately after logging messages with a severity level of trace. This ensures that log messages are immediately written to the console and log file, rather than being buffered and potentially lost in the event of a crash or unexpected program termination
		s_CoreLogger->flush_on(spdlog::level::trace);

		///
		// Creates a new spdlog logger with the name "APP" and a range of sinks specified by begin(logSinks) and end(logSinks). The std::make_shared function is used to create a shared pointer to the logger
		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));

		// Registers the s_ClientLogger with spdlog, making it available for use in the program
		spdlog::register_logger(s_ClientLogger);

		// Sets the logging level for the s_ClientLogger to trace, which is the lowest and most verbose logging level. This means that all log messages with a severity level of trace, debug, info, warn, error, and critical will be logged by the s_ClientLogger
		s_ClientLogger->set_level(spdlog::level::trace);

		// Sets the s_ClientLogger to flush the log buffer to the sinks immediately after logging messages with a severity level of trace. This ensures that log messages are immediately written to the console and log file, rather than being buffered and potentially lost in the event of a crash or unexpected program termination
		s_ClientLogger->flush_on(spdlog::level::trace);
	}

}