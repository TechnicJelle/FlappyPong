workspace "FlappyPong"
	configurations { "Debug", "Release" }

project "FlappyPong"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"

	files
	{
		"src/**.hpp",
		"src/**.cpp"
	}

	includedirs
	{
		"olcPixelGameEngine"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		defines { "NDEBUG" }
		symbols "Off"
		optimize "Speed"

	filter "system:Linux"
		links
		{
			"X11",
			"GL",
			"pthread",
			"png"
		}
