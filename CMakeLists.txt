cmake_minimum_required(VERSION 3.21)
project(SqMod)

# This plug-in only works on 64-bit
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "SqMod does not support 32-bit platforms anymore.")
endif()

# Tell CMake where to find our scripts
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${PROJECT_SOURCE_DIR}/vendor/POCO/cmake)

# Several plugin options
option(ENABLE_API21 "Build for 2.1 API." OFF)
option(ENABLE_OFFICIAL "Enable compatibility with official legacy plug-in" ON)
# As a fall-back for certain situations (mainly some docker ubuntu containers)
option(ENABLE_BUILTIN_MYSQL_C "Enable built-in MySQL connector library" OFF)
#option(FORCE_32BIT_BIN "Create a 32-bit executable binary if the compiler defaults to 64-bit." OFF)
# This option should only be available in certain conditions
if(WIN32 AND MINGW)
    option(COPY_DEPENDENCIES "Copy dependent DLLs into the deps folder." OFF)
endif()
# Discord suppport
option(ENABLE_DISCORD "Enable built-in Discord support." ON)

# C++17 is mandatory (globally)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
