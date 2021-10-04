include "Dependencies.lua"

workspace "UnitWorld"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Vega/vendor/glfw"
	include "Vega/vendor/glew"
	include "Vega/vendor/imgui"
group ""

include "Vega"
include "Sandbox"