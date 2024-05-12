set(CMAKE_PROJECT_MINIMUM_VERSION 3.20)
cmake_minimum_required(VERSION 3.20)

project(SteelEngine
	VERSION 0.1.0
	LANGUAGES CXX
)

set(PROJECT_ACRONYM se)
string(TOUPPER ${PROJECT_ACRONYM} PROJECT_ACRONYM_UPPERCASE)
set(PROJECT_CPP_DIALECT 23)

option(BUILD_TESTS "BUILD_TESTS" OFF)
option(FORCE_SDL2 "FORCE_SDL2" OFF)

# external dependencies
find_package(Vulkan REQUIRED)
message("Vulkan include directory : " ${Vulkan_INCLUDE_DIRS})
message("Vulkan libraries : " ${Vulkan_LIBRARIES})

find_package(SDL3 CONFIG COMPONENTS SDL3-shared)
if (SDL3_FOUND AND NOT FORCE_SDL2)
	set(WSI_PROVIDER_INCLUDE_DIRS $<TARGET_FILE_DIR:SDL3::SDL3>/../include)
	set(WSI_PROVIDER_LIBRARIES SDL3::SDL3)
	set(WSI_PROVIDER SDL3)
	if (WIN32)
		function(CLONE_WSI_DLL target)
			add_custom_command(
				TARGET ${target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_FILE:SDL3::SDL3>
				$<TARGET_FILE_DIR:${target}>
				VERBATIM)
		endfunction(CLONE_WSI_DLL)
	endif()
else()
	find_package(SDL2 CONFIG REQUIRED)
	set(WSI_PROVIDER SDL2)
	set(WSI_PROVIDER_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
	set(WSI_PROVIDER_LIBRARIES ${SDL2_LIBRARIES})
	if (WIN32)
		function(CLONE_WSI_DLL target)
			add_custom_command(
				TARGET ${target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_FILE:SDL2::SDL2>
				$<TARGET_FILE_DIR:${target}>
				VERBATIM)
		endfunction(CLONE_WSI_DLL)
	endif()
endif()

add_compile_definitions(${PROJECT_ACRONYM_UPPERCASE}_WSI_${WSI_PROVIDER})

message("WSI provider : " ${WSI_PROVIDER})
message("WSI include directory : " ${WSI_PROVIDER_INCLUDE_DIRS})
message("WSI libraries : " ${WSI_PROVIDER_LIBRARIES})



if (WIN32)
	add_compile_definitions(${PROJECT_ACRONYM_UPPERCASE}_WINDOWS)
elseif (UNIX)
	add_compile_definitions(${PROJECT_ACRONYM_UPPERCASE}_LINUX)
elseif (APPLE)
	add_compile_definitions(${PROJECT_ACRONYM_UPPERCASE}_APPLE)
else()
	message(FATAL_ERROR "Platforms other than Windows, Linux and MacOS aren't supported for now")
endif()

add_subdirectory(lib)
add_subdirectory(sandbox)
if (BUILD_TESTS)
	add_subdirectory(tests)
endif()