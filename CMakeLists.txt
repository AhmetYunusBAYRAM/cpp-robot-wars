cmake_minimum_required(VERSION 3.10)
project(RobotWars)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(RobotWars
    src/main.cpp
    src/Point.cpp
    src/Movable.cpp
    src/Robot.cpp
    src/Arena.cpp
)