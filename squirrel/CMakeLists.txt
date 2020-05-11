# Create the Squirrel library
add_library(Squirrel STATIC
	include/sqconfig.h
	include/sqstdaux.h
	include/sqstdblob.h
	include/sqstdmath.h
	include/sqstdstring.h
	include/sqstdsystem.h
	include/squirrel.h
	include/squirrelex.h
	sqapi.cpp
	sqapiex.cpp
	sqarray.h
	sqbaselib.cpp
	sqclass.cpp sqclass.h
	sqclosure.h
	sqcompiler.cpp sqcompiler.h
	sqdebug.cpp
	sqfuncproto.h
	sqfuncstate.cpp sqfuncstate.h
	sqlexer.cpp sqlexer.h
	sqmem.cpp
	sqobject.cpp sqobject.h
	sqopcodes.h
	sqpcheader.h
	sqstate.cpp sqstate.h
	sqstring.h
	sqtable.cpp sqtable.h
	squserdata.h
	squtils.h
	sqvm.cpp sqvm.h
	stdlib/sqstdaux.cpp
	stdlib/sqstdblob.cpp
	stdlib/sqstdblobimpl.h
	stdlib/sqstdio.cpp
	stdlib/sqstdmath.cpp
	stdlib/sqstdrex.cpp
	stdlib/sqstdstream.cpp
	stdlib/sqstdstream.h
	stdlib/sqstdstring.cpp
	stdlib/sqstdsystem.cpp
)
# Determine if build mode
if(CMAKE_BUILD_TYPE MATCHES Release)
	target_compile_definitions(Squirrel PRIVATE NDEBUG=1)
else()
	target_compile_definitions(Squirrel PRIVATE _DEBUG=1)
endif()
# Make sure Squirrel knows this is 64 bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(Squirrel PUBLIC _SQ64)
endif()
# Set speciffic compiler options
if (GCC OR MINGW)
	target_compile_options(Squirrel PRIVATE -w
		-fno-exceptions
		-fno-rtti
		-fno-strict-aliasing
		-Wno-format
		-Wno-unused-variable
		-Wno-unused-but-set-variable
	)
endif()
# Configure build options
target_compile_definitions(Squirrel PRIVATE GARBAGE_COLLECTOR=1)
# Library includes
target_include_directories(Squirrel PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Squirrel PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(Squirrel PRIVATE ${CMAKE_CURRENT_LIST_DIR}/stdlib)
# Create the Squirrel library
add_library(SquirrelAPI INTERFACE)
# Library includes
target_include_directories(SquirrelAPI INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# Switch to plugin-development mode
if(PLUGIN_DEVEL)
	target_compile_definitions(SquirrelAPI INTERFACE SQMOD_PLUGIN_API=1)
endif()