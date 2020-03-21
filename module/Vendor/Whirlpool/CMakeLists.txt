# Create the WhirlpoolLib library
add_library(WhirlpoolLib STATIC
	include/ustd.h
	include/whirlpool.h
	byte_order.h byte_order.c
	whirlpool.c
	whirlpool_sbox.c
)
# Configure include folders
target_include_directories(WhirlpoolLib PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(WhirlpoolLib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
