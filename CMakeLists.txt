set(CMAKE_PROJECT_MINIMUM_VERSION 3.20)
cmake_minimum_required(VERSION 3.20)

project(SteelEngine
	VERSION 0.0.0
	LANGUAGES CXX
)

set(PROJECT_ACRONYM se)
string(TOUPPER ${PROJECT_ACRONYM} PROJECT_ACRONYM_UPPERCASE)
set(PROJECT_CPP_DIALECT 23)

option(BUILD_TESTS "BUILD_TESTS" OFF)
option(USE_UTF16_STRING "USE_UTF16_STRING" OFF)

add_compile_definitions(
	$<$<CONFIG:Debug>:${PROJECT_ACRONYM_UPPERCASE}_DEBUG>
	$<$<CONFIG:Release>:${PROJECT_ACRONYM_UPPERCASE}_RELEASE>
	$<$<CONFIG:Dist>:${PROJECT_ACRONYM_UPPERCASE}_DIST>
)

if (USE_UTF16_STRING)
	add_compile_definitions(SE_USE_UTF16_STRING)
endif()

add_subdirectory(engine)
add_subdirectory(sandbox)
if (BUILD_TESTS)
	add_subdirectory(tests)
endif()