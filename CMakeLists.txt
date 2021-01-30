cmake_minimum_required(VERSION 3.0.2)
project(SqMod)

# Tell CMake where to find our scripts
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/vendor/POCO/cmake)

# Several plugin options
option(FORCE_32BIT_BIN "Create a 32-bit executable binary if the compiler defaults to 64-bit." OFF)
option(ENABLE_API21 "Build for 2.1 API." OFF)

include(CheckCXXCompilerFlag)
# C++ standard availability check
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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
else()
	# C++14 is mandatory
	set(CPP_STD_NUMBER 14)
endif()

message(STATUS "SqMod: Using C++${CPP_STD_NUMBER} standard.")

# Default to the identified standard
if(CMAKE_VERSION VERSION_LESS "3.1")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CPP_STD_NUMBER}")
	endif()
else()
	# Apparently the above does not work with cmake from on debian 8
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CPP_STD_NUMBER}")
	endif()
	# Try the standard method as well
	set(CMAKE_CXX_STANDARD ${CPP_STD_NUMBER})
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Determine if build mode
if(CMAKE_BUILD_TYPE MATCHES "(Debug)+")
	add_compile_options(-g)
endif()

# Enable position independent code
if(UNIX)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Include vendor libraries
add_subdirectory(vendor)
# Include Module library
add_subdirectory(module)
