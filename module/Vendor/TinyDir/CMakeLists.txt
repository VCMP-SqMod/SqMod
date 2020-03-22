# Create the TinyDir library
add_library(TinyDir STATIC include/tinydir.h tinydir.c)
# Library includes
target_include_directories(TinyDir PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(TinyDir PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
