# Create the ConcurrentQueue library
add_library(ConcurrentQueue STATIC
	include/concurrentqueue.h
	include/lightweightsemaphore.h
	include/blockingconcurrentqueue.h
	concurrentqueue.cpp
)
# Configure include folders
target_include_directories(ConcurrentQueue PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(ConcurrentQueue PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
target_compile_definitions(ConcurrentQueue PRIVATE _WIN32_WINNT=0x0601)
