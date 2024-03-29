#pragma once

#include <string>
#include <optional>

namespace Arklumos
{

	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::optional<std::string> OpenFile(const char *filter);
		static std::string SaveFile(const char *filter);
	};

}