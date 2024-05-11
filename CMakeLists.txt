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

find_package(Vulkan REQUIRED)
message("Vulkan include directory : " ${Vulkan_INCLUDE_DIRS})
message("Vulkan libraries : " ${Vulkan_LIBRARIES})

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