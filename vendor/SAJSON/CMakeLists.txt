# Create the SAJSON library
add_library(SAJSON STATIC include/sajson.h sajson.cpp)
# Library includes
target_include_directories(SAJSON PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(SAJSON PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
