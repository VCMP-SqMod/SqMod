# Create the CivetWeb library
add_library(CivetWeb STATIC
	include/civetweb.h
	civetweb.c
)
# Configure include folders
target_include_directories(CivetWeb PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(CivetWeb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Feature selection
target_compile_definitions(CivetWeb PUBLIC USE_WEBSOCKET=1 USE_HTTP2=1 USE_TIMERS=1)
# Enable SSL
find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
	string(REPLACE "." ";" VERSION_LIST ${OPENSSL_VERSION})
	list(GET VERSION_LIST 0 OPENSSL_VERSION_MAJOR)
	list(GET VERSION_LIST 1 OPENSSL_VERSION_MINOR)
	list(GET VERSION_LIST 2 OPENSSL_VERSION_PATCH)
	if(OPENSSL_VERSION_MAJOR EQUAL 1 AND OPENSSL_VERSION_MINOR EQUAL 0)
		target_compile_definitions(CivetWeb PUBLIC OPENSSL_API_1_0=1)
	elseif(OPENSSL_VERSION_MAJOR EQUAL 1 AND OPENSSL_VERSION_MINOR EQUAL 1)
		target_compile_definitions(CivetWeb PUBLIC OPENSSL_API_1_1=1)
	endif()
	target_include_directories(CivetWeb PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(CivetWeb PRIVATE OpenSSL::SSL OpenSSL::Crypto)
else()
	target_compile_definitions(CivetWeb PUBLIC NO_SSL=1)
endif()
# Enable LTO
if (LTO_ENABLED)
	target_link_libraries(CivetWeb PRIVATE -flto)
endif()
# Allow dynamic loading of SSL
target_link_libraries(CivetWeb PUBLIC dl)
# Find the available thread library
find_package(Threads)
target_link_libraries(CivetWeb PUBLIC ${CMAKE_THREAD_LIBS_INIT})
# OS and compiler checks.
if(WIN32)
	target_compile_definitions(CivetWeb PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(CivetWeb PUBLIC Ws2_32)
endif()
# Need the realtime library if we're using timers
if(UNIX AND NOT APPLE)
	target_link_libraries(CivetWeb PUBLIC rt)
endif()