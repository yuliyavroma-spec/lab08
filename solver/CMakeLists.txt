cmake_minimum_required(VERSION 3.4)
project(solver_app)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(solver 
    solver.cpp
    ../formatter_ex/formatter_ex.cpp
    ../formatter_lib/formatter.cpp
    ../solver_lib/solver.cpp
)
