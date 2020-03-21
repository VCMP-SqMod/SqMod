# Create the Sqrat library
add_library(Sqrat INTERFACE)
# Set library compiler options
target_compile_definitions(Sqrat INTERFACE SCRAT_USE_EXCEPTIONS=1 SCRAT_USE_CXX11_OPTIMIZATIONS=1)
# Set specific compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	#target_compile_options(Sqrat PRIVATE -w
	#)
endif()
# Configure macro options
target_compile_definitions(Sqrat INTERFACE GARBAGE_COLLECTOR=1)
# Library includes
target_include_directories(Sqrat INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Sqrat INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# Link to required libraries
target_link_libraries(Sqrat INTERFACE Squirrel)
# Add it's sources
target_sources(Sqrat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sqrat.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratAllocator.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratArray.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratClass.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratConst.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratFunction.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratGlobalMethods.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratMemberMethods.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratObject.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratOverloadMethods.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratScript.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratTable.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratTypes.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqrat/sqratUtil.h
)
