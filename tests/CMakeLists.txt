cmake_minimum_required(VERSION 3.20)
enable_testing()


set(TEST_NAME ${PROJECT_NAME}_test)
set(TEST_VERSION ${PROJECT_VERSION})
set(TEST_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(TEST_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(TEST_VERSION_PATCH ${PROJECT_VERSION_PATCH})
string(TOUPPER ${TEST_NAME} TEST_NAME_UPPERCASE)

add_compile_definitions(
	${TEST_NAME_UPPERCASE}_NAME=${TEST_NAME}
	${TEST_NAME_UPPERCASE}_VERSION_MAJOR=${TEST_VERSION_MAJOR}
	${TEST_NAME_UPPERCASE}_VERSION_MINOR=${TEST_VERSION_MINOR}
	${TEST_NAME_UPPERCASE}_VERSION_PATCH=${TEST_VERSION_PATCH}
)

if (BUILD_STATIC)
	add_compile_definitions(${PROJECT_ACRONYM_UPPERCASE}_STATIC)
else()
	add_compile_definitions(${PROJECT_ACRONYM_UPPERCASE}_SHARED)
endif()

include(GoogleTest)

include_directories(./include ../lib/include ${DEPS_INCLUDE_DIRS})

file(GLOB_RECURSE TEST_HEADER *.hpp)
file(GLOB_RECURSE TEST_INLINE *.inl)
file(GLOB_RECURSE TEST_SOURCE *.cpp)

add_executable(${TEST_NAME} ${TEST_HEADER} ${TEST_INLINE} ${TEST_SOURCE})
set_property(TARGET ${TEST_NAME} PROPERTY CXX_STANDARD ${PROJECT_CPP_DIALECT})
set_target_properties(${TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME} ${DEPS_LIBRARIES} gtest_main gtest)
gtest_discover_tests(${TEST_NAME})



if (WIN32)
	if (BUILD_STATIC)
	else()
		message("Copying dll")
		message("${PROJECT_BINARY_DIR}/lib/lib${PROJECT_NAME}.dll")
		message("${PROJECT_BINARY_DIR}/test")
		add_custom_command(
			TARGET ${TEST_NAME}
			PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_BINARY_DIR}/lib/lib${PROJECT_NAME}.dll"
			"${PROJECT_BINARY_DIR}/test"
		)
	endif()

	CLONE_WSI_DLL(${TEST_NAME})
endif()