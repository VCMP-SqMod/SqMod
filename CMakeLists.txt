cmake_minimum_required(VERSION 3.0.2)
project(SqMod)

# Tell CMake where to find our scripts
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Several plugin options
option(BUILTIN_RUNTIMES "Include the MinGW runtime into the binary itself." ON)
option(FORCE_32BIT_BIN "Create a 32-bit executable binary if the compiler defaults to 64-bit." OFF)
option(ENABLE_MYSQL "Enable the MySQL library." OFF)

# Default to c++14 standard
if(CMAKE_VERSION VERSION_LESS "3.1")
	if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	endif()
else()
	# Apparently the above does not work with cmake from on debian 8
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	# Try the standard method as well
	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Default to release mode
set(CMAKE_BUILD_TYPE "Release")

# Include mingw runntime into the binary
if (GCC OR MINGW)
	if(BUILTIN_RUNTIMES)
		set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
	endif()
endif()

# Enable position independent code
if (UNIX)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()
# Include VCMP library
add_subdirectory(vcmp)
# Include Squirrel library
add_subdirectory(squirrel)
# Include Squat library
add_subdirectory(sqrat)
# Include SDK library
add_subdirectory(sdk)
# Include Module library
add_subdirectory(module)
