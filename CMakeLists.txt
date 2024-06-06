set(CMAKE_PROJECT_MINIMUM_VERSION 3.20)
cmake_minimum_required(VERSION 3.20)

project(SteelEngine
	VERSION 0.1.0
	LANGUAGES CXX C
)

set(PROJECT_ACRONYM se)
string(TOUPPER ${PROJECT_ACRONYM} PROJECT_ACRONYM_UPPERCASE)
set(PROJECT_CPP_DIALECT 23)
set(PROJECT_C_DIALECT 23)

option(BUILD_TESTS "BUILD_TESTS" OFF)

# external dependencies
find_package(Vulkan REQUIRED)
message("Vulkan include directory : " ${Vulkan_INCLUDE_DIRS})
message("Vulkan libraries : " ${Vulkan_LIBRARIES})

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

add_compile_definitions(${PROJECT_ACRONYM_UPPERCASE}_WSI_${WSI_PROVIDER})

message("WSI provider : " ${WSI_PROVIDER})
message("WSI include directory : " ${WSI_PROVIDER_INCLUDE_DIRS})
message("WSI libraries : " ${WSI_PROVIDER_LIBRARIES})


add_subdirectory(vendors/glad)


set(DEPS_INCLUDE_DIRS ${WSI_PROVIDER_INCLUDE_DIRS}
	${Vulkan_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/vendors/SteelEngine-Malge/se-malge/include
	${SE_GLAD_INCLUDE_DIR}
)
set(DEPS_LIBRARIES ${WSI_PROVIDER_LIBRARIES} ${Vulkan_LIBRARIES} glad)

message("Dependencies include path : " ${DEPS_INCLUDE_DIRS})
message("Dependencies libraries : " ${DEPS_LIBRARIES})


if (WIN32)
	add_compile_definitions(
		${PROJECT_ACRONYM_UPPERCASE}_WINDOWS
		#SE_MALGE_VECTORIZE
		#SE_MALGE_ARCHITECTURE_X86
		#SE_MALGE_AVX
	)
elseif (UNIX)
	add_compile_definitions(
		${PROJECT_ACRONYM_UPPERCASE}_LINUX
		#SE_MALGE_VECTORIZE
		#SE_MALGE_ARCHITECTURE_X86
		#SE_MALGE_AVX
	)
elseif (APPLE)
	add_compile_definitions(${PROJECT_ACRONYM_UPPERCASE}_APPLE)
else()
	message(FATAL_ERROR "Platforms other than Windows, Linux and MacOS aren't supported for now")
endif()

add_subdirectory(lib)
add_subdirectory(sandbox)
add_subdirectory(tests)