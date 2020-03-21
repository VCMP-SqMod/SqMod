# Create the VCMP library
add_library(VCMP INTERFACE)
# Library includes
target_include_directories(VCMP INTERFACE ${CMAKE_CURRENT_LIST_DIR})
# Add it's sources
target_sources(VCMP INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/vcmp.h
	${CMAKE_CURRENT_SOURCE_DIR}/vcmp20.h
	${CMAKE_CURRENT_SOURCE_DIR}/vcmp21.h
)
