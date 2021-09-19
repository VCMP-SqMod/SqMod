# Create the UTF8Lib library
add_library(UTF8Lib STATIC
	utf8.h
	utf8.c
)
# Set speciffic options
target_compile_options(UTF8Lib PRIVATE -fvisibility=hidden)
# Includes
target_include_directories(UTF8Lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})
# Private library defines
#target_compile_definitions(UTF8Lib PRIVATE )
# Public library defines
# target_compile_definitions(UTF8Lib PUBLIC )
