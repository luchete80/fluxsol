cmake_minimum_required(VERSION 2.8.9)

set (NAME Searching-Files)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -static-libgcc -static-libstdc++ -g")

project(${NAME})

add_executable(${NAME} Searching-Files.cpp)