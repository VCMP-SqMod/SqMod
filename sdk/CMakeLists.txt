# Create the SqSDK library
add_library(SqSDKAPI INTERFACE)
# Library includes
target_include_directories(SqSDKAPI INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# Link to required libraries
target_link_libraries(SqSDKAPI INTERFACE VCMP SquirrelAPI)
# Create the SqSDK library
add_library(SqSDK STATIC 
	include/SqConfSDK.h
	include/SqAPI.h
	include/SqMod.h
	SqMod.cpp
)
# Library includes
target_include_directories(SqSDK PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(SqSDK PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Link to required libraries
target_link_libraries(SqSDK PRIVATE VCMP SquirrelAPI)
# Switch to plugin-development mode
if(PLUGIN_DEVEL)
	target_compile_definitions(SqSDK PUBLIC SQMOD_PLUGIN_API=1)
endif()