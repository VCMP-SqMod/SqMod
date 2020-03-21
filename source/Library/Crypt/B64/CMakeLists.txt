# Create the B64Lib library
add_library(B64Lib STATIC include/b64.h decode.c decode.c)
# Configure include folders
target_include_directories(B64Lib PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(B64Lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure build options
target_compile_definitions(B64Lib PRIVATE )
