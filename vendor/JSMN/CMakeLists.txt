# Create the JSMN library
add_library(JSMN STATIC include/jsmn.h jsmn.c)
# Library includes
target_include_directories(JSMN PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(JSMN PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Compile time options
target_compile_definitions(JSMN PUBLIC JSMN_UTF8=1)
