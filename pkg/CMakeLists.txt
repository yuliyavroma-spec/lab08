cmake_minimum_required(VERSION 3.14)
project(pkg)

add_library(pkg STATIC src/pkg.cpp)
target_include_directories(pkg PUBLIC include)

install(TARGETS pkg DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
