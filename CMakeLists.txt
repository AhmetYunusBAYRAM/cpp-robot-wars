cmake_minimum_required(VERSION 3.10)
project(RobotWars)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows için özel ayarlar
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
endif()

add_executable(RobotWars
    src/main.cpp
    src/Arena.cpp
    src/Point.cpp
    src/Movable.cpp
    src/Robot.cpp
)

target_include_directories(RobotWars PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)