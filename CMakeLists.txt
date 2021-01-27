cmake_minimum_required(VERSION 3.0.2)
project(SqMod)

# Tell CMake where to find our scripts
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Several plugin options
option(BUILTIN_RUNTIMES "Include the MinGW runtime into the binary itself." ON)
option(LTO_ENABLED "Enable link time optimizations (takes a long time to compile!)." OFF)
option(FORCE_32BIT_BIN "Create a 32-bit executable binary if the compiler defaults to 64-bit." OFF)
option(ENABLE_CURL "Enable the CURL library." ON)
option(ENABLE_MYSQL "Enable the MySQL library." ON)
option(ENABLE_MYSQL_OPENSSL "Enable MySQL library to use OpenSSL (windows only)." ON)
option(ENABLE_API21 "Build for 2.1 API." OFF)

include(CMakeDependentOption)
# Discord depends on CURL
cmake_dependent_option(ENABLE_DISCORD "Enable the Discord library." ON "ENABLE_CURL" OFF)

include(CheckCXXCompilerFlag)
# C++ standard availability check
check_cxx_compiler_flag(-std=c++20 HAVE_FLAG_STD_CXX20)
if(HAVE_FLAG_STD_CXX20)
	# We can use C++20
    set(CPP_STD_NUMBER 20)
else()
	check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)
    if(HAVE_FLAG_STD_CXX17)
		# We can use C++17
		set(CPP_STD_NUMBER 17)
    else()
		# C++14 is mandatory
		set(CPP_STD_NUMBER 14)
    endif()
endif()

message(STATUS "Using C++${CPP_STD_NUMBER} standard.")

# Default to the identified standard
if(CMAKE_VERSION VERSION_LESS "3.1")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CPP_STD_NUMBER}")
	endif()
else()
	# Apparently the above does not work with cmake from on debian 8
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CPP_STD_NUMBER}")
	# Try the standard method as well
	set(CMAKE_CXX_STANDARD ${CPP_STD_NUMBER})
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Determine if build mode
if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_compile_options(-g)
endif()

# Include MINGW runtime into the binary
if(GCC OR MINGW)
	if(BUILTIN_RUNTIMES)
		set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
	endif()
endif()

# Enable LTO
if (NOT LTO_ENABLED)
	message("Link time optimizations are disabled")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "MINGW")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
	endif()
else()
	set(LTO_ENABLED OFF)
	message("Link time optimizations not supported")
endif ()

# Enable position independent code
if(UNIX)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Include SDK library
add_subdirectory(sdk)
# Include Module library
add_subdirectory(module)