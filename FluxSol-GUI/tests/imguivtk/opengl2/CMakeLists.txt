cmake_minimum_required(VERSION 2.8.4)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

set (NAME imguivtkdemo)
project(${NAME})

set (GLFW_DIR "$ENV{GLFW_DIR}")
set (IMGUI_DIR "$ENV{IMGUI_DIR}")

include_directories ("${IMGUI_DIR}")
include_directories ("${GLFW_DIR}/include")

link_directories ("${IMGUI_DIR}/bin")
link_directories ("${GLFW_DIR}/lib")

add_executable(${NAME} main.cpp imgui_impl_glfw.cpp timer.cpp)

target_link_libraries (${NAME} imgui glfw3 opengl32)