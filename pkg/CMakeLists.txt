cmake_minimum_required(VERSION 3.10)
project(pkg)

add_library(pkg STATIC lib.cpp)
target_include_directories(pkg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS pkg DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.hpp")
