# This is the libmaxminddb project.
PROJECT(libmaxminddb C)

# Config options
SET(MM_VERSION_MAJOR 1)
SET(MM_VERSION_MINOR 4)
SET(MM_VERSION_PATCH 2)
SET(MM_PACKAGE "libmaxminddb")
SET(MM_PACKAGE_BUGREPORT "support@maxmind.com")
SET(MM_PACKAGE_NAME "libmaxminddb")
SET(MM_PACKAGE_STRING "${MM_PACKAGE_NAME} ${MM_VERSION_MAJOR}.${MM_VERSION_MINOR}.${MM_VERSION_PATCH}")
SET(MM_PACKAGE_TARNAME "libmaxminddb")
SET(MM_PACKAGE_URL "http://maxmind.github.io/libmaxminddb/")
SET(MM_PACKAGE_VERSION "${MM_VERSION_MAJOR}.${MM_VERSION_MINOR}.${MM_VERSION_PATCH}")
SET(MM_VERSION "${MM_VERSION_MAJOR}.${MM_VERSION_MINOR}.${MM_VERSION_PATCH}")

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/include/maxminddb_config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/include/maxminddb_config.h)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/include/maxminddb_config.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/include/maxminddb_config.h)

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/include)

# Create library
add_library(MaxmindDB STATIC include/maxminddb_config.h.in
	include/maxminddb.h src/maxminddb.c
	src/data-pool.h src/data-pool.c
	src/maxminddb-compat-util.h
)
# Set C99 standard
if (CMAKE_VERSION VERSION_LESS "3.1")
	if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
		set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
	endif ()
else ()
	set(CMAKE_C_STANDARD 99)
endif ()
# Configure include folders
target_include_directories(MaxmindDB PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(MaxmindDB PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
#target_compile_definitions(MaxmindDB PRIVATE )
# Configure build options
if(GCC OR MINGW)
	target_compile_options(MaxmindDB PRIVATE -Wno-unused -Wno-incompatible-pointer-types)
endif()
