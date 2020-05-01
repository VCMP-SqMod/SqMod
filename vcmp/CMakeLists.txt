# Create the VCMP library
add_library(VCMP STATIC vcmp.h vcmp.c vcmp20.h vcmp21.h)
# Checkf for API version
if (ENABLE_API21)
	target_compile_definitions(VCMP PUBLIC VCMP_SDK_2_1=1)
endif()
# Library includes
target_include_directories(VCMP PUBLIC ${CMAKE_CURRENT_LIST_DIR})
