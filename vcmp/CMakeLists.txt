# Create the VCMP library
add_library(VCMP STATIC vcmp.h vcmp.c vcmp20.h vcmp21.h)
# Library includes
target_include_directories(VCMP PUBLIC ${CMAKE_CURRENT_LIST_DIR})

