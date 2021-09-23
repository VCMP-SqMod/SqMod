cmake_minimum_required(VERSION 3.7)
project(SqMod)

# This plug-in only works on 64-bit
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "SqMod does not support 32-but platforms anymore.")
endif()

# Tell CMake where to find our scripts
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/vendor/POCO/cmake)

# Several plugin options
option(ENABLE_API21 "Build for 2.1 API." OFF)
option(ENABLE_OFFICIAL "Enable compatibility with official legacy plug-in" ON)
# As a fall-back for certain situations (mainly some docker ubuntu containers)
option(ENABLE_BUILTIN_MYSQL_C "Enable built-in MySQL connector library" OFF)
#option(FORCE_32BIT_BIN "Create a 32-bit executable binary if the compiler defaults to 64-bit." OFF)
# This option should only be available in certain conditions
if(WIN32 AND MINGW)
    option(COPY_DEPENDENCIES "Copy deppendent DLLs into the deps folder." OFF)
endif()

# C++14 is mandatory
set(CPP_STD_NUMBER 14)

include(CheckCXXCompilerFlag)
# C++ standard availability check
if(${CMAKE_CXX_COMPILER_ID} MATCHES "(GNU)+")
    # Specific flags
    set(CPP_STD_COMPILER_FLAG "-std=c++14")
    # Don't even bother with previous version
    if(CPP_STD_NUMBER LESS 20 AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
        check_cxx_compiler_flag(-std=c++20 HAVE_FLAG_STD_CXX20)
        check_cxx_compiler_flag(-std=c++2a HAVE_FLAG_STD_CXX2A)
        if(HAVE_FLAG_STD_CXX20 OR HAVE_FLAG_STD_CXX2A)
            # We can use C++20
            set(CPP_STD_NUMBER 20)
            # Specific flags
            if (HAVE_FLAG_STD_CXX2A AND NOT HAVE_FLAG_STD_CXX20)
                set(CPP_STD_COMPILER_FLAG "-std=c++2a")
            else()
                set(CPP_STD_COMPILER_FLAG "-std=c++20")
            endif()
            # Need these workarounds for older CMake
            if(${CMAKE_VERSION} VERSION_LESS "3.8.0")
                if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
                    set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std=c++20")
                    set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std=gnu++20")
                elseif (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
                    set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std=c++2a")
                    set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std=gnu++2a")
                endif()
            endif()
        endif()
    endif()
    # Don't even bother with previous version
    if(CPP_STD_NUMBER LESS 17 AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)
        check_cxx_compiler_flag(-std=c++1z HAVE_FLAG_STD_CXX1Z)
        if(HAVE_FLAG_STD_CXX17 OR HAVE_FLAG_STD_CXX1Z)
            # We can use C++17
            set(CPP_STD_NUMBER 17)
            # Specific flags
            if (HAVE_FLAG_STD_CXX1Z AND NOT HAVE_FLAG_STD_CXX17)
                set(CPP_STD_COMPILER_FLAG "-std=c++1z")
            else()
                set(CPP_STD_COMPILER_FLAG "-std=c++17")
            endif()
            # Need these workarounds for older CMake
            if(${CMAKE_VERSION} VERSION_LESS "3.8.0")
                if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
                    set(CMAKE_CXX17_STANDARD_COMPILE_OPTION "-std=c++17")
                    set(CMAKE_CXX17_EXTENSION_COMPILE_OPTION "-std=gnu++17")
                elseif (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1)
                    set(CMAKE_CXX17_STANDARD_COMPILE_OPTION "-std=c++1z")
                    set(CMAKE_CXX17_EXTENSION_COMPILE_OPTION "-std=gnu++1z")
                endif()
            endif()
        endif()
    endif()
else()
    # C++14 is mandatory
    set(CPP_STD_NUMBER 14)
endif()

message(STATUS "SqMod: Using C++${CPP_STD_NUMBER} standard.")

# Default to the identified standard
if(CMAKE_VERSION VERSION_LESS "3.1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPP_STD_COMPILER_FLAG}")
else()
    # Apparently the above does not work with cmake from on debian 8
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPP_STD_COMPILER_FLAG}")
    # F* you too Debian. What can I say.
    if(CMAKE_VERSION VERSION_LESS "3.8.0" AND CPP_STD_NUMBER LESS 17)
        # Try the standard method as well
        set(CMAKE_CXX_STANDARD ${CPP_STD_NUMBER})
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
    endif()
endif()

# Strip binary
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -g")

# Enable position independent code
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Include vendor libraries
add_subdirectory(vendor)
# Include Module library
add_subdirectory(module)
