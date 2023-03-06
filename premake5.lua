workspace "Arklumos"
	architecture "x86_64"
	startproject "Arklusis"

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
IncludeDir["entt"] = "Arklumos/vendor/entt/include"

group "Dependencies"
	include "Arklumos/vendor/GLFW"
	include "Arklumos/vendor/glad"
	include "Arklumos/vendor/imgui"

group ""

project "Arklumos"
	location "Arklumos"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
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
	cppdialect "C++17"
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
		"Arklumos/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Arklumos"
	}

	filter "system:windows"
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


project "Arklusis"
	location "Arklusis"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
		"Arklumos/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Arklumos"
	}

	filter "system:windows"
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