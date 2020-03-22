#
# This module is designed to find/handle mysql(client) library
#
# Requirements:
# - CMake >= 2.8.3 (for new version of find_package_handle_standard_args)
#
# The following variables will be defined for your use:
#   - MYSQL_INCLUDE_DIRS  : mysql(client) include directory
#   - MYSQL_LIBRARIES     : mysql(client) libraries
#   - MYSQL_VERSION       : complete version of mysql(client) (x.y.z)
#   - MYSQL_VERSION_MAJOR : major version of mysql(client)
#   - MYSQL_VERSION_MINOR : minor version of mysql(client)
#   - MYSQL_VERSION_PATCH : patch version of mysql(client)
#
# How to use:
#   1) Copy this file in the root of your project source directory
#   2) Then, tell CMake to search this non-standard module in your project directory by adding to your CMakeLists.txt:
#        set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
#   3) Finally call find_package(MySQL) once
#
# Here is a complete sample to build an executable:
#
#   set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
#
#   find_package(MySQL REQUIRED) # Note: name is case sensitive
#
#   add_executable(myapp myapp.c)
#   include_directories(${MYSQL_INCLUDE_DIRS})
#   target_link_libraries(myapp ${MYSQL_LIBRARIES})
#   # with CMake >= 3.0.0, the last two lines can be replaced by the following
#   target_link_libraries(myapp MySQL::MySQL) # Note: case also matters here
#


#=============================================================================
# Copyright (c) 2013-2016, julp
#
# Distributed under the OSI-approved BSD License
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#=============================================================================

cmake_minimum_required(VERSION 2.8.3)

# "As of MySQL 5.7.9, MySQL distributions contain a mysqlclient.pc file that provides information about MySQL configuration for use by the pkg-config command."
find_package(PkgConfig QUIET)

########## Private ##########
if(NOT DEFINED MYSQL_PUBLIC_VAR_NS)
    set(MYSQL_PUBLIC_VAR_NS "MYSQL")
endif(NOT DEFINED MYSQL_PUBLIC_VAR_NS)
if(NOT DEFINED MYSQL_PRIVATE_VAR_NS)
    set(MYSQL_PRIVATE_VAR_NS "_${MYSQL_PUBLIC_VAR_NS}")
endif(NOT DEFINED MYSQL_PRIVATE_VAR_NS)
if(NOT DEFINED PC_MYSQL_PRIVATE_VAR_NS)
    set(PC_MYSQL_PRIVATE_VAR_NS "_PC${PC_MYSQL_PRIVATE_VAR_NS}")
endif(NOT DEFINED PC_MYSQL_PRIVATE_VAR_NS)

# Alias all MySQL_FIND_X variables to MYSQL_FIND_X
# Workaround for find_package: no way to force case of variable's names it creates (I don't want to change MY coding standard)
set(${MYSQL_PRIVATE_VAR_NS}_FIND_PKG_PREFIX "MySQL")
get_directory_property(${MYSQL_PRIVATE_VAR_NS}_CURRENT_VARIABLES VARIABLES)
foreach(${MYSQL_PRIVATE_VAR_NS}_VARNAME ${${MYSQL_PRIVATE_VAR_NS}_CURRENT_VARIABLES})
    if(${MYSQL_PRIVATE_VAR_NS}_VARNAME MATCHES "^${${MYSQL_PRIVATE_VAR_NS}_FIND_PKG_PREFIX}")
        string(REGEX REPLACE "^${${MYSQL_PRIVATE_VAR_NS}_FIND_PKG_PREFIX}" "${MYSQL_PUBLIC_VAR_NS}" ${MYSQL_PRIVATE_VAR_NS}_NORMALIZED_VARNAME ${${MYSQL_PRIVATE_VAR_NS}_VARNAME})
        set(${${MYSQL_PRIVATE_VAR_NS}_NORMALIZED_VARNAME} ${${${MYSQL_PRIVATE_VAR_NS}_VARNAME}})
    endif(${MYSQL_PRIVATE_VAR_NS}_VARNAME MATCHES "^${${MYSQL_PRIVATE_VAR_NS}_FIND_PKG_PREFIX}")
endforeach(${MYSQL_PRIVATE_VAR_NS}_VARNAME)

macro(_mysql_set_dotted_version VERSION_STRING)
    set(${MYSQL_PUBLIC_VAR_NS}_VERSION "${VERSION_STRING}")
    string(REGEX MATCHALL "[0-9]+" ${MYSQL_PRIVATE_VAR_NS}_VERSION_PARTS ${VERSION_STRING})
    list(GET ${MYSQL_PRIVATE_VAR_NS}_VERSION_PARTS 0 ${MYSQL_PUBLIC_VAR_NS}_VERSION_MAJOR)
    list(GET ${MYSQL_PRIVATE_VAR_NS}_VERSION_PARTS 1 ${MYSQL_PUBLIC_VAR_NS}_VERSION_MINOR)
    list(GET ${MYSQL_PRIVATE_VAR_NS}_VERSION_PARTS 2 ${MYSQL_PUBLIC_VAR_NS}_VERSION_PATCH)
endmacro(_mysql_set_dotted_version)

########## Public ##########
if(PKG_CONFIG_FOUND)
    pkg_check_modules(${PC_MYSQL_PRIVATE_VAR_NS} "mysqlclient" QUIET)
    if(${PC_MYSQL_PRIVATE_VAR_NS}_FOUND)
        if(${PC_MYSQL_PRIVATE_VAR_NS}_VERSION)
            _mysql_set_dotted_version("${${PC_MYSQL_PRIVATE_VAR_NS}_VERSION}")
        endif(${PC_MYSQL_PRIVATE_VAR_NS}_VERSION)
    endif(${PC_MYSQL_PRIVATE_VAR_NS}_FOUND)
endif(PKG_CONFIG_FOUND)

find_program(${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE mysql_config)
if(${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE)
    execute_process(OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND ${${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE} --cflags                 OUTPUT_VARIABLE ${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_C_FLAGS)
    execute_process(OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND ${${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE} --version                OUTPUT_VARIABLE ${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_VERSION)
    execute_process(OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND ${${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE} --variable=pkglibdir     OUTPUT_VARIABLE ${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_LIBRARY_DIR)
    execute_process(OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND ${${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE} --variable=pkgincludedir OUTPUT_VARIABLE ${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_INCLUDE_DIR)
#     execute_process(OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND ${${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE} --plugindir              OUTPUT_VARIABLE ${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_PLUGIN_DIR)
#     execute_process(OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND ${${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE} --socket                 OUTPUT_VARIABLE ${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_SOCKET)
#     execute_process(OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND ${${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE} --port                   OUTPUT_VARIABLE ${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_PORT)
#     execute_process(OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND ${${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE} --libmysqld-libs         OUTPUT_VARIABLE ${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_LIBRARY_EMBEDDED)

    _mysql_set_dotted_version("${${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_VERSION}")
endif(${MYSQL_PUBLIC_VAR_NS}_CONFIG_EXECUTABLE)

find_path(
    ${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIR
    NAMES mysql_version.h
    PATH_SUFFIXES include mysql
    PATHS ${${PC_MYSQL_PRIVATE_VAR_NS}_INCLUDE_DIRS} ${${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_INCLUDE_DIR}
)

if(MSVC)
    include(SelectLibraryConfigurations)
    # "On Windows, the static library is mysqlclient.lib and the dynamic library is libmysql.dll. Windows distributions also include libmysql.lib, a static import library needed for using the dynamic library."
    set(${MYSQL_PRIVATE_VAR_NS}_POSSIBLE_DEBUG_NAMES "mysqld mysqlclientd")
    set(${MYSQL_PRIVATE_VAR_NS}_POSSIBLE_RELEASE_NAMES "mysql mysqlclient")

    find_library(
        ${MYSQL_PUBLIC_VAR_NS}_LIBRARY_RELEASE
        NAMES ${${MYSQL_PRIVATE_VAR_NS}_POSSIBLE_RELEASE_NAMES}
        DOC "Release library for mysqlclient"
    )
    find_library(
        ${MYSQL_PUBLIC_VAR_NS}_LIBRARY_DEBUG
        NAMES ${${MYSQL_PRIVATE_VAR_NS}_POSSIBLE_DEBUG_NAMES}
        DOC "Debug library for mysqlclient"
    )

    select_library_configurations("${MYSQL_PUBLIC_VAR_NS}")
else(MSVC)
    # "On Unix (and Unix-like) sytems, the static library is libmysqlclient.a. The dynamic library is libmysqlclient.so on most Unix systems and libmysqlclient.dylib on OS X."
    find_library(
        ${MYSQL_PUBLIC_VAR_NS}_LIBRARY
        NAMES mysqlclient
        PATHS ${${PC_MYSQL_PRIVATE_VAR_NS}_LIBRARY_DIRS} ${${MYSQL_PUBLIC_VAR_NS}_MYSQL_CONFIG_LIBRARY_DIR}
    )
endif(MSVC)

if(${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIR AND NOT ${MYSQL_PUBLIC_VAR_NS}_VERSION)
    file(STRINGS "${${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIR}/mysql_version.h" ${MYSQL_PRIVATE_VAR_NS}_VERSION_NUMBER_DEFINITION LIMIT_COUNT 1 REGEX ".*#[ \t]*define[ \t]*MYSQL_VERSION_ID[ \t]*[0-9]+.*")
    string(REGEX REPLACE ".*# *define +MYSQL_VERSION_ID +([0-9]+).*" "\\1" ${MYSQL_PRIVATE_VAR_NS}_VERSION_NUMBER ${${MYSQL_PRIVATE_VAR_NS}_VERSION_NUMBER_DEFINITION})

    math(EXPR ${MYSQL_PUBLIC_VAR_NS}_VERSION_MAJOR "${${MYSQL_PRIVATE_VAR_NS}_VERSION_NUMBER} / 10000")
    math(EXPR ${MYSQL_PUBLIC_VAR_NS}_VERSION_MINOR "(${${MYSQL_PRIVATE_VAR_NS}_VERSION_NUMBER} - ${${MYSQL_PUBLIC_VAR_NS}_VERSION_MAJOR} * 10000) / 100")
    math(EXPR ${MYSQL_PUBLIC_VAR_NS}_VERSION_PATCH "${${MYSQL_PRIVATE_VAR_NS}_VERSION_NUMBER} - ${${MYSQL_PUBLIC_VAR_NS}_VERSION_MAJOR} * 10000 - ${${MYSQL_PUBLIC_VAR_NS}_VERSION_MINOR} * 100")
    set(${MYSQL_PUBLIC_VAR_NS}_VERSION "${${MYSQL_PUBLIC_VAR_NS}_VERSION_MAJOR}.${${MYSQL_PUBLIC_VAR_NS}_VERSION_MINOR}.${${MYSQL_PUBLIC_VAR_NS}_VERSION_PATCH}")
endif(${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIR AND NOT ${MYSQL_PUBLIC_VAR_NS}_VERSION)

# Check find_package arguments
include(FindPackageHandleStandardArgs)
if(${MYSQL_PUBLIC_VAR_NS}_FIND_REQUIRED AND NOT ${MYSQL_PUBLIC_VAR_NS}_FIND_QUIETLY)
    find_package_handle_standard_args(
        ${MYSQL_PUBLIC_VAR_NS}
        REQUIRED_VARS ${MYSQL_PUBLIC_VAR_NS}_LIBRARY ${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIR
        VERSION_VAR ${MYSQL_PUBLIC_VAR_NS}_VERSION
    )
else(${MYSQL_PUBLIC_VAR_NS}_FIND_REQUIRED AND NOT ${MYSQL_PUBLIC_VAR_NS}_FIND_QUIETLY)
    find_package_handle_standard_args(${MYSQL_PUBLIC_VAR_NS} "Could NOT find mysql(client)" ${MYSQL_PUBLIC_VAR_NS}_LIBRARY ${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIR)
endif(${MYSQL_PUBLIC_VAR_NS}_FIND_REQUIRED AND NOT ${MYSQL_PUBLIC_VAR_NS}_FIND_QUIETLY)

if(${MYSQL_PUBLIC_VAR_NS}_FOUND)
    # <deprecated>
    # for compatibility with previous versions, alias old MYSQL_(MAJOR|MINOR|PATCH)_VERSION to MYSQL_VERSION_$1
    set(${MYSQL_PUBLIC_VAR_NS}_MAJOR_VERSION ${${MYSQL_PUBLIC_VAR_NS}_VERSION_MAJOR})
    set(${MYSQL_PUBLIC_VAR_NS}_MINOR_VERSION ${${MYSQL_PUBLIC_VAR_NS}_VERSION_MINOR})
    set(${MYSQL_PUBLIC_VAR_NS}_PATCH_VERSION ${${MYSQL_PUBLIC_VAR_NS}_VERSION_PATCH})
    # </deprecated>
    set(${MYSQL_PUBLIC_VAR_NS}_LIBRARIES ${${MYSQL_PUBLIC_VAR_NS}_LIBRARY})
    set(${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIRS ${${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIR})
    if(CMAKE_VERSION VERSION_GREATER "3.0.0")
        if(NOT TARGET MySQL::MySQL)
            add_library(MySQL::MySQL UNKNOWN IMPORTED)
        endif(NOT TARGET MySQL::MySQL)
        if(${MYSQL_PUBLIC_VAR_NS}_LIBRARY_RELEASE)
            set_property(TARGET MySQL::MySQL APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
            set_target_properties(MySQL::MySQL PROPERTIES IMPORTED_LOCATION_RELEASE "${${MYSQL_PUBLIC_VAR_NS}_LIBRARY_RELEASE}")
        endif(${MYSQL_PUBLIC_VAR_NS}_LIBRARY_RELEASE)
        if(${MYSQL_PUBLIC_VAR_NS}_LIBRARY_DEBUG)
            set_property(TARGET MySQL::MySQL APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
            set_target_properties(MySQL::MySQL PROPERTIES IMPORTED_LOCATION_DEBUG "${${MYSQL_PUBLIC_VAR_NS}_LIBRARY_DEBUG}")
        endif(${MYSQL_PUBLIC_VAR_NS}_LIBRARY_DEBUG)
        if(${MYSQL_PUBLIC_VAR_NS}_LIBRARY)
            set_target_properties(MySQL::MySQL PROPERTIES IMPORTED_LOCATION "${${MYSQL_PUBLIC_VAR_NS}_LIBRARY}")
        endif(${MYSQL_PUBLIC_VAR_NS}_LIBRARY)
        set_target_properties(MySQL::MySQL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIR}")
    endif(CMAKE_VERSION VERSION_GREATER "3.0.0")
endif(${MYSQL_PUBLIC_VAR_NS}_FOUND)

mark_as_advanced(
    ${MYSQL_PUBLIC_VAR_NS}_INCLUDE_DIR
    ${MYSQL_PUBLIC_VAR_NS}_LIBRARY
)

########## <debug> ##########

if(${MYSQL_PUBLIC_VAR_NS}_DEBUG)

    function(mysql_debug _VARNAME)
        if(DEFINED ${MYSQL_PUBLIC_VAR_NS}_${_VARNAME})
            message("${MYSQL_PUBLIC_VAR_NS}_${_VARNAME} = ${${MYSQL_PUBLIC_VAR_NS}_${_VARNAME}}")
        else(DEFINED ${MYSQL_PUBLIC_VAR_NS}_${_VARNAME})
            message("${MYSQL_PUBLIC_VAR_NS}_${_VARNAME} = <UNDEFINED>")
        endif(DEFINED ${MYSQL_PUBLIC_VAR_NS}_${_VARNAME})
    endfunction(mysql_debug)

    # IN (args)
    mysql_debug("FIND_REQUIRED")
    mysql_debug("FIND_QUIETLY")
    mysql_debug("FIND_VERSION")
    # OUT
    # Linking
    mysql_debug("INCLUDE_DIRS")
    mysql_debug("LIBRARIES")
    # Version
    mysql_debug("VERSION_MAJOR")
    mysql_debug("VERSION_MINOR")
    mysql_debug("VERSION_PATCH")
    mysql_debug("VERSION")

endif(${MYSQL_PUBLIC_VAR_NS}_DEBUG)

########## </debug> ##########
