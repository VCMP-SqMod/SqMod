# Create the HashLib library
add_library(MuJS STATIC include/mujs.h
	jscompile.c
	jscompile.h
	jsdate.c
	jsdtoa.c
	jsdump.c
	jserror.c
	jsfunction.c
	jsgc.c
	jsi.h
	jsintern.c
	jslex.c
	jslex.h
	jsmath.c
	jsnumber.c
	jsobject.c
	json.c
	jsparse.c
	jsparse.h
	jsproperty.c
	jsregexp.c
	jsrepr.c
	jsrun.c
	jsrun.h
	jsstate.c
	jsstring.c
	jsvalue.c
	jsvalue.h
	opnames.h
	pp.c
	regexp.c
	regexp.h
	utf.c
	utf.h
	utftype.c
	astnames.h
	jsarray.c
	jsboolean.c
	jsbuiltin.c
	jsbuiltin.h
)
# Configure include folders
target_include_directories(MuJS PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(MuJS PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
target_compile_definitions(MuJS PRIVATE _WIN32_WINNT=0x0601)
# Enable LTO
if (LTO_ENABLED)
	target_link_libraries(MuJS PRIVATE -flto)
endif()