# Create the SimpleINI library
add_library(SimpleINI STATIC
	include/ConvertUTF.h
	include/SimpleIni.h
	ConvertUTF.cpp
)
# Configure include folders
target_include_directories(SimpleINI PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(SimpleINI PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
