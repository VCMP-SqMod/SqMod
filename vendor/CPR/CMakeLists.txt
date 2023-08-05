# Create the CPR library
add_library(CPR STATIC
    # Source files
    cpr/error.cpp
    cpr/file.cpp
    cpr/interceptor.cpp
    cpr/multipart.cpp
    cpr/multiperform.cpp
    cpr/parameters.cpp
    cpr/payload.cpp
    cpr/proxies.cpp
    cpr/proxyauth.cpp
    cpr/redirect.cpp
    cpr/response.cpp
    cpr/session.cpp
    cpr/ssl_ctx.cpp
    cpr/threadpool.cpp
    cpr/timeout.cpp
    cpr/unix_socket.cpp
    cpr/util.cpp
    cpr/accept_encoding.cpp
    cpr/async.cpp
    cpr/auth.cpp
    cpr/bearer.cpp
    cpr/callback.cpp
    cpr/cert_info.cpp
    cpr/cookies.cpp
    cpr/cprtypes.cpp
    cpr/curl_container.cpp
    cpr/curlholder.cpp
    cpr/curlmultiholder.cpp
    # Header Files
    include/cpr/bearer.h
    include/cpr/body.h
    include/cpr/buffer.h
    include/cpr/callback.h
    include/cpr/cert_info.h
    include/cpr/connect_timeout.h
    include/cpr/cookies.h
    include/cpr/cpr.h
    include/cpr/cprtypes.h
    include/cpr/curl_container.h
    include/cpr/curlholder.h
    include/cpr/curlmultiholder.h
    include/cpr/error.h
    include/cpr/file.h
    include/cpr/filesystem.h
    include/cpr/http_version.h
    include/cpr/interceptor.h
    include/cpr/interface.h
    include/cpr/limit_rate.h
    include/cpr/local_port.h
    include/cpr/local_port_range.h
    include/cpr/low_speed.h
    include/cpr/multipart.h
    include/cpr/multiperform.h
    include/cpr/parameters.h
    include/cpr/payload.h
    include/cpr/proxies.h
    include/cpr/proxyauth.h
    include/cpr/range.h
    include/cpr/redirect.h
    include/cpr/reserve_size.h
    include/cpr/resolve.h
    include/cpr/response.h
    include/cpr/session.h
    include/cpr/singleton.h
    include/cpr/ssl_ctx.h
    include/cpr/ssl_options.h
    include/cpr/status_codes.h
    include/cpr/threadpool.h
    include/cpr/timeout.h
    include/cpr/unix_socket.h
    include/cpr/user_agent.h
    include/cpr/util.h
    include/cpr/verbose.h
    include/cpr/accept_encoding.h
    include/cpr/api.h
    include/cpr/async.h
    include/cpr/async_wrapper.h
    include/cpr/auth.h
)
# Library includes
target_include_directories(CPR PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cpr)
target_include_directories(CPR PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Disable deprecated warnings
if (NOT MSVC)
    target_compile_options(CPR PRIVATE -Wno-deprecated-declarations)
endif()
# Find CURL
find_package(CURL REQUIRED)
if (CURL_FOUND)
    set(SSL_ENABLED ON CACHE INTERNAL "" FORCE)
else()
    message(FATAL_ERROR "Unable to find CURL library.")
endif()
# Link to CURL
target_link_libraries(CPR PUBLIC curl)
