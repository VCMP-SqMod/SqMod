# Create the xxHash library
add_library(xxHash STATIC
	xxhash.h
	xxhash.c
)
# Set speciffic options
target_compile_options(xxHash PRIVATE -fvisibility=hidden)
# Includes
target_include_directories(xxHash PRIVATE ${CMAKE_CURRENT_LIST_DIR})
# Private library defines
#target_compile_definitions(xxHash PRIVATE )
# Public library defines
# target_compile_definitions(xxHash PUBLIC )
