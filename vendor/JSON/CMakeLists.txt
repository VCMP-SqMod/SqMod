# Create the nlohmann json library
add_library(nlohmann_json STATIC
	include/nlohmann/json_fwd.hpp
	include/nlohmann/json.hpp
	JSON.cpp
)
# Configure include folders
target_include_directories(nlohmann_json PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(nlohmann_json PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
if (WIN32)
	target_compile_definitions(nlohmann_json PRIVATE _WIN32_WINNT=0x0601)
endif()
