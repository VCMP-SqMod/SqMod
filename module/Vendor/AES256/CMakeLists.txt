# Create the AES256Lib library
add_library(AES256Lib STATIC include/aes256.h aes256.cpp)
# Configure include folders
target_include_directories(AES256Lib PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(AES256Lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Enable LTO
if (LTO_ENABLED)
    target_link_libraries(AES256Lib PRIVATE -flto)
endif()