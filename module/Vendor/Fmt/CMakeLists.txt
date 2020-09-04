# Create library
add_library(FmtLib STATIC
	include/fmt/chrono.h
	include/fmt/color.h
	include/fmt/compile.h
	include/fmt/core.h
	include/fmt/format-inl.h
	include/fmt/format.h
	include/fmt/locale.h
	include/fmt/os.h
	include/fmt/ostream.h
	include/fmt/posix.h
	include/fmt/printf.h
	include/fmt/ranges.h
	format.cc
	os.cc
)
# Configure include folders
target_include_directories(FmtLib PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(FmtLib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
target_compile_definitions(FmtLib PRIVATE _WIN32_WINNT=0x0601)
