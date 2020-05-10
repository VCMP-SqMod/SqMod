# Create the Sqrat library
add_library(Sqrat STATIC sqrat.cpp sqrat.h
	sqrat/sqratAllocator.h
	sqrat/sqratArray.h
	sqrat/sqratClass.h
	sqrat/sqratConst.h
	sqrat/sqratFunction.h
	sqrat/sqratGlobalMethods.h
	sqrat/sqratMemberMethods.h
	sqrat/sqratObject.h
	sqrat/sqratOverloadMethods.h
	sqrat/sqratScript.h
	sqrat/sqratTable.h
	sqrat/sqratTypes.h
	sqrat/sqratUtil.h
)
# Determine if build mode
if(CMAKE_BUILD_TYPE MATCHES Release)
	target_compile_definitions(Sqrat PRIVATE NDEBUG=1)
else()
	target_compile_definitions(Sqrat PRIVATE _DEBUG=1)
endif()
# Set library compiler options
target_compile_definitions(Sqrat PUBLIC SCRAT_USE_EXCEPTIONS=1 SCRAT_USE_CXX11_OPTIMIZATIONS=1)
# Switch to plugin-development mode
if(PLUGIN_DEVEL)
	target_compile_definitions(Sqrat PUBLIC SQMOD_PLUGIN_API=1)
endif()
# Set specific compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	#target_compile_options(Sqrat PRIVATE -w
	#)
endif()
# Library includes
target_include_directories(Sqrat PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Sqrat PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Link to required libraries
target_link_libraries(Sqrat PUBLIC Squirrel)
