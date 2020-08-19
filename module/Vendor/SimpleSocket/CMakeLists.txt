# Create library
add_library(SimpleSocket STATIC
    include/HostConf.h
    include/StatTimer.h
    include/ActiveSocket.h ActiveSocket.cpp
    include/PassiveSocket.h PassiveSocket.cpp
    include/SimpleSocket.h SimpleSocket.cpp
)
# Configure include folders
target_include_directories(SimpleSocket PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(SimpleSocket PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
target_compile_definitions(SimpleSocket PRIVATE _WIN32_WINNT=0x0601)
# Enable LTO
if (LTO_ENABLED)
    target_link_libraries(SimpleSocket PRIVATE -flto)
endif()
# OS and compiler checks.
if(WIN32)
    target_link_libraries(SimpleSocket PUBLIC Ws2_32)
endif()
