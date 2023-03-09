include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Arklumos"
	architecture "x86_64"
	startproject "Arklusis"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Arklumos/vendor/GLFW"
	include "Arklumos/vendor/glad"
	include "Arklumos/vendor/imgui"
	include "Arklumos/vendor/yaml-cpp"
group ""

include "Arklumos"
include "Testbox"
include "Arklusis"