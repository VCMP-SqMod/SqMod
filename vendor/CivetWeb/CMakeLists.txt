# Create the CivetWeb library
add_library(CivetWeb STATIC
    include/civetweb.h civetweb.c
    include/CivetServer.h CivetServer.cpp
    mod_mbedtls.inl
    mod_zlib.inl
    openssl_dl.inl
    response.inl
    sha1.inl
    timer.inl
    wolfssl_extras.inl
    handle_form.inl
    md5.inl
    mod_http2.inl
)
# Configure include folders
target_include_directories(CivetWeb PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(CivetWeb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
if(WIN32 OR MINGW)
    target_compile_definitions(CivetWeb PRIVATE _WIN32_WINNT=0x0601)
endif()
# Generic macro options
target_compile_definitions(CivetWeb PRIVATE USE_TIMERS=1 USE_WEBSOCKET=1 USE_IPV6=1)
# Look for SSL
find_package(OpenSSL)
# Check SSL status
if (OPENSSL_FOUND)
    message(STATUS "CivetWeb: OpenSSL was found")
    target_link_libraries(CivetWeb PUBLIC OpenSSL::Crypto OpenSSL::SSL)
    string(REPLACE "." ";" OPENSSL_VERSION_LIST ${OPENSSL_VERSION})
    list(GET OPENSSL_VERSION_LIST 0 OPENSSL_VERSION_MAJOR)
    list(GET OPENSSL_VERSION_LIST 1 OPENSSL_VERSION_MINOR)
    list(GET OPENSSL_VERSION_LIST 2 OPENSSL_VERSION_PATCH)
    # Tell the library what SSL version to expect
    target_compile_definitions(CivetWeb PRIVATE "OPENSSL_API_${OPENSSL_VERSION_MAJOR}_${OPENSSL_VERSION_MINOR}")
    message(STATUS "CivetWeb: OPENSSL_API_${OPENSSL_VERSION_MAJOR}_${OPENSSL_VERSION_MINOR}")
else()
    target_compile_definitions(CivetWeb PRIVATE NO_SSL=1)
endif()
# Look for ZLib
find_package(ZLIB)
# Check ZLib status
if (ZLIB_FOUND)
    message(STATUS "CivetWeb: ZLib was found")
    target_link_libraries(CivetWeb PUBLIC ZLIB::ZLIB)
    target_compile_definitions(CivetWeb PRIVATE USE_ZLIB=1)
endif()
