# Create the Squirrel module
add_library(SqHello MODULE Module.cpp
	Common.hpp Common.cpp
)
# Compile definitions
target_compile_definitions(SqHello PUBLIC SQMOD_PLUGIN_API=1)
# Determine if build mode
if(CMAKE_BUILD_TYPE MATCHES Release)
	target_compile_definitions(SqHello PRIVATE NDEBUG=1)
else()
	target_compile_definitions(SqHello PRIVATE _DEBUG=1 SQMOD_EXCEPTLOC=1)
endif()
# Link to base libraries
target_link_libraries(SqHello VCMP SquirrelAPI Sqrat SqSDK)
# Don't prefix the module binary.
set_target_properties(SqHello PROPERTIES PREFIX "")
# Custmize module binary name/
if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT FORCE_32BIT_BIN)
		set_target_properties(SqHello PROPERTIES OUTPUT_NAME "mod_hello_64")
	else()
		set_target_properties(SqHello PROPERTIES OUTPUT_NAME "mod_hello_32")
	endif()
else(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT FORCE_32BIT_BIN)
		set_target_properties(SqHello PROPERTIES OUTPUT_NAME "mod_hello_64")
	else()
		set_target_properties(SqHello PROPERTIES OUTPUT_NAME "mod_hello_32")
	endif()
endif(WIN32)
# Include current directory in the search path
target_include_directories(SqHello PRIVATE ${CMAKE_CURRENT_LIST_DIR})
# Copy module into the plugins folder
add_custom_command(TARGET SqHello POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SqHello> "${PROJECT_SOURCE_DIR}/bin/plugins")