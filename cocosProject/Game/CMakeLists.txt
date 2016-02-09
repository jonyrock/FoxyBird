cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SRCS
    *.cpp
    Managers/*.cpp
    Scenes/*.cpp
    Models/*.cpp
    GameElements/*.cpp
    GUI/*.cpp   
)

add_library(Game STATIC ${SRCS})

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")