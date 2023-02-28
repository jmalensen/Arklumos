workspace "Arklumos"
	architecture "x64"
	startproject "Testbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Arklumos/vendor/GLFW/include"
IncludeDir["glad"] = "Arklumos/vendor/glad/include"
IncludeDir["ImGui"] = "Arklumos/vendor/imgui"

group "Dependencies"
	include "Arklumos/vendor/GLFW"
	include "Arklumos/vendor/glad"
	include "Arklumos/vendor/imgui"

group ""

project "Arklumos"
	location "Arklumos"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "akpch.h"
	pchsource "Arklumos/src/akpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}"
	}

	links 
	{ 
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		defines
		{
			"AK_PLATFORM_WINDOWS",
			"AK_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPYFILE} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Testbox/\"")
		}

	filter "configurations:Debug"
		defines "AK_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "AK_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "AK_DIST"
		runtime "Release"
		optimize "On"

project "Testbox"
	location "Testbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Arklumos/vendor/spdlog/include",
		"Arklumos/src"
	}

	links
	{
		"Arklumos"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		defines
		{
			"AK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "AK_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "AK_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "AK_DIST"
		runtime "Release"
		optimize "On"