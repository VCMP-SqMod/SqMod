# Create the HashLib library
add_library(HashLib STATIC
	include/hash.h
	include/hmac.h
	include/crc32.h crc32.cpp
	include/keccak.h keccak.cpp
	include/md5.h md5.cpp
	include/sha1.h sha1.cpp
	include/sha3.h sha3.cpp
	include/sha256.h sha256.cpp
	digest.cpp
)
# Configure include folders
target_include_directories(HashLib PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(HashLib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Make sure headers are available on MinGW
if (MINGW)
	target_include_directories(HashLib PRIVATE ${CMAKE_CURRENT_LIST_DIR}/mingwinc)
endif(MINGW)
# Enable LTO
if (LTO_ENABLED)
	target_link_libraries(HashLib PRIVATE -flto)
endif()