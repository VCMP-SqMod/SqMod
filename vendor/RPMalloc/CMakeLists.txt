# Create library
add_library(RPMalloc STATIC include/rpmalloc.h rpmalloc.c)
# Configure include folders
target_include_directories(RPMalloc PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(RPMalloc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
target_compile_definitions(RPMalloc PRIVATE _WIN32_WINNT=0x0601)
# Debug options
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(RPMalloc PUBLIC _DEBUG ENABLE_VALIDATE_ARGS=1)
else()
    target_compile_definitions(RPMalloc PUBLIC NDEBUG)
endif()
