cmake_minimum_required(VERSION 3.10)
project(marmara_edu_assignments)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(marmara_edu_assignments
    src/main.cpp
    src/Arena.cpp
    src/Point.cpp
    src/Movable.cpp
    src/Robot.cpp
)

target_include_directories(marmara_edu_assignments PRIVATE include)