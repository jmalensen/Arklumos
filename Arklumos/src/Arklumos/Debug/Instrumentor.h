#pragma once

#include "Arklumos/Core/Log.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

namespace Arklumos
{

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor &) = delete;
		Instrumentor(Instrumentor &&) = delete;

		void BeginSession(const std::string &name, const std::string &filepath = "results.json")
		{
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					AK_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				}
				InternalEndSession();
			}
			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new InstrumentationSession({name});
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					AK_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult &result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static Instrumentor &Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		Instrumentor()
				: m_CurrentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		/*
			Note: you must already own lock on m_Mutex before
			calling InternalEndSession()
		*/
		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}

	private:
		std::mutex m_Mutex;
		InstrumentationSession *m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	class InstrumentationTimer
	{
	public:
		// Constructor with name and initialization list
		InstrumentationTimer(const char *name)
				: m_Name(name), m_Stopped(false)
		{
			// Initializes the m_StartTimepoint variable with the current time using the std::chrono::steady_clock::now() function. This starts the timer for the InstrumentationTimer instance
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			// Gets the current time
			auto endTimepoint = std::chrono::steady_clock::now();

			/*
				Creates a new FloatingPointMicroseconds object named highResStart that is initialized with the number of microseconds since the epoch (January 1, 1970) represented by the m_StartTimepoint variable.
				This will be used to record the start time of the timer
			*/
			auto highResStart = FloatingPointMicroseconds{m_StartTimepoint.time_since_epoch()};

			/*
				Calculates the elapsed time between the start and end timepoints of the timer in microseconds.
				The time_since_epoch() function is used to get the duration since the epoch for each timepoint, and the time_point_cast() function is used to convert the timepoints to a duration in microseconds
			*/
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({m_Name, highResStart, elapsedTime, std::this_thread::get_id()});

			m_Stopped = true;
		}

	private:
		const char *m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace InstrumentorUtils
	{

		/*
			Defines two templates that are used to remove specific characters or strings from a string literal at compile time.
			The ChangeResult template is used to store the resulting string after removing a character or string.
			The CleanupOutputString template takes two string literals as arguments, the expr string literal and the remove string literal, which contains the character or string that needs to be removed.

			The template function then initializes an instance of the ChangeResult template, which has a character array of size N, where N is the length of the expr string literal.
			The function then iterates through the expr string literal, and for each character, it compares it with the remove string literal.
			If there is a match, the function continues iterating over expr without copying the matching character.
			If there is no match, the function copies the character into the result variable. Additionally, if the character is a double quote, it is changed to a single quote.

			In the end, return the result containing the cleaned up string.
			This process occurs at compile time, which means the resulting string is available at runtime without any additional overhead.
		*/
		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
				{
					matchIndex++;
				}
				if (matchIndex == K - 1)
				{
					srcIndex += matchIndex;
				}
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#define AK_PROFILE 0
#if AK_PROFILE
// Resolve which function signature macro will be used.
// Note that this only is resolved when the (pre)compiler starts,
// so the syntax highlighting  could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define AK_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define AK_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define AK_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define AK_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define AK_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define AK_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define AK_FUNC_SIG __func__
#else
#define AK_FUNC_SIG "AK_FUNC_SIG unknown!"
#endif

#define AK_PROFILE_BEGIN_SESSION(name, filepath) ::Arklumos::Instrumentor::Get().BeginSession(name, filepath)
#define AK_PROFILE_END_SESSION() ::Arklumos::Instrumentor::Get().EndSession()
#define AK_PROFILE_SCOPE_LINE2(name, line)                                                               \
	constexpr auto fixedName##line = ::Arklumos::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
	::Arklumos::InstrumentationTimer timer##line(fixedName##line.Data)
#define AK_PROFILE_SCOPE_LINE(name, line) AK_PROFILE_SCOPE_LINE2(name, line)
#define AK_PROFILE_SCOPE(name) AK_PROFILE_SCOPE_LINE(name, __LINE__)
#define AK_PROFILE_FUNCTION() AK_PROFILE_SCOPE(AK_FUNC_SIG)
#else
#define AK_PROFILE_BEGIN_SESSION(name, filepath)
#define AK_PROFILE_END_SESSION()
#define AK_PROFILE_SCOPE(name)
#define AK_PROFILE_FUNCTION()
#endif