# Create the CivetWeb library
add_library(CivetWeb STATIC
	include/civetweb.h
	civetweb.c
)
# Configure include folders
target_include_directories(CivetWeb PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(CivetWeb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
target_compile_definitions(CivetWeb PRIVATE _WIN32_WINNT=0x0601)
# Enable LTO
if (LTO_ENABLED)
	target_link_libraries(CivetWeb PRIVATE -flto)
endif()
# Allow dynamic loading of SSL
target_link_libraries(CivetWeb PUBLIC dl)
# Find the available thread library
find_package(Threads)
target_link_libraries(CivetWeb PUBLIC ${CMAKE_THREAD_LIBS_INIT})
# OS and compiler checks.
if(WIN32)
    target_link_libraries(CivetWeb PUBLIC Ws2_32)
endif()
# Need the realtime library if we're using timers
if(UNIX AND NOT APPLE)
	target_link_libraries(CivetWeb PUBLIC rt)
endif()