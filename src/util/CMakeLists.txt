add_library(util_lib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp)
target_include_directories(util_lib PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(util_lib PUBLIC spdlog::spdlog)