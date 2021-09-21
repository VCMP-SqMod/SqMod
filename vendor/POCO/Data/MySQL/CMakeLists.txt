# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(MYSQL_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(MYSQL_SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND MYSQL_SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(DataMySQL ${MYSQL_SRCS})
add_library(Poco::DataMySQL ALIAS DataMySQL)
set_target_properties(DataMySQL
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoDataMySQL
	DEFINE_SYMBOL MySQL_EXPORTS
)
if (ENABLE_BUILTIN_MYSQL_C)
	target_link_libraries(DataMySQL PUBLIC Poco::Data mariadbclient)
else()
	target_link_libraries(DataMySQL PUBLIC Poco::Data MySQL::client)
endif()
target_include_directories(DataMySQL
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_definitions(DataMySQL PUBLIC THREADSAFE NO_TCL)

if(WIN32)
	target_compile_definitions(DataMySQL PUBLIC __LCC__) #__LCC__ define used by MySQL.h
endif()

# Some instalations don't have mysql_reset_connection() even if their version indicates otherwise
# Debian 9 (Stretch) is one such example
if(UNIX)
    file(STRINGS "${MYSQL_INCLUDE_DIR}/mysql/mysql.h" lines REGEX "mysql_reset_connection")
    if(lines)
        target_compile_definitions(DataMySQL PRIVATE HAVE_MYSQL_RESET_CONNECTION)
    endif()
endif()

# Meh...
if (NOT ENABLE_BUILTIN_MYSQL_C)
	POCO_INSTALL(DataMySQL)
	POCO_GENERATE_PACKAGE(DataMySQL)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
