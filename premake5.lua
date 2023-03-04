workspace "Arklumos"
	architecture "x86_64"
	startproject "Testbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Arklumos/vendor/GLFW/include"
IncludeDir["glad"] = "Arklumos/vendor/glad/include"
IncludeDir["ImGui"] = "Arklumos/vendor/imgui"
IncludeDir["glm"] = "Arklumos/vendor/glm"
IncludeDir["stb_image"] = "Arklumos/vendor/stb_image"

group "Dependencies"
	include "Arklumos/vendor/GLFW"
	include "Arklumos/vendor/glad"
	include "Arklumos/vendor/imgui"

group ""

project "Arklumos"
	location "Arklumos"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "akpch.h"
	pchsource "Arklumos/src/akpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
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
		symbols "on"

	filter "configurations:Release"
		defines "AK_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AK_DIST"
		runtime "Release"
		optimize "on"

project "Testbox"
	location "Testbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

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
		"Arklumos/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Arklumos"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

	filter "configurations:Debug"
		defines "AK_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AK_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AK_DIST"
		runtime "Release"
		optimize "on"