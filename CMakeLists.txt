cmake_minimum_required(VERSION 3.25.2)
project(sengine VERSION 1.0.0 DESCRIPTION "taking a mean shit right now!")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin> CACHE INTERNAL "")

# Create library and include source files
file(GLOB_RECURSE SENGINE_SRC "./src/**.cpp" "./src/**.c")
file(GLOB_RECURSE SENGINE_INCLUDE "./include/**.h")
add_library(${PROJECT_NAME} STATIC ${SENGINE_SRC} ${SENGINE_INCLUDE})
target_include_directories(${PROJECT_NAME} PUBLIC include third_party)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(SENGINE_DEBUG)
	if (MINGW)
		target_link_libraries(${PROJECT_NAME} PRIVATE -mconsole)
	endif()
endif()

# MSVC is annoying about strcpy and such
if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# SDL
if (MSVC)
	target_include_directories(${PROJECT_NAME} PUBLIC thirdparty/sdl/include)
	target_link_libraries(${PROJECT_NAME} PUBLIC SDL2main SDL2)
	if (CMAKE_CL_64)
		target_link_directories(${PROJECT_NAME} PUBLIC 
			"${PROJECT_SOURCE_DIR}/thirdparty/sdl/lib/x64"
		)
	else()
		target_link_directories(${PROJECT_NAME} PUBLIC 
			"${PROJECT_SOURCE_DIR}/thirdparty/sdl/lib/x86"
		)
	endif()
else()
	find_package(SDL2 REQUIRED)
	target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
endif()

# Include any thirdparty libraries that the engine depends on
add_subdirectory(thirdparty)
target_link_libraries(${PROJECT_NAME} PUBLIC thirdparty)

# Any additional post-build scripts
include(CopyDatafiles)