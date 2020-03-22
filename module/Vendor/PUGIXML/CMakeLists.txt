# Create library
add_library(PUGIXML STATIC include/pugiconfig.hpp include/pugixml.hpp pugixml.cpp)
# Configure include folders
target_include_directories(PUGIXML PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(PUGIXML PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

