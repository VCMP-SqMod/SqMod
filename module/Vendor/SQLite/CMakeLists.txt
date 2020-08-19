# Create library
add_library(SQLite STATIC include/sqlite3.h sqlite3.c)
# Configure include folders
target_include_directories(SQLite PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(SQLite PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Configure macro options
target_compile_definitions(SQLite PRIVATE _WIN32_WINNT=0x0601 SQLITE_ENABLE_FTS3=1 SQLITE_ENABLE_FTS4=1 SQLITE_ENABLE_FTS5=1 SQLITE_ENABLE_JSON1=1 SQLITE_ENABLE_RTREE=1 SQLITE_ENABLE_GEOPOLY=1)
# Enable LTO
if (LTO_ENABLED)
    target_link_libraries(SQLite PRIVATE -flto)
endif()
