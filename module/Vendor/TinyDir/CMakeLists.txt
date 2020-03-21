# Create the TinyDir library
add_library(TinyDir INTERFACE)
# Library includes
target_include_directories(TinyDir INTERFACE ${CMAKE_CURRENT_LIST_DIR})
# Add it's sources
target_sources(TinyDir INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/tinydir.h)
