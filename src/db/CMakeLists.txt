add_library(db_lib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/DatabaseMgr.cpp)
target_include_directories(db_lib PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(db_lib PUBLIC SQLite::SQLite3 util_lib)
