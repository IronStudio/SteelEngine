cmake_minimum_required(VERSION 3.20)


set(LIBRARY_NAME ${PROJECT_NAME})
set(LIBRARY_VERSION ${PROJECT_VERSION})
set(LIBRARY_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(LIBRARY_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(LIBRARY_VERSION_PATCH ${PROJECT_VERSION_PATCH})

add_compile_definitions(
	${PROJECT_ACRONYM_UPPERCASE}_NAME=${LIBRARY_NAME}
	${PROJECT_ACRONYM_UPPERCASE}_VERSION_MAJOR=${LIBRARY_VERSION_MAJOR}
	${PROJECT_ACRONYM_UPPERCASE}_VERSION_MINOR=${LIBRARY_VERSION_MINOR}
	${PROJECT_ACRONYM_UPPERCASE}_VERSION_PATCH=${LIBRARY_VERSION_PATCH}
)

option(BUILD_STATIC "BUILD_STATIC" OFF)

if (BUILD_STATIC)
	set(LIBRARY_TYPE STATIC)
	add_compile_definitions(${PROJECT_ACRONYM_UPPERCASE}_STATIC)
else()
	set(LIBRARY_TYPE SHARED)
	add_compile_definitions(
		${PROJECT_ACRONYM_UPPERCASE}_SHARED
		${PROJECT_ACRONYM_UPPERCASE}_BUILD_LIB
	)
endif()

include_directories(include ${DEPS_INCLUDE_DIRS})

file(GLOB_RECURSE LIBRARY_HEADER *.hpp)
file(GLOB_RECURSE LIBRARY_INLINE *.inl)
file(GLOB_RECURSE LIBRARY_SOURCE *.cpp)
file(GLOB_RECURSE LIBRARY_C_FILES *.c)

add_library(${LIBRARY_NAME} ${LIBRARY_TYPE} ${LIBRARY_HEADER} ${LIBRARY_INLINE} ${LIBRARY_SOURCE} ${LIBRARY_C_FILES})
set_property(TARGET ${LIBRARY_NAME} PROPERTY CXX_STANDARD ${PROJECT_CPP_DIALECT})
set_property(TARGET ${LIBRARY_NAME} PROPERTY C_STANDARD ${PROJECT_C_DIALECT})
set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${LIBRARY_NAME} PUBLIC ${DEPS_LIBRARIES})