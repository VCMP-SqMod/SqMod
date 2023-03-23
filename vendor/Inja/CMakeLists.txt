# Create the inja template library
add_library(inja STATIC
	include/inja/inja.hpp inja.cpp
)
# Configure include folders
target_include_directories(inja PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(inja PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
if (WIN32)
	target_compile_definitions(inja PRIVATE _WIN32_WINNT=0x0601)
endif()
# Link to nlohmann json library
target_link_libraries(inja PRIVATE nlohmann_json)
