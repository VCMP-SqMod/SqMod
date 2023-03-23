# Create the CPR library
add_library(CPR STATIC
    # Source files
    cpr/auth.cpp
    cpr/bearer.cpp
    cpr/cookies.cpp
    cpr/cprtypes.cpp
    cpr/curl_container.cpp
    cpr/curlholder.cpp
    cpr/error.cpp
    cpr/multipart.cpp
    cpr/parameters.cpp
    cpr/payload.cpp
    cpr/proxies.cpp
    cpr/proxyauth.cpp
    cpr/redirect.cpp
    cpr/session.cpp
    cpr/timeout.cpp
    cpr/unix_socket.cpp
    cpr/util.cpp
    cpr/response.cpp
    # Header files
    include/cpr/api.h
    include/cpr/auth.h
    include/cpr/bearer.h
    include/cpr/body.h
    include/cpr/cookies.h
    include/cpr/cpr.h
    include/cpr/cprtypes.h
    include/cpr/curlholder.h
    include/cpr/curlholder.h
    include/cpr/digest.h
    include/cpr/error.h
    include/cpr/interface.h
    include/cpr/limit_rate.h
    include/cpr/multipart.h
    include/cpr/ntlm.h
    include/cpr/parameters.h
    include/cpr/payload.h
    include/cpr/proxies.h
    include/cpr/proxyauth.h
    include/cpr/redirect.h
    include/cpr/response.h
    include/cpr/session.h
    include/cpr/ssl_options.h
    include/cpr/timeout.h
    include/cpr/unix_socket.h
    include/cpr/util.h
    include/cpr/verbose.h
)
# Library includes
target_include_directories(CPR PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cpr)
target_include_directories(CPR PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Disable deprecated warnings
if (NOT MSVC)
    target_compile_options(CPR PRIVATE -Wno-deprecated-declarations)
endif()
# Find CURK
find_package(CURL REQUIRED)
if (CURL_FOUND)
    set(SSL_ENABLED ON CACHE INTERNAL "" FORCE)
else()
    message(FATAL_ERROR "Unable to find CURL library.")
endif()
# Link to CURL
target_link_libraries(CPR PUBLIC curl)
