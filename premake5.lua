workspace "SteelEngine"
	configurations {"debug", "release", "dist"}
	newoption {
		trigger = "shared",
		description = "Build SteelEngine as a shared library (.dll on Windows)"
	}


obj_dir = "obj/%{cfg.buildcfg}/%{prj.name}"
output_dir_without_project = "bin/%{cfg.buildcfg}"
output_dir = output_dir_without_project .. "/%{prj.name}"

vector_extension_version = "SSE4.1"


--		███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗
--		██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝
--		█████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗  
--		██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝  
--		███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗
--		╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝



project "engine"
	language "C++"
	cppdialect "C++20"
	location "engine"
	warnings "Extra"
	vectorextensions(vector_extension_version)

	files {
		"%{prj.location}/include/**.hpp",
		"%{prj.location}/include/**.inl",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/src/**.inl"
	}

	includedirs {
		"%{prj.location}/include/se"
	}

	targetname "steelengine"
	targetdir(output_dir)
	objdir(obj_dir)

	defines {"SE_BUILD_ENGINE"}


	filter "options:shared"
		kind "SharedLib"
		postbuildcommands("python3 ../scripts/copy.py ../" .. output_dir .. "/%{cfg.buildtarget.name} ../" .. output_dir_without_project .. "/sandbox/")
		defines {"SE_BUILD_SHARED"}

	filter "not options:shared"
		kind "StaticLib"

	filter "configurations:debug"
		defines {"DEBUG", "SE_DEBUG_MODE"}
		symbols "On"
		optimize "Off"

	filter "configurations:release"
		defines {"DEBUG", "SE_RELEASE_MODE"}
		symbols "On"
		optimize "On"

	filter "configurations:dist"
		defines {"NDEBUG", "SE_DIST_MODE"}
		symbols "Off"
		optimize "On"

	filter "system:Windows"
		defines "SE_PLATFORM_WINDOWS"

	filter "system:Unix"
		defines "SE_PLATFORM_LINUX"

	filter "system:Mac"
		defines "SE_PLATFORM_MACOS"



--		███████╗ █████╗ ███╗   ██╗██████╗ ██████╗  ██████╗ ██╗  ██╗
--		██╔════╝██╔══██╗████╗  ██║██╔══██╗██╔══██╗██╔═══██╗╚██╗██╔╝
--		███████╗███████║██╔██╗ ██║██████╔╝██║  ██║██║   ██║ ╚███╔╝ 
--		╚════██║██╔══██║██║╚██╗██║██╔══██╗██║  ██║██║   ██║ ██╔██╗ 
--		███████║██║  ██║██║ ╚████║██████╔╝██████╔╝╚██████╔╝██╔╝ ██╗
--		╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝


project "sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	location "sandbox"
	warnings "Extra"
	vectorextensions(vector_extension_version)

	files {
		"%{prj.location}/include/**.hpp",
		"%{prj.location}/include/**.inl",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/src/**.inl"
	}

	includedirs {
		"%{prj.location}/include/",
		"engine/include/"
	}

	libdirs {
		output_dir_without_project .. "/engine"
	}

	links {
		"steelengine"
	}

	targetname "sandbox"
	targetdir(output_dir)
	objdir(obj_dir)


	filter "configurations:debug"
		defines {"DEBUG", "SE_DEBUG_MODE"}
		symbols "On"
		optimize "Off"

	filter "configurations:release"
		defines {"DEBUG", "SE_RELEASE_MODE"}
		symbols "On"
		optimize "On"

	filter "configurations:dist"
		defines {"NDEBUG", "SE_DIST_MODE"}
		symbols "Off"
		optimize "On"
		kind "WindowedApp"

	filter "system:Windows"
		defines "SE_PLATFORM_WINDOWS"

	filter "system:Unix"
		defines "SE_PLATFORM_LINUX"

	filter "system:Mac"
		defines "SE_PLATFORM_MACOS"



--		████████╗███████╗███████╗████████╗
--		╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝
--		   ██║   █████╗  ███████╗   ██║   
--		   ██║   ██╔══╝  ╚════██║   ██║   
--		   ██║   ███████╗███████║   ██║   
--		   ╚═╝   ╚══════╝╚══════╝   ╚═╝



project "test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	location "tests"
	warnings "Extra"
	vectorextensions(vector_extension_version)

	files {
		"%{prj.location}/include/**.hpp",
		"%{prj.location}/include/**.inl",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/src/**.inl"
	}

	includedirs {
		"%{prj.location}/include/",
		"engine/include/"
	}

	libdirs {
		output_dir_without_project .. "/engine"
	}

	links {
		"steelengine"
	}

	targetname "tests"
	targetdir(output_dir)
	objdir(obj_dir)

	filter "configurations:debug"
		defines {"DEBUG", "SE_DEBUG_MODE"}
		symbols "On"
		optimize "Off"

	filter "configurations:release"
		defines {"DEBUG", "SE_RELEASE_MODE"}
		symbols "On"
		optimize "On"

	filter "configurations:dist"
		defines {"NDEBUG", "SE_DIST_MODE"}
		symbols "Off"
		optimize "On"
		kind "WindowedApp"

	filter "system:Windows"
		defines "SE_PLATFORM_WINDOWS"

	filter "system:Unix"
		defines "SE_PLATFORM_LINUX"

	filter "system:Mac"
		defines "SE_PLATFORM_MACOS"